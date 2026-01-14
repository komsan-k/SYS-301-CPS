/*
  Lab 4: TinyML + MQTT + Node-RED CPS Dashboard (ESP32 / Wokwi)

  Publishes:
    - cps/lab4/ldr   : raw ADC (0..4095)
    - cps/lab4/pred  : prediction label (DARK / BRIGHT)
    - cps/lab4/prob  : confidence (0..100)
    - cps/lab4/led   : actual LED state (ON / OFF)

  Subscribes:
    - cps/lab4/led/set : actuator command (ON / OFF)

  Notes:
  - This sketch includes a lightweight "TinyML-like" inference stub (logistic model).
    Replace tinyml_predict() with your real TFLM/Edge Impulse inference later.
  - Works well with Wokwi WiFi SSID: Wokwi-GUEST (no password).
*/

#include <WiFi.h>
#include <PubSubClient.h>

// -------------------- Pins --------------------
#define LDR_PIN 34
#define LED_PIN 2

// -------------------- WiFi --------------------
const char* WIFI_SSID = "Wokwi-GUEST";
const char* WIFI_PASS = "";

// -------------------- MQTT --------------------
const char* MQTT_HOST = "broker.hivemq.com";
const int   MQTT_PORT = 1883;

// Topics
const char* TOPIC_LDR     = "cps/lab4/ldr";
const char* TOPIC_PRED    = "cps/lab4/pred";
const char* TOPIC_PROB    = "cps/lab4/prob";
const char* TOPIC_LED     = "cps/lab4/led";
const char* TOPIC_LED_SET = "cps/lab4/led/set";

// Publish rate
const uint32_t PUBLISH_PERIOD_MS = 1000;

// Safety: if no command received for a while, you can choose to keep last state.
// Here we just keep last state; you can add a failsafe timeout if you want.
const uint32_t MQTT_RECONNECT_RETRY_MS = 1000;

WiFiClient espClient;
PubSubClient mqtt(espClient);

// -------------------- State --------------------
bool ledState = false;                // actual LED output state
uint32_t lastPublishMs = 0;
uint32_t lastReconnectAttemptMs = 0;

// Optional smoothing for ADC
static float ldrEma = 0.0f;
const float EMA_ALPHA = 0.2f;         // 0..1 (higher = more responsive)

// ---------- Utility ----------
static inline float clampf(float x, float lo, float hi) {
  if (x < lo) return lo;
  if (x > hi) return hi;
  return x;
}

// ------------------------------------------------------------
// TinyML Inference Stub (Replace with real model later)
// ------------------------------------------------------------
// Input: normalized sensor x in [0,1]
// Output: label + confidence (0..100)
//
// This is a tiny logistic classifier to mimic TinyML behavior:
//   p_dark = sigmoid(w*x + b)
//
// You can calibrate w/b so the decision boundary fits your sensor range.
static inline float sigmoid(float z) {
  // numerically safe-ish for typical small z
  return 1.0f / (1.0f + expf(-z));
}

void tinyml_predict(float x_norm_0_1, const char** outLabel, int* outConfidencePct) {
  // Tune these two numbers for your setup:
  // - If your environment is mostly bright, increase b negative, etc.
  // - You can also flip sign to invert behavior.
  const float w = -10.0f;   // negative slope: higher light -> lower "dark" probability
  const float b =  5.0f;    // bias shifts threshold

  float p_dark = sigmoid(w * x_norm_0_1 + b);   // 0..1
  float p_bright = 1.0f - p_dark;

  if (p_dark >= p_bright) {
    *outLabel = "DARK";
    *outConfidencePct = (int)roundf(p_dark * 100.0f);
  } else {
    *outLabel = "BRIGHT";
    *outConfidencePct = (int)roundf(p_bright * 100.0f);
  }
}

// ------------------------------------------------------------
// MQTT Callback: remote actuation
// ------------------------------------------------------------
void onMqttMessage(char* topic, byte* payload, unsigned int length) {
  // Build message string
  String msg;
  msg.reserve(length + 1);
  for (unsigned int i = 0; i < length; i++) msg += (char)payload[i];
  msg.trim();
  msg.toUpperCase();

  if (String(topic) == TOPIC_LED_SET) {
    if (msg == "ON") {
      ledState = true;
      digitalWrite(LED_PIN, HIGH);
    } else if (msg == "OFF") {
      ledState = false;
      digitalWrite(LED_PIN, LOW);
    }
    // Immediately publish LED status feedback
    mqtt.publish(TOPIC_LED, ledState ? "ON" : "OFF", true);
  }
}

// ------------------------------------------------------------
// Connectivity helpers
// ------------------------------------------------------------
void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
  }
}

String makeClientId() {
  // Unique-ish client id using MAC
  uint64_t mac = ESP.getEfuseMac();
  char buf[40];
  snprintf(buf, sizeof(buf), "ESP32_CPS_LAB4_%04X%08X",
           (uint16_t)(mac >> 32), (uint32_t)mac);
  return String(buf);
}

bool connectMQTT() {
  if (mqtt.connected()) return true;

  String clientId = makeClientId();
  bool ok = mqtt.connect(clientId.c_str());
  if (!ok) return false;

  mqtt.subscribe(TOPIC_LED_SET);

  // Publish initial LED status (retained)
  mqtt.publish(TOPIC_LED, ledState ? "ON" : "OFF", true);
  return true;
}

// ------------------------------------------------------------
// Setup / Loop
// ------------------------------------------------------------
void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.begin(115200);
  delay(200);

  connectWiFi();

  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setCallback(onMqttMessage);

  // Initial EMA seed
  ldrEma = (float)analogRead(LDR_PIN);
}

void loop() {
  // Maintain MQTT connection
  if (!mqtt.connected()) {
    uint32_t now = millis();
    if (now - lastReconnectAttemptMs >= MQTT_RECONNECT_RETRY_MS) {
      lastReconnectAttemptMs = now;
      connectMQTT();
    }
  }
  mqtt.loop();

  // Periodic publish
  uint32_t now = millis();
  if (now - lastPublishMs >= PUBLISH_PERIOD_MS) {
    lastPublishMs = now;

    // 1) Read sensor (raw)
    int raw = analogRead(LDR_PIN);           // 0..4095 (ESP32 ADC)
    // EMA smoothing
    ldrEma = (1.0f - EMA_ALPHA) * ldrEma + EMA_ALPHA * (float)raw;

    // 2) Feature: normalize to 0..1
    float x = clampf(ldrEma / 4095.0f, 0.0f, 1.0f);

    // 3) TinyML inference (stub)
    const char* predLabel = "UNKNOWN";
    int confPct = 0;
    tinyml_predict(x, &predLabel, &confPct);

    // 4) Publish telemetry
    // raw ADC
    char rawBuf[12];
    snprintf(rawBuf, sizeof(rawBuf), "%d", raw);
    mqtt.publish(TOPIC_LDR, rawBuf, false);

    // prediction label
    mqtt.publish(TOPIC_PRED, predLabel, false);

    // confidence (0..100)
    char confBuf[8];
    snprintf(confBuf, sizeof(confBuf), "%d", confPct);
    mqtt.publish(TOPIC_PROB, confBuf, false);

    // LED status (retained is useful so dashboard shows last state immediately)
    mqtt.publish(TOPIC_LED, ledState ? "ON" : "OFF", true);

    // Serial log
    Serial.printf("LDR(raw)=%d | EMA=%.1f | x=%.3f | pred=%s | conf=%d%% | LED=%s\n",
                  raw, ldrEma, x, predLabel, confPct, ledState ? "ON" : "OFF");
  }
}

