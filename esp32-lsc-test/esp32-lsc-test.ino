// ESP32 DevKit V1: talk to the uHandbit board's servo controller directly (option A test).
//
// Step 1 (listen only): prints any messages the board's helper chip sends, such as its
//   battery report "A<p14><sound><volt>$", to the Mac over USB.
// Step 2 (only when you press the BOOT button): sends one servo command that toggles
//   the index finger (servo 3) between open (130) and closed (0).
//
// Uses the ESP32's second serial port: RX2 = GPIO 16, TX2 = GPIO 17, 115200 baud.
// Only connect these after the board's serial socket has been verified.

const int RX2_PIN = 16;
const int TX2_PIN = 17;
const int BOOT_BUTTON = 0;
const int LED_PIN = 2;

bool fingerClosed = false;
String incoming = "";

// Hiwonder servo controller frame: 55 55 08 03 01 timeL timeH id posL posH
// Position 500-2500 covers 0-180 degrees, the same as the micro:bit's StartbitV2 extension.
void moveServo(uint8_t id, int angle, uint16_t ms) {
  angle = constrain(angle, 0, 180);
  uint16_t pos = 500 + (uint32_t)angle * 2000 / 180;
  uint8_t frame[10] = {0x55, 0x55, 0x08, 0x03, 0x01,
                       (uint8_t)(ms & 0xff), (uint8_t)(ms >> 8),
                       id, (uint8_t)(pos & 0xff), (uint8_t)(pos >> 8)};
  Serial2.write(frame, sizeof(frame));
  Serial.printf("sent servo %d -> %d degrees (pos %d)\n", id, angle, pos);
}

// "A" frames carry three 2-digit hex values; the last is battery voltage in units of 78.63 mV
void reportFrame(const String &frame) {
  Serial.print("from board: ");
  Serial.println(frame);
  if (frame.length() == 7 && frame.charAt(0) == 'A') {
    long volt = strtol(frame.substring(5, 7).c_str(), nullptr, 16);
    Serial.printf("  battery about %.2f V\n", volt * 78.63 / 1000.0);
  }
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RX2_PIN, TX2_PIN);
  pinMode(BOOT_BUTTON, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  Serial.println("listening to the board; press BOOT to toggle the index finger");
}

void loop() {
  while (Serial2.available()) {
    char c = (char)Serial2.read();
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));  // blink on every byte received
    if (c == '$') {
      reportFrame(incoming);
      incoming = "";
    } else if (incoming.length() < 40) {
      incoming += c;
    }
  }

  if (digitalRead(BOOT_BUTTON) == LOW) {
    fingerClosed = !fingerClosed;
    moveServo(3, fingerClosed ? 0 : 130, 500);
    delay(400);  // ignore the rest of this press
    while (digitalRead(BOOT_BUTTON) == LOW) {
      delay(10);
    }
  }
}
