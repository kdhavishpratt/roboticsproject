# fist-open

uHandbit (micro:bit V2) program: press A to make a fist, B to open the hand.
The hand opens by itself when the program starts.

## Build and load

    npx makecode build
    cp -X built/binary.hex /Volumes/MICROBIT/

Or open https://makecode.microbit.org, choose Import > Import File, and pick `built/binary.hex`.

The servos need the 6V adapter plugged into the barrel jack; USB only powers the micro:bit.
