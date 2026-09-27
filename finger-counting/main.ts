// uHandbit finger counting (micro:bit V2 + StartbitV2 extension)
// Starts as a fist showing 0.
// Button A: raise one more finger (count up to 5).
// Button B: lower the last finger raised (count down to 0).
// Angles confirmed on this hand:
//   fingers (servos 3-6): 0 = closed, 130 = open
//   thumb (servo 2):    140 = closed,   0 = open
// The servos only move with the 6V adapter plugged into the barrel jack.

const MOVE_MS = 400

// Counting order: index, middle, ring, little finger, then thumb
const SERVOS = [3, 4, 5, 6, 2]
const OPEN = [130, 130, 130, 130, 0]
const CLOSED = [0, 0, 0, 0, 140]

let count = 0

function setFinger(position: number, open: boolean) {
    const angle = open ? OPEN[position] : CLOSED[position]
    StartbitV2.setPwmServo(StartbitV2.startbit_servorange.range1, SERVOS[position], angle, MOVE_MS)
}

// Start as a fist: fingers first, then the thumb over them
StartbitV2.startbit_Init()
for (let i = 0; i < 4; i++) {
    setFinger(i, false)
}
basic.pause(MOVE_MS + 200)
setFinger(4, false)
basic.showNumber(count)

input.onButtonPressed(Button.A, function () {
    if (count < 5) {
        setFinger(count, true)
        count += 1
        basic.showNumber(count)
    } else {
        // Already showing 5
        basic.showIcon(IconNames.No)
        basic.pause(500)
        basic.showNumber(count)
    }
})

input.onButtonPressed(Button.B, function () {
    if (count > 0) {
        count -= 1
        setFinger(count, false)
        basic.showNumber(count)
    } else {
        // Already a fist
        basic.showIcon(IconNames.No)
        basic.pause(500)
        basic.showNumber(count)
    }
})
