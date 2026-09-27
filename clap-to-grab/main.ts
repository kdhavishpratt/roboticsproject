// uHandbit clap to grab (micro:bit V2 + StartbitV2 extension)
// Clap once: the hand closes into a fist. Clap again: it opens.
// Button A: less sensitive (needs a louder clap).
// Button B: more sensitive (a quieter clap works).
// Angles confirmed on this hand:
//   fingers (servos 3-6): 0 = closed, 130 = open
//   thumb (servo 2):    140 = closed,   0 = open
// The servos only move with the 6V adapter plugged into the barrel jack.

const FINGER_CLOSED = 0
const FINGER_OPEN = 130
const THUMB_CLOSED = 140
const THUMB_OPEN = 0
const MOVE_MS = 400

// Ignore sounds for this long after a move, so the motors' own noise
// doesn't count as a clap
const QUIET_MS = 1000

// Sensitivity level 1 (quiet claps work) to 9 (needs a very loud clap)
let level = 5
let closed = false
let busy = false

function thresholdFor(lvl: number) {
    // Level 1 = 80, level 9 = 240 on the micro:bit's 0-255 sound scale
    return 60 + lvl * 20
}

function move(servo: number, angle: number) {
    StartbitV2.setPwmServo(StartbitV2.startbit_servorange.range1, servo, angle, MOVE_MS)
}

// Fist: fingers first, then the thumb over them
function makeFist() {
    for (let servo = 3; servo <= 6; servo++) {
        move(servo, FINGER_CLOSED)
    }
    basic.pause(MOVE_MS + 200)
    move(2, THUMB_CLOSED)
    basic.showIcon(IconNames.SmallSquare)
}

// Open: thumb out of the way first, then the fingers
function openHand() {
    move(2, THUMB_OPEN)
    basic.pause(MOVE_MS + 200)
    for (let servo = 3; servo <= 6; servo++) {
        move(servo, FINGER_OPEN)
    }
    basic.showIcon(IconNames.Square)
}

function showLevel() {
    basic.showNumber(level)
    basic.pause(300)
    basic.showIcon(closed ? IconNames.SmallSquare : IconNames.Square)
}

StartbitV2.startbit_Init()
input.setSoundThreshold(SoundThreshold.Loud, thresholdFor(level))
openHand()

input.onSound(DetectedSound.Loud, function () {
    if (busy) {
        return
    }
    busy = true
    if (closed) {
        openHand()
    } else {
        makeFist()
    }
    closed = !closed
    basic.pause(QUIET_MS)
    busy = false
})

input.onButtonPressed(Button.A, function () {
    if (level < 9) {
        level += 1
    }
    input.setSoundThreshold(SoundThreshold.Loud, thresholdFor(level))
    showLevel()
})

input.onButtonPressed(Button.B, function () {
    if (level > 1) {
        level -= 1
    }
    input.setSoundThreshold(SoundThreshold.Loud, thresholdFor(level))
    showLevel()
})
