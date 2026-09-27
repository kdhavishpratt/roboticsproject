// uHandbit fist and open hand (micro:bit V2 + StartbitV2 extension)
// Button A: make a fist. Button B: open the hand.
// Positions from the community uHandbit projects:
//   fingers (servos 3-6): 0 = closed, 130 = open
//   thumb (servo 2):    140 = closed,   0 = open
// The servos only move with the 6V adapter plugged into the barrel jack.

const FINGER_CLOSED = 0
const FINGER_OPEN = 130
const THUMB_CLOSED = 140
const THUMB_OPEN = 0
const MOVE_MS = 500

function moveFingers(angle: number) {
    for (let servo = 3; servo <= 6; servo++) {
        StartbitV2.setPwmServo(StartbitV2.startbit_servorange.range1, servo, angle, MOVE_MS)
    }
}

function moveThumb(angle: number) {
    StartbitV2.setPwmServo(StartbitV2.startbit_servorange.range1, 2, angle, MOVE_MS)
}

// Fist: fold the fingers first, then wrap the thumb over them
function makeFist() {
    basic.showIcon(IconNames.SmallSquare)
    moveFingers(FINGER_CLOSED)
    basic.pause(MOVE_MS + 200)
    moveThumb(THUMB_CLOSED)
}

// Open: move the thumb out of the way first, then straighten the fingers
function openHand() {
    basic.showIcon(IconNames.Square)
    moveThumb(THUMB_OPEN)
    basic.pause(MOVE_MS + 200)
    moveFingers(FINGER_OPEN)
}

StartbitV2.startbit_Init()
openHand()

input.onButtonPressed(Button.A, function () {
    makeFist()
})

input.onButtonPressed(Button.B, function () {
    openHand()
})
