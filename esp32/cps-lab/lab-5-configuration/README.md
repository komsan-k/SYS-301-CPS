# ESP32 5C CPS Lab Series

This package contains **one CPS lab per 5C layer** using an **ESP32**.

**5C Layers:** Connection → Conversion → Cyber → Cognition → Configuration

## Recommended tools
- ESP32 DevKit (or Wokwi ESP32 DevKit v1)
- Arduino IDE or PlatformIO
- MQTT broker (Mosquitto recommended)
- Node-RED (+ node-red-dashboard) for Labs 3–5

---

## Lab 5 — Configuration Layer
### Closed-Loop Control (Safe Actuation + Manual Override)

**Goal:** Implement **closed-loop configuration** with safety constraints.

### Learning outcomes
- Apply cognition decisions to actuation deterministically
- Implement safety: **minimum dwell time**, **fail-safe**, **override**
- Use Node-RED for manual override and monitoring

### MQTT topics
| Topic | Direction | Example |
|---|---|---|
| `cps/esp32/state` | Sub | `{"state":"DIM"}` |
| `cps/esp32/override` | Sub | `{"mode":"AUTO"}` or `{"mode":"MANUAL","led":1}` |
| `cps/esp32/actuator` | Pub | `{"mode":"AUTO","led":1,"reason":"DIM"}` |

### Safety rules
- **Minimum dwell time:** once LED changes, hold for `MIN_DWELL_MS`
- **Timeout fail-safe:** if no state received for `STATE_TIMEOUT_MS` → safe OFF
- **Bounds checking:** reject invalid values

### Tasks
1. Subscribe to `state` and `override`.
2. In AUTO:
   - DIM → LED ON
   - BRIGHT/NORMAL → LED OFF
3. In MANUAL:
   - Use override LED value directly.
4. Publish actuator status for dashboard.

### Node-RED flow
Import `node_red_lab5_flow.json` to get:
- AUTO/MANUAL dropdown
- Manual LED switch
- Actuator status widgets (mode/led/reason)

### Deliverables
- `lab5_configuration_closed_loop.ino`
- Logs/screenshots of:
  1) Normal operation
  2) Broker disconnect (fail-safe)
  3) Manual override
- State/Mode diagram (simple PNG or ASCII ok)
