// uHandbit distance meter (micro:bit V2, plain ultrasonic sensor)
// Finds which port the sensor is on (Port 1 or Port 2), shows "P1" or "P2",
// then shows the distance in cm on the LED grid.
// Readings are also sent over USB as lines like "cm:23" for checking on a computer.
// This program doesn't move the hand.

// Startbit ports: Port 1 = trig P1 / echo P2, Port 2 = trig P13 / echo P14
const TRIG = [DigitalPin.P1, DigitalPin.P13]
const ECHO = [DigitalPin.P2, DigitalPin.P14]

let port = -1

// One measurement in cm, or 0 if no echo came back
function measure(p: number): number {
    pins.setPull(TRIG[p], PinPullMode.PullNone)
    pins.setPull(ECHO[p], PinPullMode.PullNone)
    pins.digitalWritePin(TRIG[p], 0)
    control.waitMicros(2)
    pins.digitalWritePin(TRIG[p], 1)
    control.waitMicros(10)
    pins.digitalWritePin(TRIG[p], 0)
    // Sound travels about 1 cm there and back every 58 microseconds
    const echoMicros = pins.pulseIn(ECHO[p], PulseValue.High, 25000)
    return Math.round(echoMicros / 58)
}

// Try both ports until one of them answers
basic.showString("?")
while (port < 0) {
    for (let p = 0; p < 2; p++) {
        if (measure(p) > 0) {
            port = p
        }
        basic.pause(60)
    }
    if (port < 0) {
        serial.writeLine("no sensor found on Port 1 or Port 2")
    }
}
serial.writeLine("sensor on Port " + (port + 1))
basic.showString("P" + (port + 1))

basic.forever(function () {
    const cm = measure(port)
    serial.writeLine("cm:" + cm)
    if (cm == 0) {
        // Nothing in range (or pointing at something soft or angled)
        basic.showIcon(IconNames.No)
    } else {
        basic.showNumber(cm)
    }
    basic.pause(200)
})
