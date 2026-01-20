# Introduction to Node-RED

## Overview
Node-RED is an open-source, flow-based programming tool designed for wiring together hardware devices, APIs, and online services. It provides a browser-based visual editor that allows users to build applications by connecting functional blocks, known as *nodes*, using simple drag-and-drop operations.

Node-RED is widely adopted in Internet of Things (IoT), Cyber-Physical Systems (CPS), smart city platforms, and rapid prototyping environments due to its low-code approach and real-time, event-driven execution model.

---

## Key Concepts

### Flow-Based Programming
Applications in Node-RED are created as **flows**, composed of interconnected **nodes**:
- **Input nodes** receive data from sensors, devices, or network services
- **Processing nodes** analyze data and implement decision logic
- **Output nodes** visualize information or control actuators and external systems

Each message is passed through the flow as a structured data object, enabling modular and reusable system design.

---

## Why Node-RED for Cyber-Physical Systems (CPS)?

Cyber-Physical Systems tightly integrate physical processes with cyber computation and control. Node-RED naturally supports CPS development through:

- Real-time sensor data acquisition
- Event-driven data processing
- Closed-loop feedback control
- Seamless integration of edge devices, networks, and dashboards

A typical CPS workflow is:
Sensor → Data Processing → Decision Logic → Actuator

Node-RED enables this workflow to be implemented visually and tested rapidly.

---

## Architecture Overview

A typical Node-RED deployment consists of:

1. **Physical Layer**  
   Sensors and actuators (e.g., ESP32, temperature sensors, relays)

2. **Communication Layer**  
   Protocols such as MQTT, HTTP, WebSocket, or TCP/IP

3. **Node-RED Runtime**  
   The execution engine that manages flows and message routing

4. **Application Layer**  
   Dashboards, databases, alerts, and cloud services

---

## Common Use Cases

- IoT monitoring and control
- Smart home and smart building automation
- Industrial monitoring and predictive maintenance
- CPS laboratory experiments
- Rapid prototyping of real-time systems

---

## Example Basic Flow

[MQTT Input] → [Function Node] → [Dashboard / MQTT Output]

This flow:
1. Receives sensor data via MQTT  
2. Processes the data using custom logic  
3. Displays results or controls actuators  

---

## Educational Benefits

Node-RED is particularly effective for education because:
- Minimal programming experience is required
- System behavior is easy to visualize
- Learners can focus on system-level thinking
- Supports key CPS concepts such as feedback, timing, and control

---

## Summary

Node-RED is a powerful and intuitive platform that bridges the gap between physical systems and digital intelligence. Its flow-based paradigm makes it especially suitable for developing, testing, and teaching Cyber-Physical Systems and IoT applications.

---

## Next Steps
- Connect Node-RED to an ESP32 using MQTT
- Build a real-time dashboard
- Implement a Sensor → Decision → Actuator CPS loop
