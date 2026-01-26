# ESP32-Based CPS Labs Aligned with the 5C Architecture

This repository contains a **5-lab Cyber-Physical Systems (CPS) sequence** implemented using **ESP32**, where **each lab maps exactly to one layer of the 5C CPS architecture**.

The labs are **course-ready**, including learning goals, required hardware/software, tasks, deliverables, and assessment criteria.  
They are suitable for **undergraduate senior / early graduate CPS, IoT, or Embedded Systems courses**.

---

## CPS–5C Mapping Overview

| CPS 5C Layer | Lab | Focus |
|-------------|-----|-------|
| Connection | Lab 1 | Physical sensing & actuation |
| Conversion | Lab 2 | Signal conditioning & features |
| Cyber | Lab 3 | Networking & data contracts |
| Cognition | Lab 4 | Edge intelligence |
| Configuration | Lab 5 | Closed-loop adaptive control |

---

## Lab 1 — Connection Layer  
### Sensor / Actuator Bring-Up on ESP32 (Physical I/O)

**Goal**  
Connect the physical world to the cyber node using basic ESP32 I/O.

### Core Skills
- GPIO digital input/output  
- ADC reading (LDR or potentiometer)  
- PWM output (LED dimming or servo control)

### Hardware
- ESP32  
- LDR + resistor (voltage divider)  
- LED + resistor  
- *(Optional: DHT22 sensor)*

### Tasks
1. Read LDR value using ESP32 ADC  
2. Stream raw ADC values over Serial Monitor  
3. Drive LED brightness using PWM proportional to light level  

### Deliverables
- Arduino sketch (.ino)  
- Screenshot or log of ADC values and LED response  
- Pin-mapping diagram (PNG)

### Assessment Criteria
- Correct wiring and stable ADC readings  
- Proper scaling from ADC → light-level proxy  
- Clean, modular code structure  

---

## Lab 2 — Conversion Layer  
### Signal Conditioning & Feature Extraction

**Goal**  
Convert raw sensor signals into meaningful, low-noise features.

### Core Skills
- Sensor calibration (min/max, offset)  
- Digital filtering (moving average or median)  
- Feature extraction (mean, variance, trend)

### Tasks
1. Capture calibration data (baseline and maximum)  
2. Apply noise filtering  
3. Compute features every 1–2 seconds:
   - Average  
   - Standard deviation  
   - Trend (Δ over window)

### Deliverables
- Arduino sketch  
- Short report including:
  - Raw vs. filtered data plot (CSV → graph)
  - Feature table (time, avg, std, trend)

### Assessment Criteria
- Quantified noise reduction  
- Correct feature computation  
- Clear explanation of processing pipeline  

---

## Lab 3 — Cyber Layer  
### ESP32 Networking Node (MQTT + Data Contract)

**Goal**  
Transform the ESP32 into a networked CPS node.

### Core Skills
- Wi-Fi provisioning  
- MQTT publish/subscribe  
- JSON payload design with timestamps  

### Tasks
1. Publish telemetry to:
   ```
   cps/esp32/telemetry
   ```
2. Subscribe to:
   ```
   cps/esp32/cmd
   ```
3. Implement JSON command parsing:
   ```json
   {"led":1}
   {"sample_ms":200}
   ```

### Deliverables
- Arduino sketch  
- MQTT topic specification table  
- Node-RED flow OR MQTT explorer screenshots  

### Assessment Criteria
- Robust reconnection handling  
- Valid JSON data contract  
- Reliable command execution  

---

## Lab 4 — Cognition Layer  
### Local Intelligence at the Edge

**Goal**  
Add decision-making capability to the ESP32.

### Options (Instructor Choice)

#### Option A — Rule-Based Logic (Baseline)
- If `avg < threshold` and `trend ↓` → **DARKENING**

#### Option B — TinyML (Advanced)
- Train classifier (Bright / Normal / Dim)
- Deploy using TensorFlow Lite Micro (TFLM)

### Tasks
- Generate state label:
  ```
  BRIGHT | NORMAL | DIM
  ```
- Publish decision to:
  ```
  cps/esp32/state
  ```

### Deliverables
- Rule logic or TinyML model  
- Confusion matrix (TinyML) OR rule evaluation table  
- Latency and memory report (ms, RAM, flash)

### Assessment Criteria
- Stable decisions (no rapid flicker)  
- Measured inference or cycle time  
- Evidence-based tuning  

---

## Lab 5 — Configuration Layer  
### Closed-Loop Control & System Adaptation

**Goal**  
Implement safe, adaptive feedback control.

### Core Skills
- Finite state machine design  
- Safety constraints (hysteresis, rate limiting)  
- Manual override and fail-safe behavior  

### Tasks
1. Control actuator using cognition output  
2. Implement safety mechanisms:
   - Hysteresis band  
   - Minimum on/off duration  
   - Fail-safe state if MQTT disconnects  
3. Add override control:
   ```
   cps/esp32/override
   AUTO | MANUAL
   ```

### Deliverables
- Final closed-loop Arduino sketch  
- State machine diagram (PNG)  
- Test report covering:
  - Normal operation  
  - Noisy sensor input  
  - MQTT connection loss  

### Assessment Criteria
- Stable closed-loop behavior  
- No oscillation or chatter  
- Safe response under fault conditions  

---

## Learning Outcome Summary

After completing all five labs, students will be able to:
- Map CPS functionality to the 5C architecture  
- Build networked ESP32 CPS nodes  
- Perform signal processing and feature extraction  
- Deploy edge intelligence safely  
- Design resilient closed-loop CPS systems  

---

## License
Educational use only.
