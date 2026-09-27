# waving-hand

uHandbit (micro:bit V2) program: press A to wave the wrist, B to stop.

## Build and load

    npx makecode build
    cp -X built/binary.hex /Volumes/MICROBIT/

Or open https://makecode.microbit.org, choose Import > Import File, and pick `built/binary.hex`.

The servos need the 6V adapter plugged into the barrel jack; USB only powers the micro:bit.
