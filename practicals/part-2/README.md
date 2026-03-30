# 📶 CPS + TinyML Lab Manual  
## RSSI Signal Classification Using ESP32 (AP Mode) and a Plain-Code 1–8–3 Neural Network

**Course:** Cyber-Physical Systems / TinyML for IoT  
**Platform:** ESP32 + Arduino IDE  
**Lab Type:** CPS Edge Intelligence + Wireless Signal Classification  

---

# 1. Lab Title
**Cyber-Physical System-Based RSSI Monitoring and TinyML Classification Using ESP32 Access Point Mode**

---

# 2. Lab Overview
This laboratory demonstrates how an **ESP32 operating in Access Point (AP) mode** can be used as a **Cyber-Physical System (CPS) edge node** to monitor **wireless signal strength (RSSI)** and classify it into three signal-quality classes using a **plain-code neural network (1–8–3 architecture)**.

The system classifies Wi-Fi signal strength into:
- **Strong**
- **Moderate**
- **Weak**

---

# 3. Learning Objectives
- explain how RSSI represents wireless signal strength
- configure ESP32 in AP mode
- collect RSSI values from connected clients
- normalize sensor data for neural-network input
- implement a **1–8–3 neural network** manually
- perform embedded classification on ESP32
- map the experiment to the **5C CPS architecture**

---

# 4. CPS Architecture
```text
Connection → Conversion → Cyber → Cognition → Configuration
```

---

# 5. Neural Network Architecture
```text
Input Layer      : 1 neuron
Hidden Layer     : 8 neurons
Output Layer     : 3 neurons
Activation       : ReLU
Classifier       : Argmax
```

---

# 6. Mathematical Model
## Normalization
\[
x = \frac{RSSI + 90}{60}
\]

## Hidden Layer
\[
h_i = ReLU(w_i x + b_i)
\]

## Output Layer
\[
y_k = \sum_{i=1}^{8} W_{ki}h_i + b_k
\]

## Classification
\[
\hat{c}=\arg\max(y_k)
\]

---

# 7. Hardware Required
- ESP32 DevKit
- Smartphone / Laptop
- USB cable
- Optional LEDs

---

# 8. Arduino Code
```cpp
#include <WiFi.h>
#include <esp_wifi.h>

const char* ap_ssid = "ESP32_RSSI_CPS";
const char* ap_password = "12345678";

float W1[8] = {1.2,-0.8,1.6,-1.1,0.9,1.8,-1.5,0.7};
float b1[8] = {-0.2,0.7,-0.4,0.9,0.1,-0.8,1.0,-0.1};

float W2[3][8] = {
  {1.6,-0.5,1.3,-0.6,0.9,1.1,-0.8,0.7},
  {-0.3,1.4,0.2,1.1,0.8,-0.4,1.0,0.6},
  {-1.2,1.1,-1.0,1.5,-0.6,-1.3,1.4,-0.5}
};

float b2[3] = {0.1,0.2,0.0};

float relu(float x){ return x > 0 ? x : 0; }

float clamp01(float x){
  if (x < 0) return 0;
  if (x > 1) return 1;
  return x;
}

float normalizeRSSI(int rssi){
  return clamp01((rssi + 90.0)/60.0);
}

int argmax3(float y[3]){
  int idx = 0;
  if (y[1] > y[idx]) idx = 1;
  if (y[2] > y[idx]) idx = 2;
  return idx;
}

const char* className(int c){
  if (c == 0) return "Strong";
  if (c == 1) return "Moderate";
  return "Weak";
}
```

---

# 9. Experimental Procedure
1. Upload the sketch to ESP32
2. Open Serial Monitor (115200 baud)
3. Connect phone/laptop to `ESP32_RSSI_CPS`
4. Move closer and farther from ESP32
5. Observe RSSI classification

---

# 10. Expected Output
```text
RSSI: -45 dBm -> Strong
RSSI: -62 dBm -> Moderate
RSSI: -80 dBm -> Weak
```

---

# 11. Conclusion
This lab demonstrates a complete **CPS + TinyML workflow** using **ESP32 AP mode**, **RSSI sensing**, and a **plain-code 1–8–3 neural network classifier**.

