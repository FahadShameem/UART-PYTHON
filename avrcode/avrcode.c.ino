#define F_CPU 16000000UL // Define CPU frequency as 16 MHz

#include <avr/io.h>
#include <util/delay.h>

#define BAUD 2400
#define UBRR_VALUE ((F_CPU / (16UL * BAUD))- 1) // from datasheet aprox val 415
#define EEPROM_MAX_SIZE 1024

uint16_t i = 0;


void uart_send(char data) 
{
    while (!(UCSR0A & (1 << UDRE0))); 
    UDR0 = data;                     
}


char uart_read() 
{
    while (!(UCSR0A & (1 << RXC0))); 
    return UDR0;                    
}


void eeprom_write(uint16_t address, uint8_t data) 
{
    while (EECR & (1 << EEPE));//wait until prevoius write complete
    EEARL = address & 0X00FF;
    EEARH = (address>>8);
    EEDR = data;

    EECR |= (1 << EEMPE);
    EECR |= (1 << EEPE);//setting bit to write
}


char eeprom_read(uint16_t address) 
{
    while (EECR & (1 << EEPE));//safety check if any write operation going on
    EEARL = address & 0x00FF;
    EEARH = (address >> 8);
    EECR |= (1 << EERE);//setting bit to read
    while(EECR & (1 << EERE));//wait until read complete
    return EEDR;
}

int main() 
{
    // Initialize UART
    uint16_t UBRR = UBRR_VALUE;
    UBRR0H = (UBRR >> 8);
    UBRR0L = UBRR & 0X00FF;       

    UCSR0B = (1 << RXEN0) | (1 << TXEN0);//enabling rx and tx
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);// standard uart `

    while (1) 
    {
      i=0;
      while (1) 
      {
          char receivedChar = uart_read();
          if(i < EEPROM_MAX_SIZE) 
            eeprom_write(i++, receivedChar);    
          if (receivedChar == '\0') 
          {        
              i = 0;
              break;
          }
      }

      _delay_ms(3000);//to settle things down

      i=0;
      while(1)
      {
          char temp=eeprom_read(i++);
          uart_send(temp);
          if(temp == '\0')
            break;
      }
      
    }
    return 0;
}
