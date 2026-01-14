# Lab 1: Sensor → Decision → Actuator → MQTT (ESP32, Wokwi)

## Overview
This laboratory extends Lab 0 by introducing MQTT-based communication into the Cyber-Physical System (CPS) loop.
The ESP32 publishes sensor data and system state to an MQTT broker, enabling networked CPS monitoring.

## Learning Objectives
- Extend a local CPS with network communication
- Understand MQTT in IoT-based CPS
- Publish sensor data and actuator states
- Design MQTT topic hierarchies

## CPS Mapping
| CPS Layer | Implementation |
|----------|----------------|
| Physical | LDR, LED |
| Cyber | ESP32 firmware |
| Decision | Threshold-based logic |
| Actuation | LED control |
| Network | MQTT publish/subscribe |

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

## MQTT Topics
- cps/lab1/ldr
- cps/lab1/state
- cps/lab1/led

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

void setupWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void setupMQTT() {
  while (!client.connected()) {
    client.connect("ESP32_CPS_LAB1");
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  setupWiFi();
  client.setServer(mqttServer, mqttPort);
  setupMQTT();
}

void loop() {
  int ldrValue = analogRead(LDR_PIN);
  bool isDark = ldrValue < THRESHOLD;
  digitalWrite(LED_PIN, isDark ? HIGH : LOW);

  client.publish("cps/lab1/ldr", String(ldrValue).c_str());
  client.publish("cps/lab1/state", isDark ? "DARK" : "BRIGHT");
  client.publish("cps/lab1/led", isDark ? "ON" : "OFF");

  delay(1000);
}
```

## Expected Results
- LED responds to light intensity
- Sensor and state data published via MQTT

## Outcome
Students experience the transition from local CPS to networked CPS using MQTT.
