# ESP32 Sensor → Node-RED → GPT Summary → Dashboard (Simple Working Pack)

This pack gives you a minimal **end-to-end CPS demo**:

**ESP32 (LDR sensor) → MQTT → Node-RED → GPT summary → Node-RED Dashboard**

## 1) Requirements
- ESP32 (any dev board)
- LDR sensor (or a potentiometer for testing)
- MQTT broker (e.g., Mosquitto)
- Node-RED
- Node-RED Dashboard (`node-red-dashboard`)
- Custom ChatGPT node (`node-red-contrib-custom-chatgpt`) + OpenAI API key

## 2) ESP32 wiring (LDR)
Typical divider:
- 3.3V → LDR → (ADC pin) → 10kΩ → GND
- ADC pin example in code: **GPIO34**

## 3) ESP32 code
Open `esp32_ldr_mqtt_publish.ino` in Arduino IDE.
Edit:
- `YOUR_WIFI`, `YOUR_PASS`
- `mqtt_server` (your broker IP)

Uploads and publishes to topic:
- `cps/esp32/sensor`

Payload example:
```json
{"device":"esp32-01","ts":123,"ldr":623}
```

## 4) Node-RED flow
Import `node_red_flow_esp32_gpt_dashboard.json` in Node-RED.

### After import, configure:
1) MQTT broker node:
- Change broker from `127.0.0.1` to your broker IP if needed.

2) GPT node:
- Open **GPT Summary** node and set your OpenAI API key in the node config.

## 5) Open Dashboard
Go to:
- `http://<node-red-host>:1880/ui`

You will see:
- LDR gauge
- LDR chart
- GPT summary text

## 6) Troubleshooting
- No MQTT data: check ESP32 serial output + broker IP + topic `cps/esp32/sensor`
- Dashboard empty: ensure `node-red-dashboard` installed and open `/ui`
- GPT not updating: confirm API key and that your model name in `msg.topic` works with your node config.

