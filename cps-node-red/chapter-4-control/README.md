# Chapter 4: Actuation and Control Interfaces in Cyber-Physical Systems (CPS)

## Chapter Overview
While sensors provide the *eyes and ears* of Cyber-Physical Systems (CPS), actuators are their *hands and muscles*, enabling CPS to influence the physical environment. This chapter introduces actuator types, interfacing methods, feedback control principles, and real-time constraints. A practical lab demonstrates cyber-to-physical actuation using Node-RED dashboards, MQTT, and ESP32.

## Learning Outcomes
After completing this chapter, students will be able to:
1. Classify different types of actuators.
2. Explain the actuation chain from cyber commands to physical devices.
3. Discuss feedback loops and timing constraints in actuation.
4. Implement Node-RED dashboards to control actuators via ESP32.
5. Recognize safety and reliability concerns in CPS actuation.

## Key Terms
Actuator, driver, PWM, H-bridge, servo, relay, feedback loop, latency, jitter, fail-safe.

---

## Actuator Types and Principles
- **Electrical:** LEDs, DC motors, stepper motors, servo motors  
- **Mechanical:** valves, pumps, robotic arms  
- **Electro-mechanical:** relays, solenoids  
- **Others:** piezoelectric, MEMS, pneumatic, hydraulic actuators  

*Example:* A servo motor uses PWM to control angular position.

---

## The Actuation Chain
Cyber Command → Interface & Driver → Actuator  

This chain converts software decisions into physical actions through drivers such as PWM generators, H-bridges, or relays.

---

## Feedback and Control Loops
Most CPS actuators operate in closed-loop systems:

```
u(t) = f(x_desired − x_measured)
```

Common controllers:
- **On/Off (Bang-Bang)** – thermostats  
- **Proportional (P)** – speed/position control  
- **PID** – industrial motor and process control  

---

## Real-Time Constraints in Actuation
- **Latency:** command must reach actuator before deadline  
- **Jitter:** timing variations can destabilize control  
- **Resolution:** discrete actuation steps (e.g., PWM duty cycle)

---

## Safety and Reliability
- Emergency stop and hardware interlocks  
- Watchdog timers  
- Fail-safe default states  
- Redundancy for critical actuators  

---

## Case Study: Smart Lighting Control
- **Sensor:** LDR measures ambient light  
- **Actuator:** LED dimmer via PWM  
- **Control:** Node-RED adjusts PWM based on light setpoint  

---

# Lab 4: Actuator Control Dashboard (Node-RED + ESP32)

## Lab Objectives
- Control actuators using Node-RED dashboard
- Send MQTT commands to ESP32
- Drive LED (ON/OFF) and servo motor (angle)

## Required Hardware & Software
- ESP32 DevKit
- LED + resistor
- Servo motor (SG90)
- Node-RED with MQTT broker (Mosquitto)

---

## Lab Procedure

### 1. Node-RED Dashboard
Create:
- **UI Switch** → topic `/cps/cmd/led`
- **UI Slider (0–180)** → topic `/cps/cmd/servo`
- Connect both to **MQTT-out** nodes

### 2. ESP32
- Subscribe to command topics
- Control LED and servo based on received MQTT messages

---

## Expected Output
- LED toggles ON/OFF from dashboard  
- Servo rotates according to slider position  
- Real-time cyber-to-physical actuation  

---

## Assessment Rubric (10 points)
- Dashboard UI elements: 2  
- MQTT commands sent correctly: 3  
- LED & servo respond correctly: 3  
- Debug payload verification: 2  

---

## Starter ESP32 Code (Arduino – Example)
```cpp
#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>
#include <ArduinoJson.h>

const char* WIFI_SSID = "YOUR_WIFI";
const char* WIFI_PASS = "YOUR_PASS";
const char* MQTT_HOST = "192.168.1.10";
const int MQTT_PORT = 1883;

const int LED_PIN = 2;
const int SERVO_PIN = 14;

Servo myServo;
WiFiClient espClient;
PubSubClient mqtt(espClient);

void callback(char* topic, byte* payload, unsigned int len){
  String data((char*)payload, len);
  if(String(topic)=="/cps/cmd/led"){
    digitalWrite(LED_PIN, data=="ON" ? HIGH : LOW);
  } else if(String(topic)=="/cps/cmd/servo"){
    StaticJsonDocument<128> doc;
    if(deserializeJson(doc, data) == DeserializationError::Ok){
      int angle = doc["angle"] | 90;
      myServo.write(constrain(angle,0,180));
    }
  }
}

void setup(){
  pinMode(LED_PIN, OUTPUT);
  myServo.attach(SERVO_PIN);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while(WiFi.status()!=WL_CONNECTED) delay(300);
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setCallback(callback);
  mqtt.connect("esp32-actuator");
  mqtt.subscribe("/cps/cmd/led");
  mqtt.subscribe("/cps/cmd/servo");
}

void loop(){
  mqtt.loop();
}
```

---

## Summary
Actuators allow CPS to affect the physical world through cyber commands, drivers, and feedback loops. Real-time constraints, reliability, and safety are essential. This lab demonstrates practical actuation using Node-RED, MQTT, and ESP32.

---

## Review Questions
1. List actuator categories with examples.
2. What is PWM and why is it used?
3. Why are latency and jitter critical in actuation?
4. Give an example of a fail-safe mechanism.

## Exercises
1. Add a buzzer actuator controlled via MQTT.
2. Implement a bang-bang LED controller using temperature data.
3. Sketch a PID motor control loop for CPS.

## Further Reading
- K. Ogata, *Modern Control Engineering*  
- D. E. Seborg et al., *Process Dynamics and Control*  
- H. K. Khalil, *Nonlinear Systems*
