# AI-Enhanced RSSI Prediction (TinyML)
## ESP32 + TensorFlow Lite for Microcontrollers + Node-RED Digital Twin

This document presents a complete **AI-enhanced RSSI prediction framework** using **TinyML on ESP32** for Cyber-Physical Systems (CPS) and Digital Twin applications.

The system learns the trend of Wi-Fi RSSI values and predicts future signal quality as:

- **Strong**
- **Medium**
- **Weak**

It can be integrated with:

- ESP32 edge sensing
- TensorFlow Lite for Microcontrollers (TFLM)
- MQTT communication
- Node-RED dashboards
- CPS 5C architecture

---

## 1. System Concept

The full pipeline is:

```text
Wi-Fi RSSI sensing → windowed features → TinyML model on ESP32 → prediction → dashboard / alert / control
```

Typical capabilities:

- Predict whether RSSI will drop in the next few seconds
- Trigger alerts before signal degradation
- Drive LEDs / buzzers / MQTT notifications
- Feed a Node-RED digital twin

---

## 2. Problem Formulation

### 2.1 Classification Approach

The recommended approach is **3-class classification**:

| Class | Meaning | RSSI Range |
|------|---------|------------|
| 0 | Strong | RSSI ≥ -55 dBm |
| 1 | Medium | -70 ≤ RSSI < -55 dBm |
| 2 | Weak | RSSI < -70 dBm |

This is ideal for ESP32 because:

- model is small
- inference is fast
- output is easy to interpret

### 2.2 Regression Alternative

A regression model could predict future RSSI directly, such as:

```text
RSSI(t+1)
```

However, for teaching labs and embedded deployment, **classification is easier and more robust**.

---

## 3. Feature Engineering

Instead of using only the current RSSI value, use a short history window.

### Example feature vector

```text
x = [RSSI(t), RSSI(t-1), RSSI(t-2), RSSI(t-3), RSSI(t-4)]
```

Extended version:

```text
x = [rssi0, rssi1, rssi2, rssi3, rssi4, mean, diff]
```

Where:

- `mean` = average RSSI over recent samples
- `diff` = current RSSI − previous RSSI

### Recommended starting point

For ESP32 lab deployment:

- **Input size = 5**
- **Model = 5 → 8 → 3**
- **Output = Strong / Medium / Weak**

---

## 4. Dataset Format

Example CSV dataset:

```csv
rssi0,rssi1,rssi2,rssi3,rssi4,label
-58,-57,-56,-56,-55,0
-68,-67,-69,-70,-71,1
-78,-79,-80,-81,-82,2
```

Where:

- `rssi0` = latest RSSI
- `rssi4` = oldest value in the window
- `label` = future class or target class

---

## 5. Python Training Script

Save as:

```text
train_rssi_predictor_int8.py
```

```python
import numpy as np
import tensorflow as tf
from tensorflow import keras

# X shape = (N, 5)
# y shape = (N,)
X = np.load("X_rssi.npy").astype(np.float32)
y = np.load("y_rssi.npy").astype(np.int32)

# Normalize RSSI from [-100, -30] to [0, 1]
Xn = (X + 100.0) / 70.0
Xn = np.clip(Xn, 0.0, 1.0)

model = keras.Sequential([
    keras.layers.Input(shape=(5,)),
    keras.layers.Dense(8, activation="relu"),
    keras.layers.Dense(3, activation="softmax")
])

model.compile(
    optimizer="adam",
    loss="sparse_categorical_crossentropy",
    metrics=["accuracy"]
)

model.fit(Xn, y, epochs=40, batch_size=32, validation_split=0.2)

converter = tf.lite.TFLiteConverter.from_keras_model(model)
converter.optimizations = [tf.lite.Optimize.DEFAULT]

def representative_dataset():
    for i in range(min(200, len(Xn))):
        yield [Xn[i:i+1]]

converter.representative_dataset = representative_dataset
converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS_INT8]
converter.inference_input_type = tf.int8
converter.inference_output_type = tf.int8

tflite_model = converter.convert()
open("rssi_predictor_int8.tflite", "wb").write(tflite_model)

print("Saved: rssi_predictor_int8.tflite")
```

---

## 6. Convert TFLite to C Header

### Option A: `xxd`

```bash
xxd -i rssi_predictor_int8.tflite > rssi_model_data.h
```

### Option B: Python converter

```bash
python tflite_to_c.py rssi_predictor_int8.tflite rssi_model_data.h
```

---

## 7. ESP32 Inference Concept

The ESP32 works as follows:

1. Measure RSSI every 200–500 ms
2. Store the latest 5 RSSI values
3. Normalize values
4. Feed them into the TinyML model
5. Read 3-class output
6. Publish result to Serial / MQTT / Node-RED

### Example normalization

```cpp
float rssi_window[5] = {-60, -61, -62, -64, -66};

for (int i = 0; i < 5; i++) {
  float x = (rssi_window[i] + 100.0f) / 70.0f;
  x = constrain(x, 0.0f, 1.0f);
}
```

---

## 8. ESP32 TFLM Sketch Skeleton

