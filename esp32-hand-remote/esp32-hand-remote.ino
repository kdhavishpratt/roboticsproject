// uHandbit phone remote (ESP32 DevKit V1)
//
// The ESP32 makes its own Wi-Fi network. Join it from your phone and open http://192.168.4.1
// Buttons and sliders on that page are sent over Bluetooth to the micro:bit on the hand,
// which runs the hand-bluetooth program.
//
// Phone --Wi-Fi--> ESP32 --Bluetooth--> micro:bit --> servos

#include <WiFi.h>
#include <WebServer.h>
#include <BLEDevice.h>

// The ESP32's own Wi-Fi network. Change the password if you like (at least 8 characters).
const char *AP_NAME = "uHandbit-Remote";
const char *AP_PASSWORD = "handbit123";

// micro:bit Bluetooth UART service. The ESP32 writes commands to the RX characteristic.
static BLEUUID UART_SERVICE("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
static BLEUUID UART_RX("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");

const int LED_PIN = 2;  // Blue LED: on when connected to the micro:bit

WebServer server(80);
BLEClient *client = nullptr;
BLERemoteCharacteristic *rx = nullptr;
BLEAdvertisedDevice *found = nullptr;
bool connected = false;
String microbitName = "";
unsigned long lastScan = 0;

class ClientEvents : public BLEClientCallbacks {
  void onConnect(BLEClient *c) override {}
  void onDisconnect(BLEClient *c) override {
    connected = false;
    rx = nullptr;
    digitalWrite(LED_PIN, LOW);
    Serial.println("micro:bit disconnected");
  }
};

// Look for a nearby micro:bit for 3 seconds
void scanForMicrobit() {
  Serial.println("scanning for a micro:bit...");
  BLEScan *scan = BLEDevice::getScan();
  scan->setActiveScan(true);
  BLEScanResults *results = scan->start(3, false);
  for (int i = 0; i < results->getCount(); i++) {
    BLEAdvertisedDevice device = results->getDevice(i);
    if (device.haveName() && device.getName().startsWith("BBC micro:bit")) {
      delete found;
      found = new BLEAdvertisedDevice(device);
      Serial.print("found ");
      Serial.println(device.getName());
      break;
    }
  }
  scan->clearResults();
}

bool connectToMicrobit() {
  if (found == nullptr) {
    return false;
  }
  if (client == nullptr) {
    client = BLEDevice::createClient();
    client->setClientCallbacks(new ClientEvents());
  }
  if (!client->connect(found)) {
    Serial.println("could not connect");
    return false;
  }
  BLERemoteService *service = client->getService(UART_SERVICE);
  if (service == nullptr) {
    Serial.println("no UART service: is hand-bluetooth loaded on the micro:bit?");
    client->disconnect();
    return false;
  }
  rx = service->getCharacteristic(UART_RX);
  if (rx == nullptr) {
    client->disconnect();
    return false;
  }
  microbitName = found->getName();
  connected = true;
  digitalWrite(LED_PIN, HIGH);
  Serial.print("connected to ");
  Serial.println(microbitName);
  return true;
}

// Send one command line to the micro:bit
bool sendCommand(const String &cmd) {
  if (!connected || rx == nullptr) {
    return false;
  }
  String line = cmd + "\n";
  rx->writeValue((uint8_t *)line.c_str(), line.length(), true);
  Serial.print("sent ");
  Serial.print(cmd);
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
<div id="status">Looking for the hand...</div>
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
  // Only pass on the commands the micro:bit understands
  if (c.length() == 0 || c.length() > 12 || String("FOWXCS").indexOf(c.charAt(0)) < 0) {
    server.send(400, "text/plain", "bad command");
    return;
  }
  server.send(200, "text/plain", sendCommand(c) ? "ok" : "not connected");
}

void handleStatus() {
  String json = String("{\"connected\":") + (connected ? "true" : "false") +
                ",\"name\":\"" + microbitName + "\"}";
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  WiFi.softAP(AP_NAME, AP_PASSWORD);
  Serial.print("Wi-Fi network ");
  Serial.print(AP_NAME);
  Serial.print(" ready. Open http://");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/cmd", handleCmd);
  server.on("/status", handleStatus);
  server.begin();

  BLEDevice::init("uHandbit-Remote");
}

void loop() {
  server.handleClient();
  // While not connected, look for the micro:bit every 5 seconds
  if (!connected && millis() - lastScan > 5000) {
    scanForMicrobit();
    connectToMicrobit();
    lastScan = millis();
  }
  delay(2);
}
