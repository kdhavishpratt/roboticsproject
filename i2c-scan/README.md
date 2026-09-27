# i2c-scan

uHandbit (micro:bit V2) troubleshooting program: lists which I2C devices answer, every 3 seconds.
Hiwonder's glowing ultrasonic sensor should answer at 0x77.
A check mark means the sensor was found, a number is some other device, an X means nothing answered.
Results also go over USB as `scan: ...` lines (115200 baud). This program doesn't move the hand.

## Build and load

    npx makecode build
    cp -X built/binary.hex /Volumes/MICROBIT/
