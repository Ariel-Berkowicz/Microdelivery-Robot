# get data from gpsd from a serial port and print it

import gps
import time
import serial
import sys

port = serial.Serial("/dev/serial0", baudrate=9600, timeout=10.0)

line = []
print("connected to: " + port.portstr)


def remove_all_other_letters(rsv):
    # go through the alphabet
    alphabet = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z']

    for letter in alphabet:
        # if the letter is N or S or E or W then keep it
        if letter == 'N' or letter == 'S' or letter == 'E' or letter == 'W':
            continue
        # if the letter is not N or S or E or W then remove it
        else:
            rsv = rsv.replace(letter, '')
    return rsv

while True:
    try:
        rcv = port.readline()
        if (('N' in rcv.decode('utf-8')[8:])
                or ('E' in rcv.decode('utf-8')[8:])
                or ('W' in rcv.decode('utf-8')[8:])
                or ('S' in rcv.decode('utf-8')[8:])):
            
            if remove_all_other_letters(rcv.decode('utf-8')[18:]).replace(',','')[:21].replace('.','').replace('-','').replace('*','').isdigit():
                continue
            else:
                rcv2 = remove_all_other_letters(rcv.decode('utf-8')[18:]).replace(',','')[:21]
                print(rcv2[:2]+" "+ rcv2[2:10] +" "+ rcv2[10:13]+" "+rcv2[13:])

    except KeyboardInterrupt:
        print("\nExiting...")
        sys.exit(0)
    except StopIteration:
        print("GPSD has terminated")
        sys.exit(0)
    except Exception:
        print("Error:", sys.exc_info()[0])
        sys.exit(0)