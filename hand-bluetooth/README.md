# hand-bluetooth

uHandbit (micro:bit V2) program that moves the hand from commands sent over Bluetooth by the ESP32 remote
(`../esp32-hand-remote`). No pairing needed. Check mark on the LED grid = remote connected, X = disconnected.

Commands (one per line): `F` fist, `O` open, `W` wave, `X` stop waving, `C n` count n fingers, `S s a` set servo s to angle a.

## Build and load

    npx makecode build
    cp -X built/binary.hex /Volumes/MICROBIT/

The servos need the 6V adapter plugged into the barrel jack; USB only powers the micro:bit.
