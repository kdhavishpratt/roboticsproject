# glowing-distance-meter

uHandbit (micro:bit V2) program for Hiwonder's glowing ultrasonic sensor (the one with lights around its eyes).
Plug the sensor into socket 4, 5 or 6 (I2C). It shows the distance in cm and colors the eyes:
green = far, yellow = 10-30 cm, red = under 10 cm, blue = nothing in range.
This program doesn't move the hand.

Readings also go over USB as `raw:230 cm:23` lines (115200 baud).

## Build and load

    npx makecode build
    cp -X built/binary.hex /Volumes/MICROBIT/
