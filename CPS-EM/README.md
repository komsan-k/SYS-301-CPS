# 📡 ESP32 RSSI Digital Twin Lab  
## CPS-Integrated Electromagnetic Monitoring using Wi-Fi Signals

---

## 🧠 Introduction
This lab demonstrates a Cyber-Physical System (CPS) integrated with a simple EM Digital Twin using RSSI from ESP32.

---

## 🎯 Learning Objectives
- Understand RSSI
- Collect real-time EM data
- Build a simple digital twin
- Analyze EM propagation

---

## 🧩 System Architecture
Wi-Fi → ESP32 → Serial/MQTT → Digital Twin

---

## 🔌 Hardware
- ESP32 DevKit
- USB Cable
- Wi-Fi Router

---

## 💻 Software
- Arduino IDE
- ESP32 Board Package

---

## ⚙️ Arduino Code
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
  Serial.println(rssi);
  delay(1000);
}
```

---

## 🧪 Experiments
1. Distance effect
2. Obstacle effect
3. Motion effect

---

## 📊 Digital Twin Model
- Strong ≥ -55 dBm
- Medium ≥ -70 dBm
- Weak < -70 dBm

---

## 📚 Conclusion
Simple CPS-integrated EM digital twin using ESP32 RSSI.
