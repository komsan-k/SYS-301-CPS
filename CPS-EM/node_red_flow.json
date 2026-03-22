# Full Node-RED Dashboard UI for ESP32 RSSI Digital Twin

This package includes a complete Node-RED dashboard flow with:

- MQTT input from topic `esp32/rssi`
- RSSI gauge
- Live RSSI time-series chart
- Quality and alert text indicators
- Average / minimum / maximum / trend analytics
- Signal-state distribution bar chart
- Demo data injector for testing without ESP32

## Expected payload
The flow accepts either:
- a raw number, for example `-67`
- or JSON, for example `{"rssi": -67}`

## Import steps
1. Open Node-RED.
2. Menu -> Import -> select `node_red_dashboard_rssi_twin.json`
3. Deploy the flow.
4. Open the dashboard.

## Required Node-RED packages
Install:
- `node-red-dashboard`

## MQTT topic
Publish ESP32 RSSI data to:
`esp32/rssi`

## Notes
- The included demo injector sends simulated RSSI values every 2 seconds.
- When using real ESP32 data, disable the demo injector node.
