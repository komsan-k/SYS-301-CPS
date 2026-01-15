/*
  Lab 6: Automated CPS Control (ESP32)
  - Publishes room temperature to MQTT topic: /cps/temp/room   payload: {"t": <float>}
  - Subscribes to fan command topic:        /cps/cmd/fan      payload: "ON" or "OFF"
  - Drives an actuator pin (LED/FAN/Relay) based on received command

  Hardware:
  - ESP32 DevKit
  - FAN/LED/Relay input on FAN_PIN (default GPIO 5)  [change if needed]

  Notes:
  - If you use a relay module, ensure proper wiring and consider active-low modules.
  - For a real temperature sensor (LM73), replace readRoomTempC() with your sensor reading.

  Libraries:
  - WiFi (built-in)
  - PubSubClient (install via Library Manager)
*/

#include <WiFi.h>
#include <PubSubClient.h>
#include <math.h>   // for sin() in simulated temperature

/************ USER CONFIG ************/
const char* WIFI_SSID = "YOUR_WIFI";
const char* WIFI_PASS = "YOUR_PASS";

// MQTT broker IP/hostname and port
const char* MQTT_HOST = "192.168.1.10";
const int   MQTT_PORT = 1883;

// Topics
const char* TOPIC_TEMP_PUB = "/cps/temp/room";
const char* TOPIC_FAN_SUB  = "/cps/cmd/fan";

// Actuator pin
const int FAN_PIN = 5;   // change to match your wiring

// If your relay is active-low, set to true
const bool ACTUATOR_ACTIVE_LOW = false;

// Publish interval (ms)
const unsigned long PUBLISH_PERIOD_MS = 1000;

// Use simulated temperature (true) or replace readRoomTempC() for real sensor
const bool USE_SIM_TEMP = true;
/*************************************/

WiFiClient espClient;
PubSubClient mqtt(espClient);

unsigned long lastPub = 0;

static void setActuator(bool on) {
  if (ACTUATOR_ACTIVE_LOW) {
    digitalWrite(FAN_PIN, on ? LOW : HIGH);
  } else {
    digitalWrite(FAN_PIN, on ? HIGH : LOW);
  }
}

static float readRoomTempC() {
  if (USE_SIM_TEMP) {
    // Simulate temperature oscillating between ~25 and ~35 °C
    float t = 30.0f + 5.0f * sinf((float)millis() / 5000.0f * 2.0f * 3.1415926f);
    return t;
  }

  // TODO: Replace with real sensor reading (LM73, etc.)
  return 25.0f;
}

static void wifiConnect() {
  if (WiFi.status() == WL_CONNECTED) return;

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  Serial.print("WiFi connecting");
  unsigned long t0 = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
    if (millis() - t0 > 20000) { // 20s timeout then retry
      Serial.println("\nWiFi timeout, retrying...");
      WiFi.disconnect(true);
      delay(500);
      WiFi.begin(WIFI_SSID, WIFI_PASS);
      t0 = millis();
    }
  }
  Serial.print("\nWiFi connected. IP: ");
  Serial.println(WiFi.localIP());
}

static void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String msg;
  msg.reserve(length + 1);
  for (unsigned int i = 0; i < length; i++) msg += (char)payload[i];
  msg.trim();

  Serial.print("MQTT IN [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.println(msg);

  if (String(topic) == TOPIC_FAN_SUB) {
    bool on = (msg == "ON");
    setActuator(on);
    Serial.println(on ? "Actuator -> ON" : "Actuator -> OFF");
  }
}

static void mqttConnect() {
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setCallback(mqttCallback);

  while (!mqtt.connected()) {
    Serial.print("MQTT connecting...");
    String clientId = "esp32-lab6-";
    clientId += String((uint32_t)ESP.getEfuseMac(), HEX);

    if (mqtt.connect(clientId.c_str())) {
      Serial.println("connected.");
      mqtt.subscribe(TOPIC_FAN_SUB);
      Serial.print("Subscribed: ");
      Serial.println(TOPIC_FAN_SUB);
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt.state());
      Serial.println(" retry in 1s");
      delay(1000);
    }
  }
}

static void publishTemp() {
  float t = readRoomTempC();

  char buf[64];
  snprintf(buf, sizeof(buf), "{\"t\":%.2f}", t);

  bool ok = mqtt.publish(TOPIC_TEMP_PUB, buf);
  Serial.print("MQTT OUT [");
  Serial.print(TOPIC_TEMP_PUB);
  Serial.print("] ");
  Serial.print(buf);
  Serial.print("  => ");
  Serial.println(ok ? "OK" : "FAIL");
}

void setup() {
  Serial.begin(115200);
  delay(200);

  pinMode(FAN_PIN, OUTPUT);
  setActuator(false);

  wifiConnect();
  mqttConnect();
}

void loop() {
  wifiConnect();
  if (!mqtt.connected()) mqttConnect();
  mqtt.loop();

  unsigned long now = millis();
  if (now - lastPub >= PUBLISH_PERIOD_MS) {
    lastPub = now;
    publishTemp();
  }
}
