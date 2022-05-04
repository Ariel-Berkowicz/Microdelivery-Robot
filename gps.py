# get data from gpsd from a serial port and print it


import gps
import time
import serial
import sys
import os
import datetime
import math


# open serial port
ser = serial.Serial(
    port='/dev/ttyUSB0',
    baudrate=9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1
)


# open gpsd
session = gps.gps("localhost", "2947")
session.stream(gps.WATCH_ENABLE | gps.WATCH_NEWSTYLE)


# get data from gpsd
while True:
    try:
        report = session.next()
        if report['class'] == 'TPV':
            if hasattr(report, 'time'):
                print(report.time)
            if hasattr(report, 'lat'):
                print(report.lat)
            if hasattr(report, 'lon'):
                print(report.lon)
            if hasattr(report, 'alt'):
                print(report.alt)
            if hasattr(report, 'speed'):
                print(report.speed)
            if hasattr(report, 'track'):
                print(report.track)
            if hasattr(report, 'climb'):
                print(report.climb)
            if hasattr(report, 'epx'):
                print(report.epx)
            if hasattr(report, 'epy'):
                print(report.epy)
            if hasattr(report, 'epv'):
                print(report.epv)
            if hasattr(report, 'ept'):
                print(report.ept)
            if hasattr(report, 'eps'):
                print(report.eps)
            if hasattr(report, 'epc'):
                print(report.epc)
            if hasattr(report, 'epd'):
                print(report.epd)
            if hasattr(report, 'epu'):
                print(report.epu)
            if hasattr(report, 'epv'):
                print(report.epv)
            if hasattr(report, 'ept'):
                print(report.ept)
            if hasattr(report, 'eps'):
                print(report.eps)
            if hasattr(report, 'epc'):
                print(report.epc)
            if hasattr(report, 'epd'):
                print(report.epd)
            if hasattr(report, 'epu'):
                print(report.epu)
            if hasattr(report, 'epv'):
                print(report.epv)
            if hasattr(report, 'ept'):
                print(report.ept)

    except KeyError:
        pass
    except KeyboardInterrupt:
        quit()
    except StopIteration:
        session = None
        print("GPSD has terminated")
    time.sleep(0.1)


# close serial port
ser.close()


# close gpsd
session = None
print("GPSD has terminated")


# end of file
sys.exit(0)


