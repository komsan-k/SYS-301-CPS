# Lab 3: MQTT + Node-RED CPS Dashboard (ESP32, Wokwi)

## Overview
This lab extends Lab 2 (MQTT Subscribe → Remote Actuator Control) by adding a Node-RED dashboard for real-time CPS monitoring and remote actuation.
The system demonstrates a Human-in-the-Loop Cyber-Physical System (CPS).

## Learning Objectives
- Integrate ESP32 CPS with Node-RED dashboard
- Visualize sensor data using MQTT
- Remotely control actuators via dashboard
- Understand HMI roles in CPS architectures

## CPS Mapping
| CPS Layer | Implementation |
|---|---|
| Physical | LDR, LED |
| Cyber | ESP32 firmware |
| Decision | Local threshold + Human-in-the-loop |
| Actuation | LED |
| Network | MQTT |
| Interface | Node-RED Dashboard |

## Requirements

### Hardware (Simulated)
- ESP32 Dev Module (Wokwi)
- LDR + 10kΩ resistor
- LED

### Software
- Wokwi: https://wokwi.com
- Node-RED
- Node-RED Dashboard
- MQTT broker: broker.hivemq.com (Port 1883)

## MQTT Topics

### Publish (ESP32 → Broker)
- cps/lab3/ldr
- cps/lab3/state
- cps/lab3/led

### Subscribe (Broker → ESP32)
- cps/lab3/led/set

## ESP32 Arduino Code

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
  for (unsigned int i = 0; i < length; i++) msg += (char)payload[i];
  msg.trim();

  if (String(topic) == "cps/lab3/led/set") {
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

void reconnectMQTT() {
  while (!client.connected()) {
    client.connect("ESP32_CPS_LAB3");
    delay(200);
  }
  client.subscribe("cps/lab3/led/set");
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);

  setupWiFi();
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  reconnectMQTT();
}

void loop() {
  if (!client.connected()) reconnectMQTT();
  client.loop();

  int ldrValue = analogRead(LDR_PIN);
  bool isDark = (ldrValue < THRESHOLD);

  if (!remoteOverride) {
    ledState = isDark;
    digitalWrite(LED_PIN, ledState ? HIGH : LOW);
  }

  client.publish("cps/lab3/ldr", String(ldrValue).c_str());
  client.publish("cps/lab3/state", isDark ? "DARK" : "BRIGHT");
  client.publish("cps/lab3/led", ledState ? "ON" : "OFF");

  delay(1000);
}
```

## Node-RED Dashboard Setup
- MQTT-in nodes for sensor/state topics
- Dashboard gauge and text widgets
- UI switch/button publishing ON/OFF to cps/lab3/led/set
- Access dashboard at http://localhost:1880/ui

## Expected Results
- Live sensor visualization on dashboard
- LED controlled remotely from Node-RED
- Human-in-the-loop CPS demonstrated

## Outcome
Students gain experience designing a networked CPS with visualization and remote control.
