# esp32-direct-servos (option B, NOT TESTED)

ESP32 DevKit V1 drives the uHandbit's six servos directly with 50 Hz PWM, without the micro:bit or the Hiwonder board.
Same phone page as the Bluetooth remote, on Wi-Fi `uHandbit-Direct` (password `handbit123`), http://192.168.4.1

Servo signal pins: 1 wrist GPIO13, 2 thumb GPIO14, 3 index GPIO27, 4 middle GPIO26, 5 ring GPIO25, 6 little GPIO33.

Wiring rules:
- Servo red wires go to a separate 6V supply rated 5 A or more. Never to the ESP32's pins.
- Servo brown wires and the ESP32's GND all connect to the supply's GND (common ground).
- Servo orange (signal) wires go to the GPIO pins above.

    arduino-cli compile --fqbn esp32:esp32:esp32 esp32-direct-servos
    arduino-cli upload  --fqbn esp32:esp32:esp32 -p /dev/cu.usbserial-0001 esp32-direct-servos
