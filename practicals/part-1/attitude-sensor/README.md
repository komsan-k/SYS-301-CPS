# phyphox Attitude → Node-RED Dashboard + MQTT

This flow uses the phyphox Attitude (Euler) experiment and polls:

`http://172.19.177.163:8080/get?roll=full&pitch=full&yaw=full`

## Important
Use a colon before the port:
- Correct: `http://172.19.177.163:8080/`
- Wrong: `http://172.19.177.163/8080/`

## What it does
- Polls phyphox every 0.5 s
- Extracts latest roll/pitch/yaw
- Shows 3 gauges and 3 charts
- Publishes JSON to MQTT topic:
  - `cps/lab/attitude/rpy`

## Requirements
- phyphox experiment: **Attitude with Euler angles**
- Remote Access enabled on the phone
- Node-RED dashboard nodes installed
- MQTT broker running at `127.0.0.1:1883` (change in broker config if needed)

## If it still fails
Open in browser first:
- `http://172.19.177.163:8080/`
- `http://172.19.177.163:8080/get`

If `/get?roll=full&pitch=full&yaw=full` fails, inspect the returned JSON and confirm the buffer names are really `roll`, `pitch`, and `yaw`.
