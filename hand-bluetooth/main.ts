// uHandbit Bluetooth receiver (micro:bit V2 + StartbitV2 extension)
// Listens on the Bluetooth UART service for one command per line, sent by the ESP32 remote:
//   F        make a fist
//   O        open the hand
//   W        start waving (the wrist swings)
//   X        stop waving
//   C n      count n fingers (0-5)
//   S s a    set servo s (1-6) to angle a
// Angles confirmed on this hand:
//   fingers (servos 3-6): 0 = closed, 130 = open
//   thumb (servo 2):    140 = closed,   0 = open
// The servos only move with the 6V adapter plugged into the barrel jack.

const FINGER_CLOSED = 0
const FINGER_OPEN = 130
const THUMB_CLOSED = 140
const THUMB_OPEN = 0
const MOVE_MS = 400

// Counting order: index, middle, ring, little finger, then thumb
const COUNT_SERVOS = [3, 4, 5, 6, 2]
const COUNT_OPEN = [130, 130, 130, 130, 0]
const COUNT_CLOSED = [0, 0, 0, 0, 140]

let waving = false

function move(servo: number, angle: number) {
    StartbitV2.setPwmServo(StartbitV2.startbit_servorange.range1, servo, angle, MOVE_MS)
}

function makeFist() {
    for (let s = 3; s <= 6; s++) {
        move(s, FINGER_CLOSED)
    }
    basic.pause(MOVE_MS + 200)
    move(2, THUMB_CLOSED)
}

function openHand() {
    move(2, THUMB_OPEN)
    basic.pause(MOVE_MS + 200)
    for (let s = 3; s <= 6; s++) {
        move(s, FINGER_OPEN)
    }
}

function showCount(n: number) {
    n = Math.max(0, Math.min(5, n))
    for (let i = 0; i < 5; i++) {
        move(COUNT_SERVOS[i], i < n ? COUNT_OPEN[i] : COUNT_CLOSED[i])
    }
    basic.showNumber(n)
}

function handle(line: string) {
    const parts = line.trim().split(" ")
    const cmd = parts[0]
    if (cmd == "F") {
        waving = false
        makeFist()
        basic.showIcon(IconNames.SmallSquare)
    } else if (cmd == "O") {
        waving = false
        openHand()
        basic.showIcon(IconNames.Square)
    } else if (cmd == "W") {
        waving = true
        basic.showArrow(ArrowNames.East)
    } else if (cmd == "X") {
        waving = false
        basic.showIcon(IconNames.Happy)
    } else if (cmd == "C" && parts.length >= 2) {
        waving = false
        showCount(parseInt(parts[1]))
    } else if (cmd == "S" && parts.length >= 3) {
        const servo = parseInt(parts[1])
        const angle = parseInt(parts[2])
        if (servo >= 1 && servo <= 6 && angle >= 0 && angle <= 180) {
            if (servo == 1) {
                waving = false
            }
            move(servo, angle)
        }
    }
}

StartbitV2.startbit_Init()
bluetooth.startUartService()
openHand()
basic.showIcon(IconNames.Happy)

bluetooth.onBluetoothConnected(function () {
    basic.showIcon(IconNames.Yes)
})

bluetooth.onBluetoothDisconnected(function () {
    waving = false
    basic.showIcon(IconNames.No)
})

bluetooth.onUartDataReceived(serial.delimiters(Delimiters.NewLine), function () {
    handle(bluetooth.uartReadUntil(serial.delimiters(Delimiters.NewLine)))
})

basic.forever(function () {
    if (waving) {
        move(1, 0)
        basic.pause(1000)
        if (waving) {
            move(1, 180)
            basic.pause(1000)
        }
    } else {
        basic.pause(100)
    }
})
