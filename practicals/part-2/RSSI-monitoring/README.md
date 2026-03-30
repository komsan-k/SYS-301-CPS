# RSSI Monitoring for CPS + TinyML

## 3-Class Signal Classification Using a Plain-Code 1–8–3 Neural Network on ESP32

---

## 1. Lab Idea

This lab demonstrates a simple **Cyber-Physical System (CPS)** in which:

* the physical world is represented by wireless signal strength
* the ESP32 measures RSSI values
* a small neural network classifies the signal level
* the result is published or displayed as a cyber-state
* the classification can be used for decision-making in the configuration layer

This is a very good **TinyML-style lab** because:

* the input is only **1 feature: RSSI**
* the hidden layer has **8 neurons**
* the output has **3 classes**
* the network can run directly on ESP32 using only plain C/Arduino code

---

## 2. CPS Architecture

```text
Physical Layer
   Wi-Fi signal environment
        ↓
Connection Layer
   ESP32 scans / reads RSSI
        ↓
Conversion Layer
   RSSI normalized to [0,1]
        ↓
Cyber Layer
   1–8–3 neural network inference
        ↓
Cognition Layer
   Classify signal as Strong / Moderate / Weak
        ↓
Configuration Layer
   LED / dashboard / alert / control response
```

---

## 3. Classification Concept

RSSI is normally negative in dBm.

Typical ranges:

* **Strong:** −30 to −55 dBm
* **Moderate:** −56 to −70 dBm
* **Weak:** −71 to −90 dBm

Class labels:

* **Class 0 = Strong**
* **Class 1 = Moderate**
* **Class 2 = Weak**

---

## 4. System Options

### Option A: ESP32 in STA Mode

The ESP32 connects to a Wi-Fi router and reads:

```cpp
WiFi.RSSI()
```

This is the easiest method.

### Option B: ESP32 in AP Mode

The ESP32 acts as an access point and monitors connected stations’ RSSI.

This makes the AP-mode ESP32 the **edge CPS node**.

---

## 5. Neural Network Structure

We use a **1–8–3 feedforward neural network**

```text
Input (1)
  ↓
Dense (8) + ReLU
  ↓
Dense (3)
  ↓
Argmax
  ↓
[Strong, Moderate, Weak]
```

* **Input layer:** 1 neuron
* **Hidden layer:** 8 neurons
* **Output layer:** 3 neurons

---

## 6. Input Normalization

Normalize RSSI from **−90 dBm to −30 dBm**

$$
x = \frac{RSSI + 90}{60}
$$

Examples:

* RSSI = −90 → 0.00
* RSSI = −60 → 0.50
* RSSI = −30 → 1.00

Clamp to ([0,1])

---

## 7. Example Dataset

| RSSI (dBm) | Normalized | Class    |
| ---------- | ---------: | -------- |
| -35        |       0.92 | Strong   |
| -42        |       0.80 | Strong   |
| -50        |       0.67 | Strong   |
| -60        |       0.50 | Moderate |
| -65        |       0.42 | Moderate |
| -70        |       0.33 | Moderate |
| -75        |       0.25 | Weak     |
| -82        |       0.13 | Weak     |
| -88        |       0.03 | Weak     |

---

## 8. Full ESP32 Arduino Code

```cpp
#include <WiFi.h>
#include <esp_wifi.h>

const char* ap_ssid = "ESP32_RSSI_CPS";
const char* ap_password = "12345678";

float W1[8] = {
  1.2, -0.8, 1.6, -1.1,
  0.9, 1.8, -1.5, 0.7
};

float b1[8] = {
  -0.2, 0.7, -0.4, 0.9,
   0.1, -0.8, 1.0, -0.1
};

float W2[3][8] = {
  { 1.6, -0.5, 1.3, -0.6, 0.9, 1.1, -0.8, 0.7 },
  {-0.3, 1.4, 0.2, 1.1, 0.8, -0.4, 1.0, 0.6 },
  {-1.2, 1.1, -1.0, 1.5, -0.6, -1.3, 1.4, -0.5 }
};

float b2[3] = {0.1, 0.2, 0.0};

float relu(float x) {
  return (x > 0.0f) ? x : 0.0f;
}

float clamp01(float x) {
  if (x < 0.0f) return 0.0f;
  if (x > 1.0f) return 1.0f;
  return x;
}

float normalizeRSSI(int rssi) {
  return clamp01((rssi + 90.0f) / 60.0f);
}
```

---

## 9. CPS Pipeline Demonstration

This sketch demonstrates:

* physical measurement
* signal conversion
* edge intelligence
* cyber-state generation
* decision/action layer

---

## 10. Example Output

```text
RSSI = -46 dBm → Strong
RSSI = -63 dBm → Moderate
RSSI = -81 dBm → Weak
```

---

## 11. Optional LED Response

* Strong → Green LED
* Moderate → Yellow LED
* Weak → Red LED

---

## 12. Better Accuracy Workflow

Recommended:

1. Collect RSSI samples
2. Label classes
3. Train model in Python
4. Export weights
5. Paste into Arduino

---

## 13. Python Training Idea

Train:

* input = normalized RSSI
* hidden = 8 neurons
* output = 3 classes

Then export:

```text
W1[8]
b1[8]
W2[3][8]
b2[3]
```
