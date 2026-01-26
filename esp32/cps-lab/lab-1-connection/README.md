# ESP32 5C CPS Lab Series

This package contains **one CPS lab per 5C layer** using an **ESP32**.

**5C Layers:** Connection → Conversion → Cyber → Cognition → Configuration

## Recommended tools
- ESP32 DevKit (or Wokwi ESP32 DevKit v1)
- Arduino IDE or PlatformIO
- MQTT broker (Mosquitto recommended)
- Node-RED (+ node-red-dashboard) for Labs 3–5

---

## Lab 1 — Connection Layer
### Sensor/Actuator Bring-Up on ESP32 (Physical I/O)

**Goal:** Interface the physical world using **ADC + GPIO + PWM**.

### Learning outcomes
- Read an analog sensor via **ESP32 ADC**
- Drive an LED via **GPIO**
- Use **LEDC PWM** for brightness control

### Hardware
- ESP32 DevKit
- LDR + 10kΩ resistor (voltage divider) **or** potentiometer
- LED + 220Ω resistor

### Wiring (typical)
- LDR divider output → **GPIO34** (ADC1_CH6)
- LED anode → **GPIO2** through 220Ω, cathode → GND

### Tasks
1. Read ADC (0–4095) at 10 Hz and print to Serial.
2. Map ADC to PWM duty (0–255) and set LED brightness.
3. (Optional) Invert mapping: **darker → brighter LED**.

### Run
1. Open `lab1_connection.ino`
2. Board: **ESP32 Dev Module**
3. Upload
4. Serial Monitor: **115200**

### Deliverables
- `lab1_connection.ino`
- Screenshot/log showing ADC and duty values
- Short note: observed min/max ADC in your environment

### Checkpoint questions
- Why is GPIO34 input-only?
- What does PWM resolution change?
