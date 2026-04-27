# 📘 Lab 5: TFLM - LDR Light Classification on ESP32

## 🎯 Objectives
- Understand TinyML inference on microcontrollers
- Deploy a trained model using EloquentTinyML
- Perform real-time sensor classification
- Implement a Cyber-Physical System (CPS) loop
- Measure inference latency on ESP32

## 🧠 Concepts Covered
- TinyML (Edge AI)
- TensorFlow Lite for Microcontrollers (TFLM)
- Analog sensor acquisition (ADC)
- Normalization & feature scaling
- Classification (Softmax output)
- CPS 5C Architecture

## 🧰 Hardware Requirements
- ESP32 DevKit
- LDR sensor
- Resistor (10kΩ)
- Breadboard
- Jumper wires

## 🔌 Circuit Diagram
3.3V ---- LDR ----+---- GPIO36 (ADC)
                  |
                 10kΩ
                  |
                 GND

## ⚙️ Software Requirements
- Arduino IDE / PlatformIO
- ESP32 Board Package
- Libraries:
  - EloquentTinyML
  - tflm_esp32

## 🧪 Experiment Overview
1. Read LDR analog value
2. Normalize input
3. Run TinyML inference
4. Output classification (Dark / Normal / Bright)

## 🧩 CPS Mapping (5C Architecture)
- Connection: LDR → ESP32 ADC
- Conversion: ADC → normalized value
- Cyber: TinyML inference
- Cognition: Class prediction
- Configuration: Serial output

## 🔍 Key Code Steps
- Read ADC: `analogRead(36)`
- Normalize: `x = 1 - (adc / 4095.0)`
- Predict: `tf.predict(input)`
- Output: `tf.classification`

## 📊 Expected Output
Input: 0.9123  
Predicted class: 1 (Normal)  
Inference time: ~400 µs  

## 🧪 Experiment Tasks
### Task 1: Basic Testing
- Upload code
- Observe serial output

### Task 2: Light Variation
- Cover LDR → Dark
- Indoor light → Normal
- Flashlight → Bright

### Task 3: Performance
- Measure inference time

### Task 4: Noise Analysis
- Change light rapidly
- Observe stability

## 📈 Data Recording Table
| Trial | ADC | Normalized | Class | Time (µs) |
|------|-----|-----------|------|----------|
| 1 | | | | |
| 2 | | | | |

## 🧠 Analysis Questions
1. Why normalize input?
2. What if not normalized?
3. TinyML vs Cloud ML?
4. Why is latency important?
5. Causes of misclassification?

## ⚠️ Common Issues
- Library missing → install TFLM
- Wrong classification → check wiring
- Crash → increase arena size

## 🚀 Extensions
- Add LED control
- Add filtering
- Integrate MQTT + Node-RED
- Build Digital Twin

## 📝 Lab Report Template
1. Objective
2. Methodology
3. Results
4. Analysis
5. Conclusion

## 📚 Learning Outcomes
- Deploy ML on embedded systems
- Understand CPS + TinyML
- Build real-time AI systems

## 🧭 Next Lab
Lab 6: TinyML + MQTT + Node-RED Digital Twin
