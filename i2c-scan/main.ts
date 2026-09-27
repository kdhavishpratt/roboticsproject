// uHandbit I2C scan (micro:bit V2)
// Checks every I2C address and reports which devices answer, over USB and on the LED grid.
// Hiwonder's glowing ultrasonic sensor should answer at 0x77 (119).
// Scans again every 3 seconds, so you can plug and unplug the sensor (with the power off) and rescan.
// This program doesn't move the hand.

basic.forever(function () {
    let found: number[] = []
    const probe = pins.createBuffer(1)
    for (let addr = 8; addr < 120; addr++) {
        if (pins.i2cWriteBuffer(addr, probe) == 0) {
            found.push(addr)
        }
    }
    if (found.length == 0) {
        serial.writeLine("scan: nothing found")
        basic.showIcon(IconNames.No)
    } else {
        let line = "scan:"
        for (const addr of found) {
            line = line + " 0x" + (addr < 16 ? "0" : "") + toHex(addr) + "(" + addr + ")"
        }
        serial.writeLine(line)
        if (found.indexOf(0x77) >= 0) {
            basic.showIcon(IconNames.Yes)
        } else {
            basic.showNumber(found[0])
        }
    }
    basic.pause(3000)
})

function toHex(n: number): string {
    const digits = "0123456789abcdef"
    return digits.charAt(Math.idiv(n, 16)) + digits.charAt(n % 16)
}
