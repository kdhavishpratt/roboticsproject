// ESP32 DevKit V1 first test: blink the blue LED and say hello over USB.
// The blue LED on DevKit V1 boards is on GPIO 2.

const int LED_PIN = 2;
int count = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  delay(500);
  Serial.println("hello from the ESP32");
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
  delay(500);
  count++;
  Serial.print("hello ");
  Serial.println(count);
}
