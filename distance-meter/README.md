# distance-meter

uHandbit (micro:bit V2) program for the plain ultrasonic sensor (two silver eyes, no lights).
It finds the sensor on Port 1 or Port 2, shows "P1" or "P2", then shows the distance in cm.
An X means nothing is in range. This program doesn't move the hand.

Readings also go over USB as `cm:23` lines (115200 baud), for example:

    screen /dev/cu.usbmodem* 115200

## Build and load

    npx makecode build
    cp -X built/binary.hex /Volumes/MICROBIT/
