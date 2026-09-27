// uHandbit option B: the ESP32 drives the six servos directly (no micro:bit, no Hiwonder board).
// NOT TESTED ON THE HAND YET. Read the wiring notes on the guide page before connecting anything.
//
// The ESP32 makes its own Wi-Fi network. Join it from your phone and open http://192.168.4.1
// The page is the same as the Bluetooth remote, but here the ESP32 moves the servos itself.
//
// Wiring (servo cable colors):
//   brown (ground) -> servo power supply GND, and ESP32 GND
//   red (power)    -> servo power supply +6V   (NEVER the ESP32's pins)
//   orange (signal)-> ESP32 pin from SERVO_PINS below

#include <WiFi.h>
#include <WebServer.h>

const char *AP_NAME = "uHandbit-Direct";
const char *AP_PASSWORD = "handbit123";

// Servo 1 wrist, 2 thumb, 3 index, 4 middle, 5 ring, 6 little.
// These pins are safe general-purpose outputs on the DevKit V1 (no boot or flash pins).
const int SERVO_PINS[7] = {-1, 13, 14, 27, 26, 25, 33};

// Angles confirmed on this hand: fingers 0 = closed, 130 = open; thumb 140 = closed, 0 = open
const int FINGER_CLOSED = 0, FINGER_OPEN = 130, THUMB_CLOSED = 140, THUMB_OPEN = 0;
const int COUNT_SERVOS[5] = {3, 4, 5, 6, 2};
const int COUNT_OPEN[5] = {130, 130, 130, 130, 0};
const int COUNT_CLOSED[5] = {0, 0, 0, 0, 140};

// Start positions: open hand, wrist in the middle
float current[7] = {0, 90, THUMB_OPEN, FINGER_OPEN, FINGER_OPEN, FINGER_OPEN, FINGER_OPEN};
int target[7] = {0, 90, THUMB_OPEN, FINGER_OPEN, FINGER_OPEN, FINGER_OPEN, FINGER_OPEN};
const float STEP_DEGREES = 3.0;  // per 20 ms, about 150 degrees per second, so moves aren't violent

bool waving = false;
unsigned long lastStep = 0, lastWave = 0;
bool waveLeft = false;

WebServer server(80);

// 50 Hz servo signal with 14-bit resolution: 20 ms period = 16384 steps
void writeServo(int servo, float angle) {
  int us = 500 + (int)(constrain(angle, 0, 180) * 2000 / 180);  // same 500-2500 us range as Hiwonder
  ledcWrite(SERVO_PINS[servo], (uint32_t)us * 16384 / 20000);
}

void setTarget(int servo, int angle) {
  if (servo >= 1 && servo <= 6) {
    target[servo] = constrain(angle, 0, 180);
  }
}

bool handle(const String &line) {
  char cmd = line.charAt(0);
  if (cmd == 'F') {
    waving = false;
    for (int s = 3; s <= 6; s++) setTarget(s, FINGER_CLOSED);
    setTarget(2, THUMB_CLOSED);
  } else if (cmd == 'O') {
    waving = false;
    setTarget(2, THUMB_OPEN);
    for (int s = 3; s <= 6; s++) setTarget(s, FINGER_OPEN);
  } else if (cmd == 'W') {
    waving = true;
  } else if (cmd == 'X') {
    waving = false;
  } else if (cmd == 'C') {
    waving = false;
    int n = constrain(line.substring(2).toInt(), 0, 5);
    for (int i = 0; i < 5; i++) setTarget(COUNT_SERVOS[i], i < n ? COUNT_OPEN[i] : COUNT_CLOSED[i]);
  } else if (cmd == 'S') {
    int space = line.indexOf(' ', 2);
    if (space < 0) return false;
    int servo = line.substring(2, space).toInt();
    if (servo == 1) waving = false;
    setTarget(servo, line.substring(space + 1).toInt());
  } else {
    return false;
  }
  return true;
}

