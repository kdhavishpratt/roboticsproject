# robotics-hiwonder

Programs for a Hiwonder **uHandbit** robotic hand (micro:bit V2 + Hiwonder expansion board) and an
**ESP32 DevKit V1** phone remote.

Step-by-step guide with photos: https://claude.ai/artifact/RWXzkgmmDWiwHpDqywjX2H
(source in [`docs/guide`](docs/guide)).

## micro:bit projects

| Folder | What it does | Status |
|---|---|---|
| [`waving-hand`](waving-hand) | A waves the wrist, B stops | Working |
| [`fist-open`](fist-open) | A makes a fist, B opens the hand | Working |
| [`finger-counting`](finger-counting) | A counts up 1-5 on the fingers, B counts down | Working |
| [`rock-paper-scissors`](rock-paper-scissors) | A: 3-2-1 countdown, then rock, paper or scissors | Loaded, not reported |
| [`clap-to-grab`](clap-to-grab) | Clap to close or open; A/B change sensitivity | Working |
| [`hand-bluetooth`](hand-bluetooth) | Moves the hand from Bluetooth commands (for the ESP32 remote) | Working |
| [`distance-meter`](distance-meter) | Plain ultrasonic sensor on socket 1 or 2, distance in cm | No echo yet (sensor likely damaged) |
| [`glowing-distance-meter`](glowing-distance-meter) | Hiwonder glowing ultrasonic on socket 4, color-changing eyes | Not tested |
| [`i2c-scan`](i2c-scan) | Lists I2C devices (troubleshooting) | Working |

Build and load (in a project folder, with the micro:bit plugged in):

    npx makecode build
    cp -X built/binary.hex /Volumes/MICROBIT/

## ESP32 projects

| Folder | What it does | Status |
|---|---|---|
| [`esp32-hello`](esp32-hello) | Blink and "hello" test | Working |
| [`esp32-hand-remote`](esp32-hand-remote) | Phone page on Wi-Fi `uHandbit-Remote`, Bluetooth to `hand-bluetooth` | Working |
| [`esp32-lsc-test`](esp32-lsc-test) | Option A: talk to the board's servo controller over serial | Not tested |
| [`esp32-direct-servos`](esp32-direct-servos) | Option B: drive the servos with the ESP32's own PWM | Not tested |

Build and upload (board appears as `/dev/cu.usbserial-0001`):

    arduino-cli compile --fqbn esp32:esp32:esp32:PartitionScheme=huge_app esp32-hand-remote
    arduino-cli upload  --fqbn esp32:esp32:esp32:PartitionScheme=huge_app -p /dev/cu.usbserial-0001 esp32-hand-remote

Other ESP32 sketches use `--fqbn esp32:esp32:esp32`.

## Hardware notes

- The servos only move with the 6V adapter in the board's barrel jack; USB powers only the micro:bit.
- Servo map: 1 wrist, 2 thumb, 3-6 index to little finger. Fingers 0 = closed, 130 = open; thumb 140 = closed, 0 = open.
- Board sockets: 1 = P1/P2, 2 = P13/P14, 3 = analog, 4 = I2C (the only one), 5 = helper chip serial,
  6 = bus servos with servo voltage on pin 2. Never plug a sensor into socket 6.
