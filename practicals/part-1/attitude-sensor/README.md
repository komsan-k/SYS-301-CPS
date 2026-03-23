# 📡 phyphox → REST API (Live RPY Data)

This guide explains how to stream **real-time Roll, Pitch, and Yaw (RPY)** data from your smartphone using the phyphox app via its built-in REST API.

---

## 🌐 1. Data Endpoint

If your phone IP is:

```
172.19.177.163
```

Use:

```
http://172.19.177.163:8080/get?yaw&pitch&roll
```

---

## 🚀 2. How to Use

### Step 1: Open Experiment
- Open **phyphox**
- Load **Attitude with Euler angles**

### Step 2: Enable Remote Access
- Tap **⋮ (menu)** → **Allow Remote Access**

### Step 3: Request Data
Open in browser (same WiFi):

```
http://172.19.177.163:8080/get?yaw&pitch&roll
```

---

## 📊 3. Example JSON Response

```json
{
  "buffer": {
    "yaw":   { "buffer": [342.5] },
    "pitch": { "buffer": [-1.2] },
    "roll":  { "buffer": [5.7] }
  },
  "status": true
}
```

---

## 🧠 4. Data Meaning

| Parameter | Description |
|----------|------------|
| roll  | Rotation around X-axis |
| pitch | Rotation around Y-axis |
| yaw   | Heading (Z-axis) |

---


## ⚠️ Notes

- Use **:8080**
- Same WiFi required
- Use **Attitude (Euler)** only
- Do NOT use Gyroscope

---

## 🎯 Summary

```
phyphox (Attitude)
        ↓
REST API (/get)
        ↓
JSON (roll, pitch, yaw)
        ↓
Node-RED / Python / Dashboard
```
---
---


# phyphox Attitude → Node-RED Dashboard + MQTT

This flow uses the phyphox Attitude (Euler) experiment and polls:

`http://172.xx.xxx.xxx/get?roll&pitch&yaw`


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
- `http://172.19.177.163`
- `http://172.19.177.163/get?yaw&pitch&roll`

If `/get?roll=full&pitch=full&yaw=full` fails, inspect the returned JSON and confirm the buffer names are really `roll`, `pitch`, and `yaw`.
