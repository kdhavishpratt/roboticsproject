// uHandbit rock, paper, scissors (micro:bit V2 + StartbitV2 extension)
// Button A: count down 3, 2, 1, then show a random rock, paper or scissors.
// Angles confirmed on this hand:
//   fingers (servos 3-6): 0 = closed, 130 = open
//   thumb (servo 2):    140 = closed,   0 = open
// The servos only move with the 6V adapter plugged into the barrel jack.

const FINGER_CLOSED = 0
const FINGER_OPEN = 130
const THUMB_CLOSED = 140
const THUMB_OPEN = 0
const MOVE_MS = 400

const THUMB = 2
const INDEX = 3
const MIDDLE = 4
const RING = 5
const LITTLE = 6

let playing = false

function move(servo: number, angle: number) {
    StartbitV2.setPwmServo(StartbitV2.startbit_servorange.range1, servo, angle, MOVE_MS)
}

// Fist: fingers first, then the thumb over them
function rock() {
    move(INDEX, FINGER_CLOSED)
    move(MIDDLE, FINGER_CLOSED)
    move(RING, FINGER_CLOSED)
    move(LITTLE, FINGER_CLOSED)
    basic.pause(MOVE_MS + 200)
    move(THUMB, THUMB_CLOSED)
}

// Open hand: thumb out of the way first, then the fingers
function paper() {
    move(THUMB, THUMB_OPEN)
    basic.pause(MOVE_MS + 200)
    move(INDEX, FINGER_OPEN)
    move(MIDDLE, FINGER_OPEN)
    move(RING, FINGER_OPEN)
    move(LITTLE, FINGER_OPEN)
}

// Scissors: from a fist, raise only the index and middle fingers
function scissors() {
    move(INDEX, FINGER_OPEN)
    move(MIDDLE, FINGER_OPEN)
}

StartbitV2.startbit_Init()
rock()
basic.showIcon(IconNames.Happy)

input.onButtonPressed(Button.A, function () {
    if (playing) {
        return
    }
    playing = true

    // Start every round from a fist, like a real game
    rock()
    basic.showNumber(3)
    basic.pause(500)
    basic.showNumber(2)
    basic.pause(500)
    basic.showNumber(1)
    basic.pause(500)

    const pick = randint(0, 2)
    if (pick == 0) {
        // Rock: already a fist
        basic.showIcon(IconNames.SmallSquare)
    } else if (pick == 1) {
        paper()
        basic.showIcon(IconNames.Square)
    } else {
        scissors()
        basic.showIcon(IconNames.Scissors)
    }

    playing = false
})
