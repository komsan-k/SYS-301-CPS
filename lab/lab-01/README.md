# Lab: Sensor → TinyML Model (Decision) → Actuator (ESP32 + LDR)

This lab upgrades a classic CPS loop from **rule-based thresholding** to **TinyML-based decision-making** running **on-device** (ESP32).

**Pipeline:** `LDR Sensor → TinyML Inference (Anomaly Score) → Decision Policy → Actuator (LED / Relay)`

---

## 1) Learning Outcomes
By the end of this lab, students can:

- Interface an **LDR** with the ESP32 ADC
- Collect time-series data for TinyML training
- Deploy a TinyML model (e.g., anomaly detection) to ESP32
- Implement a stable **decision policy** (hysteresis + debounce)
- Control an **actuator** (LED/Relay) from TinyML outputs

---

## 2) Hardware

### Required
- ESP32 Dev Board
- LDR (photoresistor)
- Resistor **10 kΩ** (voltage divider)
- LED + **220 Ω** resistor
- Breadboard + jumper wires
- USB cable

### Optional (Real-world actuation)
- 1-channel relay module (3.3V/5V compatible)

---

## 3) Wiring

### 3.1 LDR Voltage Divider → ADC (Recommended)
Use **GPIO34 (ADC1)**

```
3.3V ── LDR ──┬── GPIO34 (ADC)
              │
             10k
              │
             GND
```

### 3.2 LED Actuator
- GPIO2 → 220 Ω → LED → GND

### 3.3 Optional Relay Actuator
- IN → GPIO26 (example)
- VCC → 5V or 3.3V (depends on module)
- GND → GND  
> Note: Some relays are **active LOW** (LOW = ON). See code config.

---

## 4) Software Prerequisites
- Arduino IDE (or PlatformIO)
- ESP32 board support installed in Arduino IDE
- Exported TinyML Arduino library (from your TinyML toolchain)

---

## 5) Part A — Data Acquisition (Arduino Code)

### A1) Stream LDR data to Serial (20 Hz)
Upload this sketch to collect raw sensor readings:

**File:** `stream_ldr_serial.ino`

```cpp
#define LDR_PIN 34

void setup() {
  Serial.begin(115200);
}

void loop() {
  int x = analogRead(LDR_PIN);
  Serial.println(x);
  delay(50); // 20 Hz
}
```

### A2) Data collection guidelines
Collect:
- **Normal baseline:** 3–5 minutes (steady room lighting)
- **Anomaly events:** hand blocking, flashlight flicker, sudden changes (repeat several times)

Recommended ML windowing:
- Sampling rate: **20 Hz**
- Window size: **2 seconds** → 40 samples/window
- Window shift: **1 second** (50% overlap)

---

## 6) Part B — Train & Export TinyML Model (High-level)
Train an **anomaly detection** model (recommended for LDR) so the model outputs:

- `anomaly_score` (higher = more abnormal)

Then **Export as Arduino Library** and install it in Arduino IDE.

> The exported library provides a header like:
> `your_model_inferencing.h` and a function like `run_classifier(...)`.

---

## 7) Part C — TinyML Inference → Decision Policy → Actuator (Arduino Code)

### C1) Key CPS idea: Decision Policy layer
Do **not** directly act on raw anomaly score. Use:
- **Hysteresis:** ON threshold > OFF threshold
- **Debounce:** require consecutive windows to switch states

This prevents actuator flickering.

---

### C2) Inference + Actuation Sketch (Template)
**File:** `ldr_tinyml_anomaly_actuator.ino`

> IMPORTANT:
> 1) Replace the include line with your exported header name.
> 2) This template assumes your SDK exposes `result.anomaly`. If your tool uses a different field name, adjust accordingly.

