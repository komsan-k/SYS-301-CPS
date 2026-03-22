# 📡 ESP32 RSSI Digital Twin Lab Manual  
## CPS-Integrated Electromagnetic Monitoring using Wi-Fi Signals

---

## 🧠 1. Introduction

This lab demonstrates a **Cyber-Physical System (CPS)** integrated with a simple **Electromagnetic (EM) Digital Twin** using RSSI (Received Signal Strength Indicator) measurements from an ESP32.

Instead of complex RF instruments, this lab leverages Wi-Fi signal strength as a measurable EM parameter.

The system continuously:
- Senses real EM conditions (RSSI)
- Transmits data to a cyber layer
- Builds a live digital representation (Digital Twin)
- Performs basic decision-making

This lab bridges **EM theory → CPS → Digital Twin implementation**.

---

## 🎯 2. Learning Objectives

After completing this lab, students will be able to:

- Understand RSSI as an EM measurement  
- Interface ESP32 with Wi-Fi networks  
- Collect real-time EM data  
- Build a Digital Twin model  
- Analyze EM propagation effects  
- Map system to CPS 5C architecture  

---

## 🧩 3. System Architecture

```
        Wi-Fi Access Point
                │
                ▼
        ┌────────────────┐
        │    ESP32       │
        │ (RSSI Sensor)  │
        └────────────────┘
                │
        Serial / MQTT
                ▼
        ┌──────────────────────┐
        │ Digital Twin (PC)    │
        │ - Model              │
        │ - Visualization      │
        │ - Decision Logic     │
        └──────────────────────┘
                │
                ▼
        Dashboard / Alert / LED
```

---

## 🧱 4. CPS 5C Mapping

| CPS Layer | Role |
|----------|-----|
| Connection | ESP32 Wi-Fi sensing |
| Conversion | RSSI → signal quality |
| Cyber | Digital twin model |
| Cognition | Classification |
| Configuration | System response |

---

## 🔌 5. Hardware Requirements

| Component | Quantity |
|----------|---------|
| ESP32 DevKit | 1 |
| USB Cable | 1 |
| Wi-Fi Router | 1 |
| LED (optional) | 1 |
| Resistor (220Ω) | 1 |

---

## 💻 6. Software Requirements

- Arduino IDE  
- ESP32 Board Package  
- Serial Monitor  

Optional:
- Python (matplotlib)  
- Node-RED  
- MQTT  

---

## ⚙️ 7. Setup Instructions

### Step 1: Install ESP32 Board
Add:
```
https://dl.espressif.com/dl/package_esp32_index.json
```

### Step 2: Connect ESP32
- Plug USB  
- Select COM port  

### Step 3: Upload Code

---

## 📟 8. Arduino Code

```cpp
#include <WiFi.h>

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {
  int rssi = WiFi.RSSI();

  String quality;
  if (rssi >= -55) quality = "STRONG";
  else if (rssi >= -70) quality = "MEDIUM";
  else quality = "WEAK";

  Serial.printf("RSSI: %d dBm | %s\n", rssi, quality);
  delay(1000);
}
```

---

## 💡 9. LED Alert (Optional)

```cpp
if (rssi < -70) digitalWrite(2, HIGH);
else digitalWrite(2, LOW);
```

---

## 🧪 10. Experiments

### Experiment 1: Distance
Measure RSSI at different distances

### Experiment 2: Obstacles
Insert wall/book → observe attenuation

### Experiment 3: Orientation
Rotate ESP32

### Experiment 4: Motion
Move device → observe fluctuation

---

## 📊 11. Sample Data

| Distance | RSSI | State |
|---------|------|------|
| 1m | -42 | Strong |
| 3m | -58 | Medium |
| 5m | -71 | Weak |

---

## 🧠 12. Digital Twin Model

```
RSSI ≥ -55 → Strong
-70 ≤ RSSI < -55 → Medium
RSSI < -70 → Weak
```

Example:
```json
{
  "rssi": -65,
  "state": "MEDIUM",
  "alert": false
}
```

---

## 📈 13. Python Visualization

```python
import serial
import matplotlib.pyplot as plt
```

---

## 🔍 14. Observations

- RSSI decreases with distance  
- Obstacles attenuate signals  
- Human body absorbs EM waves  
- Signal fluctuates  

---

## 🚀 15. Extensions

- MQTT + Node-RED  
- Multi-node sensing  
- Heatmap visualization  
- AI prediction  

---

## 🧾 16. Questions

- What is RSSI?  
- Why attenuation occurs?  
- CPS role?  

---

## 📚 17. Conclusion

This lab demonstrates a CPS-integrated EM Digital Twin using ESP32.

---

## 🏷️ Keywords

CPS, Digital Twin, ESP32, RSSI, IoT
