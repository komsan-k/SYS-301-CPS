# Lab 4: TinyML + MQTT + Node-RED CPS Dashboard (ESP32, Wokwi)

## Overview
This lab extends **Lab 3 (MQTT + Node-RED CPS Dashboard)** by integrating **TinyML-based decision making** into the CPS loop.

The ESP32 performs **on-device TinyML inference** (e.g., light condition classification),
publishes predictions and confidence values via MQTT, and Node-RED:
- Visualizes TinyML results
- Supports **manual control**
- Supports **automatic control driven by TinyML predictions**

This lab demonstrates an **intelligent, human-supervised CPS**.

---

## Learning Objectives
After completing this lab, students will be able to:
- Integrate TinyML inference with MQTT-based CPS
- Publish ML predictions and confidence scores
- Design Node-RED dashboards for intelligent CPS
- Implement auto-control logic based on ML outputs
- Analyze trade-offs between manual and autonomous CPS control

---

## CPS Mapping

| CPS Layer | Implementation |
|----------|----------------|
| Physical | LDR sensor, LED actuator |
| Cyber | ESP32 firmware |
| Intelligence | TinyML inference (on-device) |
| Decision | ML-driven + human override |
| Network | MQTT publish/subscribe |
| Interface | Node-RED Dashboard |

---

## System Architecture

```
LDR → ESP32 → TinyML Model → Prediction
                     ↓
                  MQTT Publish
                     ↓
               Node-RED Dashboard
              ↙                ↘
       Manual Control      Auto Control
              ↘                ↙
                 MQTT Command
                     ↓
                  ESP32 → LED
```

---

## Hardware Components (Simulated)
- ESP32 Dev Module (Wokwi)
- LDR (Light Sensor)
- 10kΩ Resistor
- LED

---

## Software Requirements
- Wokwi simulator → https://wokwi.com
- Node-RED
- Node-RED Dashboard
- Public MQTT broker:
  - `broker.hivemq.com`
  - Port `1883`

---

## MQTT Topics (Lab 4)

### ESP32 → Node-RED (Publish)
| Topic | Description |
|------|------------|
| `cps/lab4/ldr` | Raw LDR ADC value |
| `cps/lab4/pred` | TinyML prediction label |
| `cps/lab4/prob` | Prediction confidence (%) |
| `cps/lab4/led` | Current LED state |

### Node-RED → ESP32 (Subscribe)
| Topic | Payload | Description |
|------|--------|------------|
| `cps/lab4/led/set` | `ON` / `OFF` | Actuator command |

---

## TinyML Model (Concept)
- Input feature: normalized LDR value
- Output classes: `DARK`, `BRIGHT`
- Model: small neural network / decision tree
- Deployment: TensorFlow Lite for Microcontrollers (TFLM)

> ⚠️ Model training is performed offline (Edge Impulse or Python),
> inference runs fully on ESP32.

---

## ESP32 Firmware (Conceptual Flow)
1. Read LDR sensor
2. Extract features
3. Run TinyML inference
4. Publish:
   - Raw sensor value
   - Prediction label
   - Confidence score
5. Receive MQTT command (manual or auto)
6. Control LED safely

---

## Node-RED Dashboard Features
- Gauge + chart for sensor monitoring
- Text widgets for prediction and LED status
- Confidence gauge for ML reliability
- Switch for **Manual LED Control**
- Switch for **Auto Control (TinyML-driven)**

---

## Experiments

### Experiment 1: ML Monitoring
- Observe how predictions change with light conditions
- Analyze confidence values

### Experiment 2: Manual Override
- Disable auto control
- Toggle LED manually from dashboard

### Experiment 3: Auto Control
- Enable auto control
- Observe LED behavior driven by TinyML predictions

---

## Expected Results
- Real-time visualization of ML predictions
- LED responds automatically to TinyML inference
- Manual override works at all times
- Stable CPS behavior under changing environment

---

## Discussion Questions
1. Why is on-device TinyML important for CPS latency and privacy?
2. What risks arise from incorrect ML predictions?
3. How can confidence thresholds improve safety?
4. When should human override be mandatory?

---

## Extension (Optional)
- Add confidence-based gating
- Integrate multiple sensors (sensor fusion)
- Log ML decisions for Digital Twin analysis
- Apply anomaly detection TinyML models

---

## Outcome
This lab demonstrates the evolution:
**CPS → Networked CPS → Visual CPS → Intelligent CPS**

Students are now prepared for:
- Edge AI systems
- Autonomous CPS
- Digital Twin and Smart Infrastructure

---

**Author**  
Dr. Komsan Kanjanasit  
College of Computing, Prince of Songkla University
