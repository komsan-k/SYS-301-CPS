# ESP32 5C CPS Lab Series

This package contains **one CPS lab per 5C layer** using an **ESP32**.

**5C Layers:** Connection → Conversion → Cyber → Cognition → Configuration

## Recommended tools
- ESP32 DevKit (or Wokwi ESP32 DevKit v1)
- Arduino IDE or PlatformIO
- MQTT broker (Mosquitto recommended)
- Node-RED (+ node-red-dashboard) for Labs 3–5

---

## Lab 2 — Conversion Layer
### Signal Conditioning + Feature Extraction (Sensor → Data)

**Goal:** Convert raw sensor readings into **filtered signals** and **features**.

### Learning outcomes
- Implement a moving-average style filter (EMA / MA)
- Calibrate sensor range (min/max)
- Compute window features: **avg**, **std**, **trend**

### Inputs/Outputs
- Input: ADC readings from GPIO34
- Output: CSV-style Serial logs for plotting

### What you will build
Every `WINDOW_MS`, compute:
- `raw` latest ADC
- `filt` filtered value
- `avg` mean of window
- `std` standard deviation of window
- `trend` last - first (direction of change)

### Run
1. Upload `lab2_conversion.ino`
2. Serial Monitor **115200**
3. Copy output to CSV (optional)

### Deliverables
- `lab2_conversion.ino`
- (Recommended) One plot: raw vs filtered
- Short explanation: how filtering improves CPS stability

### Checkpoint questions
- Why does a larger window increase delay?
- What does `std` tell you about sensor noise?
