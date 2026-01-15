# Chapter 3: Sensing and Data Acquisition in Cyber-Physical Systems (CPS)

## Chapter Overview
Cyber-Physical Systems (CPS) depend on sensors to capture the state of the physical world. Sensors convert physical phenomena (temperature, motion, pressure, light, vibration, biosignals) into measurable signals. This chapter introduces sensing principles, signal conditioning, data acquisition (DAQ), sampling, noise, calibration, and distributed sensing systems. A hands-on lab demonstrates integrating a physical sensor with an ESP32 and visualizing data in Node-RED.

## Learning Outcomes
After completing this chapter, students will be able to:
1. Classify sensor types used in CPS applications.
2. Explain the sensing chain: transduction, conditioning, digitization, and communication.
3. Apply sampling theory and identify aliasing and noise issues.
4. Understand calibration and major error sources in sensors.
5. Build a CPS pipeline using ESP32 sensors and a Node-RED dashboard.

## Key Terms
Transducer, signal conditioning, ADC, sampling rate, Nyquist theorem, quantization, noise, calibration, MQTT, Node-RED dashboard.

---

## The Role of Sensing in CPS
Sensors act as the *eyes and ears* of CPS by capturing physical state variables:

- **Environmental:** temperature, humidity, light, gas  
- **Mechanical:** acceleration, strain, pressure, vibration  
- **Biological:** heart rate, EEG, EMG  

Accurate sensing is critical, as sensing errors directly propagate into control and actuation decisions.

---

## The Sensing Chain
The sensing process consists of four main stages:

1. **Transduction:** physical stimulus → electrical signal  
2. **Signal Conditioning:** amplification, filtering, linearization  
3. **Digitization:** ADC converts analog signals to digital values  
4. **Communication:** data sent via I²C, SPI, UART, MQTT, etc.

**Physical → Transducer → Conditioning → ADC → Communication**

---

## Sampling and Quantization

### Sampling (Nyquist Theorem)
To avoid aliasing:
```
fs ≥ 2 × f_max
```
where `f_max` is the highest frequency in the signal.

### Quantization
An N-bit ADC provides `2^N` discrete levels.  
Quantization error ≈ ±½ LSB.

**Example:**  
12-bit ADC with 3.3 V reference:
```
Δ = 3.3 / 2^12 ≈ 0.8 mV
```

---

## Noise and Calibration
- **Noise:** thermal, EMI, quantization noise  
- **Filtering:** low-pass filters remove high-frequency noise  
- **Calibration:** aligns sensor output with reference standards  

Calibration is essential for accuracy and long-term stability.

---

## Distributed Sensing Systems
Modern CPS often use distributed sensor networks:
- Wireless nodes (Wi-Fi, BLE, LoRa)
- Time synchronization for coherence
- Data fusion (e.g., Kalman filters)

---

## Case Study: Inertial Sensing (MPU6050)
- Measures acceleration (ax, ay, az) and angular velocity (ωx, ωy, ωz)
- Used in robotics, drones, wearables
- Often combined with magnetometers for 9-DOF sensing

---

# Lab 3: Sensor Integration with ESP32 and Node-RED

## Lab Objectives
- Interface ESP32 with a physical sensor (LM73 or MPU6050)
- Publish sensor data using MQTT
- Visualize real-time data on Node-RED dashboard

## Required Hardware & Software
- ESP32 DevKit board
- LM73 (temperature) **or** MPU6050 (IMU)
- Node-RED with dashboard
- MQTT broker (e.g., Mosquitto)

---

## Lab Procedure

### 1. Hardware Wiring (I²C)
| ESP32 | Sensor |
|------|--------|
| 3.3V | VCC |
| GND  | GND |
| GPIO4 | SDA |
| GPIO5 | SCL |

### 2. ESP32 → MQTT
- Read sensor via I²C
- Publish JSON payload to topic:
```
/cps/sensors/data
```

### 3. Node-RED Flow
- **MQTT-in** → **JSON** → **Gauge + Chart**
- Display real-time sensor values

---

## Expected Output
- Live sensor value on gauge
- Time-series chart updating at 1 Hz
- Structured JSON in debug panel

---

## Assessment Rubric (10 points)
- Correct sensor wiring: 2  
- ESP32 publishes MQTT data: 3  
- Dashboard gauge & chart work: 3  
- Debug JSON structure correct: 2  

---

## Starter ESP32 Code (Arduino – Example)
```cpp
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

const char* WIFI_SSID = "YOUR_WIFI";
const char* WIFI_PASS = "YOUR_PASS";
const char* MQTT_HOST = "broker.hivemq.com";
const int MQTT_PORT = 1883;

WiFiClient espClient;
PubSubClient mqtt(espClient);

void wifiConnect(){
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while(WiFi.status()!=WL_CONNECTED) delay(300);
}

void mqttConnect(){
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  while(!mqtt.connected()){
    mqtt.connect("esp32-sensor");
    delay(500);
  }
}

void setup(){
  Serial.begin(115200);
  Wire.begin(4, 5);
  wifiConnect();
  mqttConnect();
}

void loop(){
  if(WiFi.status()!=WL_CONNECTED) wifiConnect();
  if(!mqtt.connected()) mqttConnect();

  int sensorVal = analogRead(34);
  char msg[64];
  snprintf(msg,sizeof(msg), "{\"value\":%d}", sensorVal);
  mqtt.publish("/cps/sensors/data", msg);

  mqtt.loop();
  delay(1000);
}
```

---

## Summary
Sensing and data acquisition form the first step of the CPS feedback loop. Challenges include sampling, noise, calibration, and distributed coordination. This lab demonstrates real sensor integration using ESP32 and Node-RED.

---

## Review Questions
1. List the four stages of the sensing chain.
2. What happens when Nyquist sampling is violated?
3. Explain quantization error with an example.
4. Why is calibration critical in CPS?

## Exercises
1. Extend the lab to publish multiple sensor values in JSON.
2. Add a moving-average filter in Node-RED.
3. Sketch a sensor fusion system for autonomous vehicles.

## Further Reading
- J. Wilson, *Sensor Technology Handbook*, Elsevier  
- H. Kopetz, *Real-Time Systems: Design Principles*, Springer  
- IEEE SMC, Sensor Fusion in CPS
