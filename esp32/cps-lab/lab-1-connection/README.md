# ESP32 5C CPS Lab Series

This lab series implements **one CPS laboratory per 5C layer** using an **ESP32**.

**5C Layers:** Connection → Conversion → Cyber → Cognition → Configuration

> Recommended simulator option: **Wokwi (ESP32 DevKit v1)**  
> Recommended broker: **Mosquitto** (local or cloud)

---

## Lab 1 — Connection Layer
### Sensor/Actuator Bring-Up on ESP32 (Physical I/O)

**Goal:** Interface the physical world using GPIO + ADC + PWM.

### Learning Outcomes
- Read an analog sensor via **ADC**
- Control an actuator via **GPIO**
- Generate **PWM** output for dimming/control

### Hardware (real board)
- ESP32 DevKit
- LDR + 10kΩ resistor (voltage divider) **or** potentiometer
- LED + 220Ω resistor

### Wiring (typical)
- LDR divider output → **GPIO34** (ADC1_CH6)
- LED anode → **GPIO2** through 220Ω, cathode → GND

### What you will build
- Read LDR ADC value (0–4095)
- Map ADC to LED brightness using PWM (LEDC)

### Run
1. Open `lab1_connection.ino` in Arduino IDE / PlatformIO
2. Select board: **ESP32 Dev Module**
3. Upload
4. Open Serial Monitor at **115200**

### Deliverables
- Source code (`lab1_connection.ino`)
- Screenshot of Serial Monitor showing ADC values
- Short note: observed range min/max in your environment

### Extension (optional)
- Add a push button to toggle **AUTO/MANUAL** brightness mode.
