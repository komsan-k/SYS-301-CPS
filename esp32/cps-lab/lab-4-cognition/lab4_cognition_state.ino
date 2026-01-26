/*
  Lab 4 (5C) — Cognition Layer
  Edge Decision-Making (Rule-Based)

  - Reads ADC (GPIO34), runs EMA filter
  - Produces a stable discrete state using hysteresis
  - Publishes state to MQTT

  Library:
    PubSubClient

  States:
    BRIGHT, NORMAL, DIM
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
static const char* TOPIC_STATE = "cps/esp32/state";

// Hardware
static const int PIN_ADC = 34;

// Filter
static float filt = 0.0f;
static const float ALPHA = 0.15f;

// Hysteresis thresholds (tune!)
static float T_LOW  = 1200;  // below -> DIM
static float T_HIGH = 2500;  // above -> BRIGHT
static float HYST   = 150;   // band

enum State { DIM, NORMAL, BRIGHT };
State state = NORMAL;

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
    } else {
      Serial.printf("failed rc=%d. retry in 1s\n", mqtt.state());
      delay(1000);
    }
  }
}

const char* stateToStr(State s) {
  switch (s) {
    case DIM: return "DIM";
    case NORMAL: return "NORMAL";
    case BRIGHT: return "BRIGHT";
    default: return "UNKNOWN";
  }
}

State updateStateWithHysteresis(State current, float x) {
  if (current == DIM) {
    if (x > (T_LOW + HYST)) return NORMAL;
    return DIM;
  }
  if (current == BRIGHT) {
    if (x < (T_HIGH - HYST)) return NORMAL;
    return BRIGHT;
  }
  // NORMAL
  if (x < T_LOW) return DIM;
  if (x > T_HIGH) return BRIGHT;
  return NORMAL;
}

void publishState(State s, float filtVal, int raw) {
  char msg[220];
  uint32_t ts = millis();
  snprintf(msg, sizeof(msg),
           "{\"device\":\"%s\",\"ts\":%lu,\"raw\":%d,\"filt\":%.2f,\"state\":\"%s\"}",
           DEVICE_ID, (unsigned long)ts, raw, filtVal, stateToStr(s));
  mqtt.publish(TOPIC_STATE, msg);
  Serial.printf("STATE TX %s\n", msg);
}

void setup() {
  Serial.begin(115200);
  delay(300);

  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);

  connectWiFi();
  connectMqtt();

  Serial.println("Lab 4 — Cognition Layer started.");
  Serial.println("Tune T_LOW/T_HIGH/HYST for your lighting.");
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) connectWiFi();
  if (!mqtt.connected()) connectMqtt();
  mqtt.loop();

  static uint32_t last = 0;
  const uint32_t PERIOD_MS = 200;

  uint32_t now = millis();
  if (now - last >= PERIOD_MS) {
    last = now;

    int raw = analogRead(PIN_ADC);
    filt = (ALPHA * raw) + ((1.0f - ALPHA) * filt);

    State next = updateStateWithHysteresis(state, filt);
    state = next;

    // publish periodically (good for dashboards) - adjust as needed
    publishState(state, filt, raw);
  }
}