Save as:

```text
esp32_rssi_tinyml_mqtt.ino
```

```cpp
#include <WiFi.h>
#include <PubSubClient.h>
#include "TensorFlowLite.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"
#include "rssi_model_data.h"

const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";
const char* mqtt_server = "192.168.1.10";

WiFiClient espClient;
PubSubClient client(espClient);

tflite::MicroErrorReporter micro_error_reporter;
tflite::ErrorReporter* error_reporter = &micro_error_reporter;

const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;

constexpr int kTensorArenaSize = 10 * 1024;
static uint8_t tensor_arena[kTensorArenaSize];

TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;

float rssi_window[5] = {-60, -60, -60, -60, -60};

static inline int8_t clamp_int8(int v) {
  if (v < -128) return -128;
  if (v > 127) return 127;
  return (int8_t)v;
}

const char* label_from_idx(int idx) {
  switch (idx) {
    case 0: return "STRONG";
    case 1: return "MEDIUM";
    case 2: return "WEAK";
    default: return "UNKNOWN";
  }
}

int argmax3(const float p[3]) {
  int best = 0;
  if (p[1] > p[best]) best = 1;
  if (p[2] > p[best]) best = 2;
  return best;
}

void setup_wifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void setup_tflm() {
  model = tflite::GetModel(g_model);

  static tflite::MicroMutableOpResolver<3> resolver;
  resolver.AddFullyConnected();
  resolver.AddRelu();
  resolver.AddSoftmax();

  static tflite::MicroInterpreter static_interpreter(
      model, resolver, tensor_arena, kTensorArenaSize, error_reporter);

  interpreter = &static_interpreter;
  interpreter->AllocateTensors();

  input = interpreter->input(0);
  output = interpreter->output(0);
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  setup_tflm();
}

void loop() {
  // shift window
  for (int i = 4; i > 0; i--) {
    rssi_window[i] = rssi_window[i - 1];
  }
  rssi_window[0] = WiFi.RSSI();

  // quantize 5 inputs
  const float in_scale = input->params.scale;
  const int in_zero = input->params.zero_point;

  for (int i = 0; i < 5; i++) {
    float x = (rssi_window[i] + 100.0f) / 70.0f;
    x = constrain(x, 0.0f, 1.0f);
    int q = (int)roundf(x / in_scale) + in_zero;
    input->data.int8[i] = clamp_int8(q);
  }

  interpreter->Invoke();

  const float out_scale = output->params.scale;
  const int out_zero = output->params.zero_point;

  float p[3];
  for (int i = 0; i < 3; i++) {
    p[i] = ((int)output->data.int8[i] - out_zero) * out_scale;
  }

  int cls = argmax3(p);
  const char* label = label_from_idx(cls);

  char payload[128];
  snprintf(payload, sizeof(payload),
           "{"rssi":%.1f,"predicted_state":"%s","p0":%.3f,"p1":%.3f,"p2":%.3f}",
           rssi_window[0], label, p[0], p[1], p[2]);

  client.publish("cps/lab/rssi/prediction", payload);
  Serial.println(payload);

  delay(500);
}
```

---

## 9. MQTT / Node-RED Digital Twin

### Topic

```text
cps/lab/rssi/prediction
```

### Example MQTT payload

```json
{
  "rssi": -68,
  "predicted_state": "WEAK",
  "p0": 0.12,
  "p1": 0.31,
  "p2": 0.82
}
```

### Node-RED dashboard ideas

Display:
- current RSSI
- predicted class
- confidence bars
- historical trend chart
- alert indicator

---

## 10. CPS 5C Mapping

| CPS Layer | Role in AI-enhanced RSSI system |
|---|---|
| Connection | ESP32 measures RSSI |
| Conversion | Windowing and normalization |
| Cyber | TinyML model + digital twin |
| Cognition | Predict future signal quality |
| Configuration | Alert / LED / handover action |

---

## 11. Example Use Cases

- Smart Wi-Fi monitoring
- RSSI-based mobility prediction
- Smart handover decision
- EM-aware CPS monitoring
- Wireless digital twin for smart buildings

---

## 12. Advantages of TinyML for RSSI

- Runs on ESP32 without cloud
- Low latency
- Low power consumption
- Real-time CPS support
- Easy integration with MQTT / Node-RED

---

## 13. Limitations

- RSSI is noisy
- Requires real dataset for best accuracy
- Environmental changes may require retraining
- Small models capture only limited patterns

---

## 14. Recommended Lab Version

A balanced lab setup is:

- input = 5 recent RSSI values
- model = 5–8–3
- output = Strong / Medium / Weak
- deployment = INT8
- communication = MQTT to Node-RED

This gives a strong balance of:

- simplicity
- speed
- accuracy
- educational value

---

## 15. Summary

This project demonstrates a complete **AI-enhanced RSSI Digital Twin pipeline**:

```text
Wi-Fi RSSI → ESP32 TinyML → MQTT → Node-RED Dashboard → CPS Decision
```

It is suitable for:

- TinyML labs
- CPS teaching
- Digital twin research
- Smart wireless monitoring systems