```cpp
#include <Arduino.h>

// ===== Replace this with your exported model header =====
#include <your_model_inferencing.h>

#define LDR_PIN   34
#define LED_PIN   2

// Optional relay actuator
#define USE_RELAY 0
#define RELAY_PIN 26
#define RELAY_ACTIVE_LOW 1  // set 1 if relay turns ON when pin is LOW

// Training-consistent window: 20 Hz * 2 s = 40 samples
static const int N = 40;
static float buffer[N];

// ---- CPS Decision Policy (tune these) ----
float ANOMALY_ON  = 0.60;  // switch alarm ON above this score
float ANOMALY_OFF = 0.45;  // switch alarm OFF below this score (hysteresis)
int   ON_COUNT    = 3;     // require consecutive windows to turn ON
int   OFF_COUNT   = 3;     // require consecutive windows to turn OFF

int on_hits = 0;
int off_hits = 0;
bool alarm_on = false;

static void actuator_write(bool on) {
  // LED
  digitalWrite(LED_PIN, on ? HIGH : LOW);

#if USE_RELAY
  bool relay_level;
  if (RELAY_ACTIVE_LOW) relay_level = on ? LOW : HIGH;
  else                  relay_level = on ? HIGH : LOW;
  digitalWrite(RELAY_PIN, relay_level);
#endif
}

static void capture_window_20hz_2s() {
  for (int i = 0; i < N; i++) {
    buffer[i] = (float)analogRead(LDR_PIN);
    delay(50); // 20 Hz
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
#if USE_RELAY
  pinMode(RELAY_PIN, OUTPUT);
#endif
  actuator_write(false);

  Serial.println("=== LDR TinyML Anomaly Detection Lab ===");
  Serial.println("Window: 40 samples @ 20Hz (2 seconds)");
}

void loop() {
  // 1) Capture sensor window
  capture_window_20hz_2s();

  // 2) Build signal from buffer (Edge Impulse-style helper)
  ei::signal_t signal;
  int err = numpy::signal_from_buffer(buffer, N, &signal);
  if (err != 0) {
    Serial.println("ERROR: signal_from_buffer failed");
    // Fail-safe
    alarm_on = false;
    actuator_write(false);
    return;
  }

  // 3) Run inference
  ei_impulse_result_t result = { 0 };
  EI_IMPULSE_ERROR rc = run_classifier(&signal, &result, false);
  if (rc != EI_IMPULSE_OK) {
    Serial.print("ERROR: run_classifier failed: ");
    Serial.println((int)rc);
    // Fail-safe
    alarm_on = false;
    actuator_write(false);
    return;
  }

  // 4) Read anomaly score
  float score = result.anomaly;

  // 5) Decision policy (hysteresis + debounce)
  if (!alarm_on) {
    if (score > ANOMALY_ON) {
      on_hits++;
      if (on_hits >= ON_COUNT) {
        alarm_on = true;
        on_hits = 0;
        off_hits = 0;
      }
    } else {
      on_hits = 0;
    }
  } else {
    if (score < ANOMALY_OFF) {
      off_hits++;
      if (off_hits >= OFF_COUNT) {
        alarm_on = false;
        off_hits = 0;
        on_hits = 0;
      }
    } else {
      off_hits = 0;
    }
  }

  // 6) Actuate
  actuator_write(alarm_on);

  // 7) Log for report
  Serial.print("anomaly_score=");
  Serial.print(score, 3);
  Serial.print("  alarm=");
  Serial.println(alarm_on ? "ON" : "OFF");
}
```

---

## 8) Tuning Guide (Quick)
1. Observe `anomaly_score` under **normal** conditions → note typical range  
2. Trigger anomalies (hand cover / flicker) → note abnormal score range  
3. Set thresholds so:
   - `ANOMALY_ON` is above most normal scores
   - `ANOMALY_OFF` is lower than `ANOMALY_ON` (e.g., ON-0.10 to ON-0.20)
4. If LED flickers, increase `ON_COUNT` / `OFF_COUNT` (2–5)

---

## 9) Experiment Checklist
- [ ] Circuit assembled (LDR + divider + LED)
- [ ] Serial streaming works at 20 Hz
- [ ] Dataset collected (normal + anomaly)
- [ ] TinyML model trained + exported as Arduino library
- [ ] Inference runs on ESP32
- [ ] Actuator responds correctly and stably (no flicker)

---

## 10) Deliverables
1. Circuit photo + pin mapping table  
2. Baseline anomaly score range (normal vs abnormal)  
3. Final tuned thresholds: `ANOMALY_ON`, `ANOMALY_OFF`, `ON_COUNT`, `OFF_COUNT`  
4. Demo evidence: screenshots from Serial Monitor or short video  

---

## 11) Discussion Questions
1. Why is window-based inference important compared to single-sample thresholding?
2. What happens if hysteresis is removed?
3. How would you extend this lab to multiple sensors (e.g., LDR + temperature)?
4. How can SensorML metadata help describe this sensor/actuator pipeline in a digital twin?

---

## License
Educational use (CPS/TinyML lab material).

