# clap-to-grab

uHandbit (micro:bit V2) program: clap to close the hand into a fist, clap again to open it.
Sounds are ignored for 1 second after each move so the motor noise doesn't count as a clap.

- Button A: less sensitive (needs a louder clap)
- Button B: more sensitive
- The display briefly shows the level (1-9, starts at 5), then big square = open, small square = fist.

## Build and load

    npx makecode build
    cp -X built/binary.hex /Volumes/MICROBIT/

The servos need the 6V adapter plugged into the barrel jack; USB only powers the micro:bit.
