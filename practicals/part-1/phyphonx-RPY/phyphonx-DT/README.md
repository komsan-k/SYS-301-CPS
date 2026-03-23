
# phyphox → Node-RED Digital Twin

This package contains a Node-RED flow that polls the phyphox Remote Access API and displays Roll, Pitch, and Yaw on a dashboard.

## Files
- `phyphox_node_red_digital_twin.json`

## Before use
1. Open your phyphox experiment on the phone.
2. Enable **Allow Remote Access**.
3. Note the phone URL, for example `http://192.168.1.15:8080`.
4. In the Function node **Set phyphox URL**, replace the IP address.
5. If your experiment uses different buffer names, replace `roll`, `pitch`, and `yaw` in the URL and in the extract function.

## Flow
Inject → HTTP Request → Extract latest RPY → Gauges + Charts + JSON

## URL used
`/get?roll=full&pitch=full&yaw=full`

## Notes
- Phone and Node-RED must be on the same network.
- If the endpoint returns different names, inspect the raw JSON once and adjust the function.
