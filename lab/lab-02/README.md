# Lab 2: MQTT Subscribe → Remote Actuator Control (ESP32, Wokwi)

## Overview
This laboratory extends Lab 1 (MQTT Publish) by enabling remote actuator control through MQTT subscription.
The ESP32 publishes sensor data while subscribing to control commands that remotely actuate an LED.

## Learning Objectives
- Implement MQTT subscribe functionality on ESP32
- Design remote actuator control using MQTT
- Understand bidirectional CPS communication
- Analyze latency and safety in networked CPS

## CPS Mapping
| CPS Layer | Implementation |
|----------|----------------|
| Physical | LDR, LED |
| Cyber | ESP32 firmware |
| Decision | Local threshold + remote MQTT command |
| Actuation | LED control |
| Network | MQTT publish & subscribe |

## Hardware Components (Simulated)
- ESP32 Dev Module
- LDR
- LED
- 10kΩ Resistor
- Breadboard & wires

## Software & Network Requirements
- Web browser
- Wokwi online simulator (https://wokwi.com)
- Public MQTT broker: broker.hivemq.com (Port 1883)
- MQTT client (MQTT Explorer / Node-RED / mosquitto_pub)

## MQTT Topics

### Publish (ESP32 → Broker)
- cps/lab2/ldr
- cps/lab2/state

### Subscribe (Broker → ESP32)
- cps/lab2/led/set  (Payload: ON / OFF)

## Arduino Code

```cpp
#include <WiFi.h>
#include <PubSubClient.h>

#define LDR_PIN 34
#define LED_PIN 2
#define THRESHOLD 2000

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

bool remoteOverride = false;
bool ledState = false;

void callback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (int i = 0; i < length; i++) msg += (char)payload[i];

  if (String(topic) == "cps/lab2/led/set") {
    remoteOverride = true;
    if (msg == "ON") ledState = true;
    if (msg == "OFF") ledState = false;
    digitalWrite(LED_PIN, ledState ? HIGH : LOW);
  }
}

void setupWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
}

void setupMQTT() {
  while (!client.connected()) {
    client.connect("ESP32_CPS_LAB2");
  }
  client.subscribe("cps/lab2/led/set");
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);

  setupWiFi();
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  setupMQTT();
}

void loop() {
  client.loop();

  int ldrValue = analogRead(LDR_PIN);
  bool isDark = ldrValue < THRESHOLD;

  if (!remoteOverride) {
    digitalWrite(LED_PIN, isDark ? HIGH : LOW);
  }

  client.publish("cps/lab2/ldr", String(ldrValue).c_str());
  client.publish("cps/lab2/state", isDark ? "DARK" : "BRIGHT");

  delay(1000);
}
```

## Expected Results
- LED can be controlled remotely via MQTT
- Sensor data is continuously published
- Remote commands override local logic

## Outcome
Students experience a fully bidirectional, networked CPS with remote actuation.
