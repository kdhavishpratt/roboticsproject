# rock-paper-scissors

uHandbit (micro:bit V2) program: press A and the hand counts down 3, 2, 1,
then shows a random rock (fist), paper (open hand) or scissors (index and middle fingers up).
The LED grid shows the pick: small square = rock, big square = paper, scissors icon = scissors.

## Build and load

    npx makecode build
    cp -X built/binary.hex /Volumes/MICROBIT/

Or open https://makecode.microbit.org, choose Import > Import File, and pick `built/binary.hex`.

The servos need the 6V adapter plugged into the barrel jack; USB only powers the micro:bit.
