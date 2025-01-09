/*
 * File:   main.c
 * Author: Fahad
 *
 * Created on October 23, 2024, 2:56 PM
 */



// PIC16F876A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


#include <xc.h>
#include <string.h>
#include <stdio.h>

#define _XTAL_FREQ 4000000
#define BASE_ADDR 0x00

uint16_t address=BASE_ADDR;
unsigned int i=0;
//char buff[30];
//char temp[30];


void send(char*data)
{
    for(int i=0;*(data+i)!='\0';i++)
    {
        TXREG=data[i];
        while(TRMT != 1);
    }
}



void flash_write(uint16_t addr,char data)
{

    EEADR = (0xFF & addr);
    EEADRH = ((addr >> 8) & 0xFF);  
    EEDATA = data;
    EEDATH = 0x00;
    GIE=0;
    EECON1bits.EEPGD=0;
    EECON1bits.WREN=1;
    EECON2 = 0x55;  
    EECON2 = 0xAA;
    EECON1bits.WR=1;
    while(EECON1bits.WR == 1);
    EECON1bits.WREN=0;
    GIE=1;
    
}

char flash_read(uint16_t addr)
{
    EEADR = (addr &0xFF);
    EEADRH = ((addr >> 8) & 0xFF);
    GIE=0;
    EECON1bits.EEPGD=0;
    EECON1bits.RD=1;
    while(EECON1bits.RD == 1);
    //__delay_ms(10);
    GIE=1;
    char temp = EEDATA;
    return temp;
    
}

void read()
{

    unsigned int index=0;
    while(1)
    {
        while(RCIF != 1);
        char temp=RCREG;
        flash_write(address+index , temp);

        if(temp == '\0')
            break;
        index++;
    }

}


void main(void) 
{

    TXSTA=0x20;
    RCSTA=0x90;
    SPBRG=25;
    
    while(1)
    {
        read();
        //send(buff);
        i=0;
        while(1)
        {
            char temp=flash_read(address + i);
            TXREG=temp;
            while(TRMT != 1);
            if(temp == '\0')
                break;
            i++;
        }
    
    }
    
}




