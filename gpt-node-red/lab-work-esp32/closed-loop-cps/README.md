# Closed-Loop CPS (Student Guide)
## ESP32 Sensor → Node-RED Rules → MQTT Actuator → ESP32 LED + GPT Explanation + Dashboard

This lab builds a **closed-loop Cyber-Physical System (CPS)**.

**Pipeline**
```
ESP32 (LDR sensor) → MQTT → Node-RED (RULES) → MQTT → ESP32 (LED)
                         ↘ GPT Summary (EXPLAIN ONLY) → Dashboard
```

✅ **Important rule:**  
- **Node-RED rules control the LED** (safe + deterministic)  
- **GPT only explains** what is happening (GPT does NOT control hardware)

---

## 1) What You Need

### Hardware
- ESP32 board
- LDR + 10kΩ resistor (or a potentiometer)
- LED (use built-in LED if your ESP32 has one, often GPIO2)  
  *If you use an external LED: add a 220–330Ω resistor.*

### Software
- Arduino IDE
- MQTT broker (Mosquitto)
- Node-RED
- Node-RED Dashboard (`node-red-dashboard`)
- ChatGPT node (`node-red-contrib-custom-chatgpt`) + OpenAI API key

---

## 2) Topics (MQTT Channels)

### Sensor topic (ESP32 → Node-RED)
```
cps/esp32/sensor
```

Example message:
```json
{"device":"esp32-01","ts":1700000000,"ldr":623}
```

### Actuator topic (Node-RED → ESP32)
```
cps/esp32/actuator
```

Example message:
```json
{"device":"esp32-01","led":1,"reason":"Rule: LDR below threshold"}
```

- `led = 1` → LED ON  
- `led = 0` → LED OFF  

---

## 3) What the ESP32 Does

ESP32 has **two jobs**:
1. **Publish sensor** value (LDR) to `cps/esp32/sensor`
2. **Subscribe actuator** commands from `cps/esp32/actuator` and control LED

You only need to edit in the code:
- Wi-Fi name and password
- MQTT broker IP address
- (optional) LED pin

---

## 4) What Node-RED Does

Node-RED has **three jobs**:

### (A) Show live sensor data
- Gauge = latest LDR value  
- Chart = LDR trend (history)

### (B) Make a safe decision (RULES)
A **rule decision** turns LED ON/OFF using thresholds:

- If **LDR < TH_LOW** → LED ON  
- If **LDR > TH_HIGH** → LED OFF  

This is called **hysteresis** and it prevents flickering when values bounce near the threshold.

### (C) Ask GPT to explain (EXPLAIN ONLY)
GPT generates a **1–2 sentence explanation** like:
> “Light is decreasing and the system turned the LED ON using the threshold rule.”

GPT is **not allowed to control** the LED.  
Only rules do that.

---

## 5) Run the Lab (Step-by-step)

### Step 1: Start MQTT Broker
Make sure Mosquitto is running on your broker device/PC.

### Step 2: Upload ESP32 Code
- Open Arduino IDE
- Open `esp32_closed_loop_mqtt.ino`
- Edit Wi-Fi + broker IP
- Upload
- Open Serial Monitor (115200)

You should see sensor messages and actuator messages.

### Step 3: Import Node-RED Flow
- Open Node-RED
- Menu → **Import**
- Import `node_red_closed_loop_flow.json`
- Set MQTT broker IP if needed
- Open GPT node and set your OpenAI API key
- Deploy

### Step 4: Open Dashboard
Open:
```
http://<node-red-host>:1880/ui
```

---

## 6) What You Should See

- ✅ Gauge: current LDR value
- 📈 Chart: LDR trend
- 🟦 Rule Decision text: shows why LED changed
- 🧠 GPT Explanation: short explanation in simple words
- 💡 LED turns ON/OFF automatically based on rules

---

## 7) Tune the Thresholds (Optional)

In the Node-RED function node **Rule Decision**, adjust:
- `TH_LOW`
- `TH_HIGH`

Example:
- `TH_LOW = 1200`
- `TH_HIGH = 1800`

If your environment is brighter/darker, you may need different values.

---

## 8) Safety Notes (Very Important)

- Control must be **deterministic** (rules)
- GPT can be wrong, so GPT must only **explain**
- Never use AI to directly control safety-critical devices

---

## 9) Mini Challenges (Optional)

Choose 1:
1. Add a second actuator (buzzer) using another rule
2. Replace LDR with a temperature sensor
3. Add a “manual override” switch on dashboard (still rule-based)

---

**Course**: Cyber-Physical Systems  
**Instructor**: Asst. Prof. Komsan Kanjanasit, PhD  
Prince of Songkla University (Phuket)
