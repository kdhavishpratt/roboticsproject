// uHandbit glowing distance meter (micro:bit V2 + Hiwonder glowing ultrasonic sensor)
// Plug the sensor into socket 4, 5 or 6 (the I2C sockets).
// Shows the distance in cm on the LED grid, and the sensor's eyes change color:
//   green = far (more than 30 cm), yellow = 10-30 cm, red = closer than 10 cm,
//   blue = nothing in range.
// Readings are also sent over USB as lines like "raw:230 cm:23".
// This program doesn't move the hand.

let lastColor = -1

function setEyes(color: number) {
    // Only talk to the sensor when the color changes
    if (color == lastColor) {
        return
    }
    lastColor = color
    if (color == 0) {
        RGBSonar.SETRGB(RGBSonar.RGBMode.rgb, RGBSonar.RGBNum.all, 0, 0, 255)
    } else if (color == 1) {
        RGBSonar.SETRGB(RGBSonar.RGBMode.rgb, RGBSonar.RGBNum.all, 0, 255, 0)
    } else if (color == 2) {
        RGBSonar.SETRGB(RGBSonar.RGBMode.rgb, RGBSonar.RGBNum.all, 255, 160, 0)
    } else {
        RGBSonar.SETRGB(RGBSonar.RGBMode.rgb, RGBSonar.RGBNum.all, 255, 0, 0)
    }
}

basic.showIcon(IconNames.Happy)
basic.pause(500)

basic.forever(function () {
    // The sensor reports millimetres
    const raw = RGBSonar.GETDISTANCE()
    const cm = Math.round(raw / 10)
    serial.writeLine("raw:" + raw + " cm:" + cm)

    if (cm == 0) {
        setEyes(0)
        basic.showIcon(IconNames.No)
    } else {
        if (cm > 30) {
            setEyes(1)
        } else if (cm >= 10) {
            setEyes(2)
        } else {
            setEyes(3)
        }
        basic.showNumber(cm)
    }
    basic.pause(200)
})
