// uHandbit waving hand (micro:bit V2 + StartbitV2 extension)
// Based on https://github.com/cloverdale-robotics-learning/waving-hand
// Button A: start waving. Button B: stop.
// The servos only move with the 6V adapter plugged into the barrel jack.

let waving = false

StartbitV2.startbit_Init()

// Hold the thumb (servo 2) and fingers (servos 3-6) in place
StartbitV2.setPwmServo(StartbitV2.startbit_servorange.range1, 2, 0, 500)
StartbitV2.setPwmServo(StartbitV2.startbit_servorange.range1, 3, 130, 500)
StartbitV2.setPwmServo(StartbitV2.startbit_servorange.range1, 4, 130, 500)
StartbitV2.setPwmServo(StartbitV2.startbit_servorange.range1, 5, 130, 500)
StartbitV2.setPwmServo(StartbitV2.startbit_servorange.range1, 6, 130, 500)
basic.showIcon(IconNames.Happy)

input.onButtonPressed(Button.A, function () {
    waving = true
    basic.showArrow(ArrowNames.East)
})

input.onButtonPressed(Button.B, function () {
    waving = false
    basic.showIcon(IconNames.Happy)
})

// Swing the wrist (servo 1) back and forth while waving is on
basic.forever(function () {
    if (waving) {
        StartbitV2.setPwmServo(StartbitV2.startbit_servorange.range1, 1, 0, 500)
        basic.pause(1000)
        StartbitV2.setPwmServo(StartbitV2.startbit_servorange.range1, 1, 180, 500)
        basic.pause(1000)
    } else {
        basic.pause(100)
    }
})
