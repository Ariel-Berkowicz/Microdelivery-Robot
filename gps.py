# get data from gpsd from a serial port and print it


import gps
import time
import serial
import sys
import gpsd


# open serial port
ser = serial.Serial(
    port='/dev/serial0',
    baudrate=9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1
)


# open gpsd
session = gps.gps("localhost", "2947")
session.stream(gps.WATCH_ENABLE | gps.WATCH_NEWSTYLE)

def getPositionData ():
    nx = gpsd.next()
    if nx['class'] == 'TPV':
        latitude = getattr(nx, 'lat', "Unknown")
        longitude = getattr(nx, 'lon', "Unknown")
        print("Latitude: %s and Longitude: %s" % latitude, longitude)


while True:
    try:
        # get data from gpsd
        session.next()
        # get data from serial port
        data = ser.readline()
        # print data
        print(data)
        # get position data
        getPositionData()
    except KeyboardInterrupt:
        print("\nExiting...")
        sys.exit(0)
    except StopIteration:
        print("GPSD has terminated")
        sys.exit(0)
    except Exception:
        print("Error:", sys.exc_info()[0])
        sys.exit(0)
    time.sleep(1)