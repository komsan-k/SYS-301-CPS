/*
  Lab 3 (5C) — Cyber Layer
  ESP32 Networking Node (Wi‑Fi + MQTT + JSON)

  Publishes telemetry:
    cps/esp32/telemetry

  Subscribes commands:
    cps/esp32/cmd

  Library required:
    PubSubClient (Arduino Library Manager)

  NOTE:
  - This template uses minimal JSON parsing (string search) to keep dependencies light.
  - For production-grade parsing, consider ArduinoJson.
*/

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

// ====== USER SETTINGS ======
static const char* WIFI_SSID = "YOUR_WIFI_SSID";
static const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";

static const char* MQTT_HOST = "192.168.1.10"; // broker IP or domain
static const uint16_t MQTT_PORT = 1883;

static const char* DEVICE_ID = "esp32-01";

static const char* TOPIC_TELEM = "cps/esp32/telemetry";
static const char* TOPIC_CMD   = "cps/esp32/cmd";

// Sensor
static const int PIN_ADC = 34;
// LED output (for command test)
static const int PIN_LED = 2;

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

static uint32_t sampleMs = 200; // can be updated by cmd
static float filt = 0.0f;

// Simple filter: EMA
static const float ALPHA = 0.2f;

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

void handleCmd(const char* payload) {
  // Examples:
  // {"led":1}
  // {"sample_ms":200}

  if (strstr(payload, "\"led\"") != nullptr) {
    const char* p = strchr(payload, ':');
    if (p) {
      int v = atoi(p + 1);
      digitalWrite(PIN_LED, v ? HIGH : LOW);
      Serial.printf("CMD: led=%d\n", v);
    }
  }

  if (strstr(payload, "\"sample_ms\"") != nullptr) {
    const char* p = strchr(payload, ':');
    if (p) {
      int v = atoi(p + 1);
      if (v >= 20 && v <= 5000) { // safety bounds
        sampleMs = (uint32_t)v;
        Serial.printf("CMD: sample_ms=%u\n", sampleMs);
      } else {
        Serial.println("CMD: sample_ms rejected (out of bounds)");
      }
    }
  }
}

void onMqttMessage(char* topic, byte* payload, unsigned int length) {
  static char buf[256];
  unsigned int n = min(length, (unsigned int)(sizeof(buf) - 1));
  memcpy(buf, payload, n);
  buf[n] = '\0';

  Serial.printf("MQTT RX topic=%s payload=%s\n", topic, buf);

  if (strcmp(topic, TOPIC_CMD) == 0) {
    handleCmd(buf);
  }
}

void connectMqtt() {
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setCallback(onMqttMessage);

  while (!mqtt.connected()) {
    Serial.print("MQTT connecting...");
    String clientId = String(DEVICE_ID) + "-" + String((uint32_t)ESP.getEfuseMac(), HEX);
    if (mqtt.connect(clientId.c_str())) {
      Serial.println("connected.");
      mqtt.subscribe(TOPIC_CMD);
    } else {
      Serial.printf("failed rc=%d. retry in 1s\n", mqtt.state());
      delay(1000);
    }
  }
}

void publishTelemetry(int raw, float filtVal) {
  char msg[200];
  uint32_t ts = millis();
  snprintf(msg, sizeof(msg),
           "{\"device\":\"%s\",\"ts\":%lu,\"raw\":%d,\"filt\":%.2f}",
           DEVICE_ID, (unsigned long)ts, raw, filtVal);

  mqtt.publish(TOPIC_TELEM, msg);
  Serial.printf("MQTT TX %s\n", msg);
}

void setup() {
  Serial.begin(115200);
  delay(300);

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);

  connectWiFi();
  connectMqtt();

  Serial.println("Lab 3 — Cyber Layer started.");
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }
  if (!mqtt.connected()) {
    connectMqtt();
  }
  mqtt.loop();

  static uint32_t last = 0;
  uint32_t now = millis();
  if (now - last >= sampleMs) {
    last = now;
    int raw = analogRead(PIN_ADC);
    filt = (ALPHA * raw) + ((1.0f - ALPHA) * filt);
    publishTelemetry(raw, filt);
  }
}
