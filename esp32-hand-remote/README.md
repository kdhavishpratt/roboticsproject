# esp32-hand-remote

ESP32 DevKit V1 program: control the uHandbit from your phone.

    Phone --Wi-Fi--> ESP32 --Bluetooth--> micro:bit (hand-bluetooth) --> servos

1. Load `../hand-bluetooth` onto the micro:bit.
2. Build and upload this sketch to the ESP32:

       arduino-cli compile --fqbn esp32:esp32:esp32:PartitionScheme=huge_app esp32-hand-remote
       arduino-cli upload  --fqbn esp32:esp32:esp32:PartitionScheme=huge_app -p /dev/cu.usbserial-XXXX esp32-hand-remote

3. On your phone, join Wi-Fi `uHandbit-Remote` (password `handbit123`) and open http://192.168.4.1
4. The ESP32's blue LED turns on and the micro:bit shows a check mark when they're connected.

The ESP32 only needs USB power (Mac, charger or power bank). Its pins aren't used.
The hand's servos still need the 6V adapter in the barrel jack.