const char PAGE[] PROGMEM = R"HTML(<!doctype html>
<html><head><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>uHandbit Remote</title>
<style>
body{font-family:-apple-system,system-ui,sans-serif;margin:0;padding:16px;background:#111513;color:#e5ece8}
h1{font-size:22px;margin:0 0 4px}
#status{font-size:14px;margin-bottom:16px;color:#9ba9a2}
#status.on{color:#7bcb97}
.grid{display:grid;grid-template-columns:repeat(2,1fr);gap:10px;margin-bottom:18px}
button{font-size:18px;padding:16px;border:0;border-radius:10px;background:#f08a3a;color:#111513;font-weight:600}
button.alt{background:#2e3834;color:#e5ece8}
.count{display:grid;grid-template-columns:repeat(6,1fr);gap:6px;margin-bottom:18px}
.count button{padding:12px 0}
label{display:block;font-size:15px;margin:14px 0 4px}
label span{float:right;color:#9ba9a2}
input[type=range]{width:100%}
h2{font-size:16px;margin:18px 0 8px;color:#9ba9a2;text-transform:uppercase;letter-spacing:.06em}
</style></head><body>
<h1>uHandbit Remote</h1>
<div id="status">Direct servo control</div>
<div class="grid">
<button onclick="send('F')">Fist</button>
<button onclick="send('O')">Open</button>
<button onclick="send('W')">Wave</button>
<button class="alt" onclick="send('X')">Stop waving</button>
</div>
<h2>Count</h2>
<div class="count">
<button class="alt" onclick="send('C 0')">0</button><button class="alt" onclick="send('C 1')">1</button>
<button class="alt" onclick="send('C 2')">2</button><button class="alt" onclick="send('C 3')">3</button>
<button class="alt" onclick="send('C 4')">4</button><button class="alt" onclick="send('C 5')">5</button>
</div>
<h2>Each joint</h2>
<div id="sliders"></div>
<script>
const joints=[[1,'Wrist',0,180,90],[2,'Thumb (140 = closed)',0,140,0],[3,'Index (0 = closed)',0,130,130],
[4,'Middle (0 = closed)',0,130,130],[5,'Ring (0 = closed)',0,130,130],[6,'Little (0 = closed)',0,130,130]];
const box=document.getElementById('sliders');
let timer=null;
for(const [s,name,min,max,val] of joints){
  box.insertAdjacentHTML('beforeend',`<label for="s${s}">${name}<span id="v${s}">${val}</span></label>`+
    `<input type="range" id="s${s}" min="${min}" max="${max}" value="${val}">`);
  const el=document.getElementById('s'+s);
  el.addEventListener('input',()=>{document.getElementById('v'+s).textContent=el.value;
    clearTimeout(timer);timer=setTimeout(()=>send('S '+s+' '+el.value),120);});
}
function send(c){fetch('/cmd?c='+encodeURIComponent(c)).then(r=>r.text()).then(t=>{if(t!=='ok')status(false);});}
function status(on,name){const el=document.getElementById('status');
  el.textContent=on?('Connected to '+name):'Not connected to the hand. Is the micro:bit on and running hand-bluetooth?';
  el.className=on?'on':'';}
function poll(){fetch('/status').then(r=>r.json()).then(j=>status(j.connected,j.name)).catch(()=>{});}
poll();setInterval(poll,2000);
</script></body></html>)HTML";

void handleRoot() {
  server.send_P(200, "text/html", PAGE);
}

void handleCmd() {
  String c = server.arg("c");
  if (c.length() == 0 || c.length() > 12) {
    server.send(400, "text/plain", "bad command");
    return;
  }
  server.send(200, "text/plain", handle(c) ? "ok" : "bad command");
}

void handleStatus() {
  server.send(200, "application/json", "{\"connected\":true,\"name\":\"direct servos (ESP32)\"}");
}

void setup() {
  Serial.begin(115200);
  for (int s = 1; s <= 6; s++) {
    ledcAttach(SERVO_PINS[s], 50, 14);
    writeServo(s, current[s]);
  }
  WiFi.softAP(AP_NAME, AP_PASSWORD);
  Serial.print("Wi-Fi network ");
  Serial.print(AP_NAME);
  Serial.print(" ready. Open http://");
  Serial.println(WiFi.softAPIP());
  server.on("/", handleRoot);
  server.on("/cmd", handleCmd);
  server.on("/status", handleStatus);
  server.begin();
}

void loop() {
  server.handleClient();
  unsigned long now = millis();

  // Wrist wave: swing between 0 and 180 every second
  if (waving && now - lastWave > 1000) {
    waveLeft = !waveLeft;
    target[1] = waveLeft ? 0 : 180;
    lastWave = now;
  }

  // Move every servo a small step toward its target every 20 ms
  if (now - lastStep >= 20) {
    lastStep = now;
    for (int s = 1; s <= 6; s++) {
      float diff = target[s] - current[s];
      if (diff != 0) {
        current[s] += constrain(diff, -STEP_DEGREES, STEP_DEGREES);
        writeServo(s, current[s]);
      }
    }
  }
}
