
# ESP32 → Node-RED → GPT Dashboard (Student Guide)

This lab shows how a **real sensor** can be connected to **AI** and a **web dashboard**.

**Pipeline**
```
ESP32 (Sensor) → MQTT → Node-RED → GPT Summary → Dashboard
```

You will:
- See **live sensor values** on a dashboard
- See a **short AI-generated explanation** of the sensor data
- Learn how CPS (Cyber-Physical Systems) work end-to-end

---

## 1. What You Need

### Hardware
- ESP32 board
- LDR (light sensor) + 10kΩ resistor  
  *(or a potentiometer for testing)*

### Software
- Arduino IDE
- MQTT broker (Mosquitto)
- Node-RED
- Node-RED Dashboard (`node-red-dashboard`)
- ChatGPT node (`node-red-contrib-custom-chatgpt`)

---

## 2. Sensor Data Format

The ESP32 sends data using **MQTT**.

### Topic
```
cps/esp32/sensor
```

### Message (JSON)
```json
{
  "device": "esp32-01",
  "ts": 1700000000,
  "ldr": 623
}
```

- `ldr` = light level (0–4095)
- Higher value → brighter light

---

## 3. What the ESP32 Does

The ESP32:
1. Connects to Wi-Fi
2. Reads the LDR sensor
3. Sends the value to MQTT every 2 seconds

You **do not need to change the code**, except:
- Wi-Fi name
- Wi-Fi password
- MQTT broker IP address

---

## 4. What Node-RED Does

Node-RED:
- Receives sensor data from MQTT
- Shows the value on a **gauge**
- Draws a **line chart**
- Sends recent values to **GPT**
- Shows a **1–2 sentence explanation**

Example GPT output:
> “The light level is decreasing, indicating a darker environment.”

---

## 5. Run the Lab

### Step 1: Upload ESP32 Code
- Open Arduino IDE
- Upload the provided ESP32 sketch

### Step 2: Import Node-RED Flow
- Open Node-RED
- Menu → **Import**
- Paste the provided flow JSON
- Set:
  - MQTT broker IP
  - OpenAI API key (in ChatGPT node)

### Step 3: Open Dashboard
Open your browser:
```
http://<node-red-host>:1880/ui
```

---

## 6. What You Should See

- ✅ Gauge: current light value  
- 📈 Chart: light level over time  
- 🧠 GPT Summary: short explanation in text  

---

## 7. Important Notes

- GPT **does NOT control** the hardware
- GPT is used only to **explain sensor data**
- Control logic should always be **rule-based and safe**

---

## 8. Learning Outcomes

After this lab, you can:
- Explain how a CPS works
- Use MQTT for IoT communication
- Build a Node-RED dashboard
- Use AI to summarize sensor data

---

## 9. Challenge (Optional)

Try one:
- Replace LDR with a temperature sensor
- Change the GPT prompt to explain trends
- Add LED control using simple rules (no AI)

---

**Course**: Cyber-Physical Systems  
**Instructor**: Asst. Prof. Komsan Kanjanasit, PhD  
Prince of Songkla University (Phuket)
