# finger-counting

uHandbit (micro:bit V2) program: counts from 0 to 5 on the fingers.
Starts as a fist showing 0. Press A to raise one more finger, B to lower the last one.
Order: index, middle, ring, little finger, thumb.

## Build and load

    npx makecode build
    cp -X built/binary.hex /Volumes/MICROBIT/

Or open https://makecode.microbit.org, choose Import > Import File, and pick `built/binary.hex`.

The servos need the 6V adapter plugged into the barrel jack; USB only powers the micro:bit.
