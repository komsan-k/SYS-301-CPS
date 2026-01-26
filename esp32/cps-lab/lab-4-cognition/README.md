# ESP32 5C CPS Lab Series

This package contains **one CPS lab per 5C layer** using an **ESP32**.

**5C Layers:** Connection → Conversion → Cyber → Cognition → Configuration

## Recommended tools
- ESP32 DevKit (or Wokwi ESP32 DevKit v1)
- Arduino IDE or PlatformIO
- MQTT broker (Mosquitto recommended)
- Node-RED (+ node-red-dashboard) for Labs 3–5

---

## Lab 4 — Cognition Layer
### Edge Decision-Making (Rule-Based “Intelligence”)

**Goal:** Produce a stable **decision label** from sensor data at the edge.

### Learning outcomes
- Turn filtered data into a discrete state
- Apply **hysteresis** to prevent chattering
- Publish cognition output to MQTT for monitoring and control

### States
- `DIM`
- `NORMAL`
- `BRIGHT`

### MQTT topic
- Publish: `cps/esp32/state`  
  Example: `{"device":"esp32-01","ts":123,"raw":1200,"filt":1188.2,"state":"DIM"}`

### Tasks
1. Filter raw ADC using EMA.
2. Compute a stable state using thresholds + hysteresis.
3. Publish state regularly (for dashboards) and on transitions.

### Node-RED flow
Import `node_red_lab4_flow.json` to visualize:
- Current state text
- Filt gauge
- Filt trend chart

### Deliverables
- `lab4_cognition_state.ino`
- Evidence of stable state transitions (logs/screenshots)
- Explain: why hysteresis is essential in CPS cognition
