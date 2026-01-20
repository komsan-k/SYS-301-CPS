
# ESP32 Sensor → MQTT → Node-RED → GPT Summary → Dashboard

This project demonstrates a **complete Cyber-Physical System (CPS) monitoring pipeline** that integrates
embedded sensing, IoT communication, low-code orchestration, and AI-assisted explanation.

**Workflow**
```
ESP32 Sensor → MQTT → Node-RED → GPT Summary → Node-RED Dashboard
```

You will obtain:
- 📊 **Live sensor values and trends** on a web dashboard  
- 🧠 **GPT-generated 1–2 sentence summaries** explaining the system state in natural language  
- 🎓 A reusable **teaching and lab template** for CPS, IoT, and Edge-AI courses  

---

## 1. System Architecture Overview

- **Physical Layer**: ESP32 with an LDR (light sensor)
- **Communication Layer**: MQTT (publish–subscribe model)
- **Cyber Layer**: Node-RED flow logic and data processing
- **Intelligence Layer**: GPT-based summarization (non-control, explanatory role)
- **Human Interface**: Node-RED Dashboard

This separation follows best practices in CPS design, where AI is used for **interpretation and explanation**, not safety-critical control.

---

## 2. Data Contract (ESP32 → MQTT)

The ESP32 publishes sensor readings to an MQTT broker.

### MQTT Topic
```
cps/esp32/sensor
```

### Payload Format (JSON)
```json
{
  "device": "esp32-01",
  "ts": 1700000000,
  "ldr": 623
}
```

**Field description**
- `device` – unique ESP32 identifier  
- `ts` – timestamp (seconds)  
- `ldr` – ADC value from light sensor (0–4095)  

A clear data contract simplifies integration with analytics, AI models, and dashboards.

---

## 3. ESP32 Arduino Example (MQTT Publish)

**Libraries required**
- `WiFi.h`
- `PubSubClient.h`

The ESP32:
1. Connects to Wi-Fi  
2. Connects to an MQTT broker  
3. Reads the LDR via ADC  
4. Publishes a JSON message every 2 seconds  

```cpp
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASS";

const char* mqtt_server = "192.168.1.10";   // Mosquitto broker IP
const int   mqtt_port   = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

const int LDR_PIN = 34; // ESP32 ADC pin
unsigned long lastSend = 0;

void setupWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(300);
}

void reconnectMQTT() {
  while (!client.connected()) {
    String cid = "esp32-" + String((uint32_t)ESP.getEfuseMac(), HEX);
    if (client.connect(cid.c_str())) break;
    delay(1000);
  }
}

void setup() {
  Serial.begin(115200);
  setupWifi();
  client.setServer(mqtt_server, mqtt_port);
  analogReadResolution(12); // 0..4095
}

void loop() {
  if (!client.connected()) reconnectMQTT();
  client.loop();

  if (millis() - lastSend > 2000) {
    lastSend = millis();

    int ldr = analogRead(LDR_PIN);
    unsigned long ts = millis() / 1000;

    char payload[128];
    snprintf(payload, sizeof(payload),
             "{\"device\":\"esp32-01\",\"ts\":%lu,\"ldr\":%d}",
             ts, ldr);

    client.publish("cps/esp32/sensor", payload);
    Serial.println(payload);
  }
}
```

---

## 4. Node-RED Flow (MQTT → GPT → Dashboard)

### Requirements
- **Node-RED**
- **node-red-dashboard**
- **node-red-contrib-custom-chatgpt**
- **MQTT broker** (Mosquitto recommended)
- **OpenAI API key**

### Flow Functions
- **MQTT In**: receives ESP32 sensor data  
- **JSON Node**: parses payload  
- **Function (Dashboard)**: feeds gauge and chart  
- **Function (Rolling Stats)**:
  - Maintains a sliding window of values
  - Computes average, min, max, and trend
  - Builds a prompt for GPT summarization  
- **ChatGPT Node**: generates a short explanation  
- **Dashboard Widgets**: display values and summary  

The GPT prompt is intentionally constrained to:
- 1–2 sentences  
- Simple language  
- Explanatory output only  

---

## 5. Configuration Steps

### MQTT Broker
- Open **Local Mosquitto** config node
- If the broker is remote, change:
```
127.0.0.1 → <broker IP>
```

### ChatGPT Node
- Open **Custom ChatGPT Summary**
- Set your **OpenAI API key** in the node configuration
- Model selection via:
```javascript
msg.topic = "gpt4o";
```

### Deploy and View
```
http://<node-red-host>:1880/ui
```

---

## 6. Dashboard Output

You will see:
- **Gauge** – current LDR value (0–4095)
- **Line Chart** – recent sensor trend
- **GPT Summary** – short, student-friendly explanation  
  > “Light level is decreasing, indicating a darker environment.”

---

## 7. Design Notes (CPS Best Practice)

- AI is used for **interpretation and explanation**, not actuation
- Control decisions should remain **rule-based and deterministic**
- This architecture supports extension to:
  - Closed-loop CPS
  - TinyML / Edge-AI inference
  - Digital Twin dashboards

---

## 8. References

1. Node-RED Documentation  
   https://nodered.org/docs/

2. Node-RED Dashboard  
   https://flows.nodered.org/node/node-red-dashboard

3. MQTT Protocol Specification  
   https://mqtt.org/

4. node-red-contrib-custom-chatgpt  
   https://flows.nodered.org/node/node-red-contrib-custom-chatgpt

5. OpenAI API Documentation  
   https://platform.openai.com/docs

6. E. A. Lee, “Cyber Physical Systems: Design Challenges,” *IEEE ISORC*, 2008.

7. O. Givehchi et al., “Cyber-Physical Systems: Architecture, Design, and Applications,”
   *IEEE Industrial Electronics Magazine*, 2019.

---

## Author / Instructor
**Assistant Professor Komsan Kanjanasit, PhD**  
College of Computing, Prince of Songkla University (Phuket)
