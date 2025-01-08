import serial
import time
import sys


def print_speed(x,y,text):#some ansi tweeks to display speed at top of console
    ESC="\033"
    sys.stdout.write(f"{ESC}[s")
    sys.stdout.flush()
    print(f"\033[{x};{y}H{text}")
    sys.stdout.write(f"{ESC}[u")
    sys.stdout.flush()


PORT = "COM4"
BAUD_RATE = 2400
message =("Finance Minister Arun Jaitley Tuesday hit out at former RBI governor Raghuram Rajan for predicting\nthat the next banking crisis would be triggered by MSME lending, saying postmortem is easier than\ntaking action when it was required.Rajan, who had as the chief economist at IMF warned \nof impending financial crisis of 2008, in a note to a parliamentary committee warned against ambitious credit targets and loan waivers, \nsaying that they could be the sources of next banking crisis. Government should focus on sources of the next crisis, not just the last one.\n"
          "In particular, government should refrain from setting ambitious credit targets or waiving loans.\nCredit targets are sometimes achieved by abandoning appropriate due diligence, creating the environment for future NPAs,\n\" Rajan said in the note.\" Both MUDRA loans as well as the Kisan Credit Card, while popular, have to be examined more \nclosely for potential credit risk. Rajan, who was RBI governor for three years till September 2016, is currently.\0")

speed="0.00 bits/sec"



# Initialize UART connection
try:
    connection = serial.Serial(PORT, BAUD_RATE, timeout=None)
    print("UART connection established.")
except Exception as e:
    print(f"Failed to establish UART connection: {e}")
    sys.exit(1)

time.sleep(3)# to settle things down
print("SPEED OF TRANSMISSION")
print(speed)
print("\nTRANSMITTING")

for char in message:
    start = time.perf_counter()
    connection.write(char.encode())

    time.sleep(0.0)#a considerable delay if needed accounting for the time of writing to eeprom so that data is not missed in between
    elapsed = time.perf_counter() - start

    print(char, end="", flush=True)
    speed = f"{8/elapsed:.2f} bits/sec  "
    print_speed(4, 1, speed)
print("\nTransmission completed.\n")

# Receive data
print("RECIEVING")
while True:

    # Read one character from the UART
        start = time.perf_counter()
        char = connection.read(1).decode()
        elapsed = time.perf_counter() - start

        print(char, end="", flush=True)
        speed = f"{8/elapsed:.2f} bits/sec  "
        print_speed(4, 1, speed)

        if char == '\0': 
            break


print("\n\nEND OF PROGRAM")

connection.close()