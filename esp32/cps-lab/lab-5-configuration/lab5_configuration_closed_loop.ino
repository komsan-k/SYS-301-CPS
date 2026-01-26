/*
  Lab 5 (5C) — Configuration Layer
  Closed-Loop Control (Safe Actuation + Manual Override)

  - Subscribes state decisions (DIM/NORMAL/BRIGHT)
  - Subscribes override commands (AUTO/MANUAL)
  - Controls LED (GPIO2) with safety constraints:
      * minimum dwell time between toggles
      * fail-safe if state messages stop arriving

  Library:
    PubSubClient

  IMPORTANT:
  - Control is deterministic. No "AI" makes actuation decisions here.
*/

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

// ====== USER SETTINGS ======
static const char* WIFI_SSID = "YOUR_WIFI_SSID";
static const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";
static const char* MQTT_HOST = "192.168.1.10";
static const uint16_t MQTT_PORT = 1883;

static const char* DEVICE_ID = "esp32-01";

// Topics
static const char* TOPIC_STATE    = "cps/esp32/state";
static const char* TOPIC_OVERRIDE = "cps/esp32/override";
static const char* TOPIC_ACT      = "cps/esp32/actuator";

// Hardware
static const int PIN_LED = 2;

// Modes
enum Mode { AUTO, MANUAL };
Mode mode = AUTO;

// Latest state
enum State { S_DIM, S_NORMAL, S_BRIGHT, S_UNKNOWN };
State lastState = S_UNKNOWN;

static uint32_t lastStateRxMs = 0;

// Safety parameters
static const uint32_t MIN_DWELL_MS = 1500;      // minimum time between LED changes
static const uint32_t STATE_TIMEOUT_MS = 8000;  // fail-safe if no state received

static uint32_t lastLedChangeMs = 0;
static int ledValue = 0; // 0/1

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Wi-Fi connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Wi-Fi connected, IP=");
  Serial.println(WiFi.localIP());
}

void connectMqtt() {
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  while (!mqtt.connected()) {
    Serial.print("MQTT connecting...");
    String clientId = String(DEVICE_ID) + "-" + String((uint32_t)ESP.getEfuseMac(), HEX);
    if (mqtt.connect(clientId.c_str())) {
      Serial.println("connected.");
      mqtt.subscribe(TOPIC_STATE);
      mqtt.subscribe(TOPIC_OVERRIDE);
    } else {
      Serial.printf("failed rc=%d. retry in 1s\n", mqtt.state());
      delay(1000);
    }
  }
}

State parseState(const char* payload) {
  if (strstr(payload, "DIM")) return S_DIM;
  if (strstr(payload, "BRIGHT")) return S_BRIGHT;
  if (strstr(payload, "NORMAL")) return S_NORMAL;
  return S_UNKNOWN;
}

Mode parseMode(const char* payload) {
  if (strstr(payload, "MANUAL")) return MANUAL;
  return AUTO;
}

int parseLedValue(const char* payload) {
  const char* p = strstr(payload, "\"led\"");
  if (!p) return -1;
  p = strchr(p, ':');
  if (!p) return -1;
  return atoi(p + 1);
}

const char* stateStr(State s) {
  switch (s) {
    case S_DIM: return "DIM";
    case S_NORMAL: return "NORMAL";
    case S_BRIGHT: return "BRIGHT";
    default: return "UNKNOWN";
  }
}

const char* modeStr(Mode m) {
  return (m == MANUAL) ? "MANUAL" : "AUTO";
}

bool canChangeLed() {
  return (millis() - lastLedChangeMs) >= MIN_DWELL_MS;
}

void publishAct(const char* reason) {
  char msg[240];
  snprintf(msg, sizeof(msg),
           "{\"device\":\"%s\",\"ts\":%lu,\"mode\":\"%s\",\"led\":%d,\"reason\":\"%s\"}",
           DEVICE_ID, (unsigned long)millis(), modeStr(mode), ledValue, reason);
  mqtt.publish(TOPIC_ACT, msg);
  Serial.printf("ACT TX %s\n", msg);
}

void applyLed(int v, const char* reason) {
  v = v ? 1 : 0;
  if (v == ledValue) { publishAct(reason); return; }

  if (!canChangeLed()) {
    Serial.println("SAFETY: dwell time active, LED change delayed.");
    publishAct("DWELL_ACTIVE");
    return;
  }

  ledValue = v;
  digitalWrite(PIN_LED, ledValue ? HIGH : LOW);
  lastLedChangeMs = millis();

  Serial.printf("ACTUATION: LED=%d reason=%s\n", ledValue, reason);
  publishAct(reason);
}

void controlFromState(State s) {
  if (s == S_DIM) applyLed(1, "DIM");
  else if (s == S_BRIGHT) applyLed(0, "BRIGHT");
  else if (s == S_NORMAL) applyLed(0, "NORMAL");
  else applyLed(0, "UNKNOWN_STATE");
}

void onMqttMessage(char* topic, byte* payload, unsigned int length) {
  static char buf[256];
  unsigned int n = min(length, (unsigned int)(sizeof(buf) - 1));
  memcpy(buf, payload, n);
  buf[n] = '\0';

  Serial.printf("MQTT RX topic=%s payload=%s\n", topic, buf);

  if (strcmp(topic, TOPIC_STATE) == 0) {
    lastState = parseState(buf);
    lastStateRxMs = millis();
    if (mode == AUTO) controlFromState(lastState);
    return;
  }

  if (strcmp(topic, TOPIC_OVERRIDE) == 0) {
    mode = parseMode(buf);

    if (mode == MANUAL) {
      int v = parseLedValue(buf);
      if (v == 0 || v == 1) applyLed(v, "MANUAL");
      else Serial.println("MANUAL mode: missing led value, ignoring.");
    } else {
      controlFromState(lastState);
    }
    return;
  }
}

void setup() {
  Serial.begin(115200);
  delay(300);

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  connectWiFi();

  mqtt.setCallback(onMqttMessage);
  connectMqtt();

  Serial.println("Lab 5 — Configuration Layer started.");
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) connectWiFi();
  if (!mqtt.connected()) connectMqtt();
  mqtt.loop();

  // Fail-safe timeout in AUTO mode
  if (mode == AUTO) {
    uint32_t now = millis();
    if (lastStateRxMs != 0 && (now - lastStateRxMs) > STATE_TIMEOUT_MS) {
      applyLed(0, "FAIL_SAFE_TIMEOUT");
      lastStateRxMs = now; // prevent spam
    }
  }
}
