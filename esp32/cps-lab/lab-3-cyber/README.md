# ESP32 5C CPS Lab Series

This package contains **one CPS lab per 5C layer** using an **ESP32**.

**5C Layers:** Connection → Conversion → Cyber → Cognition → Configuration

## Recommended tools
- ESP32 DevKit (or Wokwi ESP32 DevKit v1)
- Arduino IDE or PlatformIO
- MQTT broker (Mosquitto recommended)
- Node-RED (+ node-red-dashboard) for Labs 3–5

---

## Lab 3 — Cyber Layer
### ESP32 Networking Node (Wi‑Fi + MQTT + JSON)

**Goal:** Make ESP32 a **networked CPS node** that publishes telemetry and receives commands.

### Learning outcomes
- Connect to Wi‑Fi
- Publish telemetry via **MQTT**
- Subscribe to commands and parse **JSON**
- Define a clear topic + payload contract

### MQTT topics
| Direction | Topic | Example Payload |
|---|---|---|
| Publish | `cps/esp32/telemetry` | `{"device":"esp32-01","ts":123,"raw":1234,"filt":1200.5}` |
| Subscribe | `cps/esp32/cmd` | `{"led":1}` or `{"sample_ms":200}` |

### Requirements
- Arduino library: **PubSubClient** (Nick O'Leary)
- MQTT broker (Mosquitto):
  - Local PC/RPi, or VM, or cloud broker

### Tasks
1. Publish telemetry every `sample_ms`.
2. Subscribe to commands:
   - `{"led":0/1}` toggles LED
   - `{"sample_ms":N}` adjusts publish period (20..5000 ms)
3. Implement reconnection logic.

### Node-RED flow
Import `node_red_lab3_flow.json` to get:
- Telemetry chart (filt/raw)
- LED switch + sample_ms control

### Deliverables
- `lab3_cyber_mqtt.ino`
- Node-RED screenshot OR MQTT Explorer screenshot
- Short note: why a data contract matters in CPS/IoT
