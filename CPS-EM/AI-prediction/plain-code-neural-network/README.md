# ESP32 RSSI Predictor using Plain-Code Neural Network

This project replaces TensorFlow Lite with a **manual (plain-code) neural network implementation** on ESP32.

It uses a **5–8–3 MLP** to predict Wi-Fi signal quality:

- STRONG
- MEDIUM
- WEAK

---

## 📦 Features

- No TFLM required
- Lightweight & fast
- MQTT integration
- Real-time RSSI prediction
- CPS-ready architecture

---

## 🧠 Model Architecture

```
Input (5 RSSI values)
        ↓
Hidden Layer (8 neurons, ReLU)
        ↓
Output Layer (3 neurons, Softmax)
```

---

## ⚙️ Arduino Code

Save as: `esp32_rssi_plain_nn.ino`

```cpp
// (Full code provided below)

#include <WiFi.h>
#include <PubSubClient.h>
#include <math.h>

// ---------- USER CONFIG ----------
const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";
const char* mqtt_server = "192.168.1.10";

WiFiClient espClient;
PubSubClient client(espClient);

// RSSI buffer
float rssi_window[5] = {-60, -60, -60, -60, -60};

// ===== Replace with trained weights =====
float W1[8][5] = { {0.12,-0.08,0.05,0.10,-0.02}, {-0.15,0.20,-0.10,0.04,0.08},
{0.30,-0.12,0.06,-0.09,0.11}, {-0.07,0.18,0.14,-0.05,-0.03},
{0.09,0.07,-0.11,0.13,-0.06}, {-0.20,0.10,0.09,-0.04,0.16},
{0.05,-0.14,0.22,0.08,-0.01}, {0.11,0.03,-0.07,0.19,-0.13} };

float b1[8] = {0.01,-0.02,0.03,0.00,0.02,-0.01,0.04,0.01};

float W2[3][8] = {
{0.21,-0.10,0.13,-0.08,0.05,-0.14,0.09,0.07},
{-0.05,0.18,-0.11,0.22,-0.09,0.04,0.16,-0.02},
{0.12,0.06,-0.15,0.03,0.20,-0.07,-0.04,0.19}
};

float b2[3] = {0.02,-0.01,0.03};

float relu(float x){ return x>0?x:0; }

void softmax(float z[3], float p[3]){
  float maxv=z[0];
  for(int i=1;i<3;i++) if(z[i]>maxv) maxv=z[i];
  float sum=0;
  for(int i=0;i<3;i++){ p[i]=exp(z[i]-maxv); sum+=p[i]; }
  for(int i=0;i<3;i++) p[i]/=sum;
}

int argmax3(float p[3]){
  int b=0; if(p[1]>p[b]) b=1; if(p[2]>p[b]) b=2; return b;
}

const char* label(int i){
  if(i==0) return "STRONG";
  if(i==1) return "MEDIUM";
  return "WEAK";
}

void predict(float x[5], float p[3]){
  float h[8], z[3];

  for(int i=0;i<8;i++){
    float s=b1[i];
    for(int j=0;j<5;j++) s+=W1[i][j]*x[j];
    h[i]=relu(s);
  }

  for(int i=0;i<3;i++){
    float s=b2[i];
    for(int j=0;j<8;j++) s+=W2[i][j]*h[j];
    z[i]=s;
  }

  softmax(z,p);
}

void setup(){
  Serial.begin(115200);
  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED) delay(500);

  client.setServer(mqtt_server,1883);
}

void loop(){
  for(int i=4;i>0;i--) rssi_window[i]=rssi_window[i-1];
  rssi_window[0]=WiFi.RSSI();

  float x[5];
  for(int i=0;i<5;i++){
    x[i]=(rssi_window[i]+100.0)/70.0;
    x[i]=constrain(x[i],0,1);
  }

  float p[3];
  predict(x,p);

  int cls=argmax3(p);

  char payload[150];
  snprintf(payload,sizeof(payload),
  "{\"rssi\":%.1f,\"state\":\"%s\"}",
  rssi_window[0],label(cls));

  client.publish("cps/lab/rssi/prediction",payload);
  Serial.println(payload);

  delay(500);
}
```

---

## 📡 MQTT Output

Topic:
```
cps/lab/rssi/prediction
```

Example:
```json
{"rssi":-65,"state":"MEDIUM"}
```

---

## 🔄 Data Flow

```
WiFi RSSI → ESP32 NN → MQTT → Node-RED → Dashboard
```

---

## ⚡ Advantages

- Faster than TFLM for small models
- No tensor arena needed
- Easy debugging
- Ideal for teaching TinyML basics

---

## ⚠️ Notes

- Replace weights with trained model values
- Normalize RSSI correctly
- Use real dataset for best results

---

## ✅ Summary

This project demonstrates a **lightweight AI-based RSSI predictor** using a plain neural network on ESP32.

Perfect for:
- CPS labs
- TinyML education
- Digital twin systems
