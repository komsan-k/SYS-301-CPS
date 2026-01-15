# Chapter 5: Communication and Networking in Cyber-Physical Systems (CPS)

## Chapter Overview
Communication networks form the *nervous system* of Cyber-Physical Systems (CPS). They connect sensors, actuators, controllers, and human interfaces across heterogeneous platforms and scales. This chapter introduces networking principles, CPS-specific requirements (latency, reliability, scalability, security), and key protocols used in IoT-enabled CPS. A practical lab demonstrates an MQTT-based CPS pipeline using ESP32 and Node-RED.

## Learning Outcomes
After completing this chapter, students will be able to:
1. Explain the role of communication in CPS closed-loop operation.
2. Describe wired and wireless communication options for CPS.
3. Compare IoT protocols (MQTT, CoAP, OPC UA, BLE, LoRa).
4. Identify networking challenges in CPS (latency, jitter, reliability, scalability).
5. Implement a distributed MQTT-based CPS network with Node-RED.

## Key Terms
Latency, jitter, bandwidth, Quality of Service (QoS), MQTT, CoAP, OPC UA, BLE, LoRa, publish/subscribe, broker.

---

## The Role of Communication in CPS
In CPS, sensing and actuation are connected by communication channels. End-to-end timing can be expressed as:

```
D = d_sense + d_net + d_compute + d_act
```

Network delay (`d_net`) is often the dominant factor affecting stability, safety, and performance.

---

## Wired vs. Wireless Communication

### Wired Communication
Examples: Ethernet, CAN bus, Modbus  
- Advantages: deterministic latency, high reliability  
- Limitations: limited mobility, higher installation cost  

### Wireless Communication
Examples: Wi-Fi, BLE, ZigBee, LoRa, 5G  
- Advantages: mobility, scalability, flexible deployment  
- Limitations: interference, variable latency, power constraints  

---

## IoT Protocols for CPS
- **MQTT:** lightweight publish/subscribe protocol, widely used in CPS and IoT  
- **CoAP:** RESTful protocol over UDP for constrained devices  
- **OPC UA:** industrial-grade interoperability with rich semantics and security  
- **BLE:** low-power, short-range communication  
- **LoRa / LoRaWAN:** long-range, low-data-rate communication for wide-area CPS  

---

## Networking Challenges in CPS
- **Latency & Jitter:** must satisfy real-time deadlines  
- **Reliability:** packet loss can cause unsafe system states  
- **Scalability:** support for hundreds or thousands of nodes  
- **Security:** authentication, encryption, and intrusion detection  

---

## Case Study: Smart Grid Communication
- Smart meters → edge gateways (ZigBee / LoRa)  
- Gateways → utility control centers (LTE / 5G)  
- Control decisions → actuators (inverters, circuit breakers)  

Typical requirements: sub-100 ms latency, high reliability, strong security.

---

# Lab 5: MQTT-Based CPS Network (ESP32 + Node-RED)

## Lab Objectives
- Set up an MQTT broker (Mosquitto)
- Publish sensor data from ESP32
- Subscribe to and visualize data in Node-RED

## Required Hardware & Software
- ESP32 board with sensor (LM73 or simulated input)
- MQTT broker (Mosquitto)
- Node-RED with dashboard nodes

---

## Lab Procedure

### 1. MQTT Broker
Install and start Mosquitto on your PC or server.

### 2. ESP32 Publisher
- Read sensor value
- Publish JSON payload to topic:
```
/cps/temp/room
```

### 3. Node-RED Subscriber
- **MQTT-in** → subscribe to `/cps/temp/room`
- **JSON** → parse payload
- **Gauge + Chart** → visualize data

---

## Expected Output
- Real-time temperature displayed on gauge
- Streaming temperature trend on chart

---

## Assessment Rubric (10 points)
- MQTT broker running: 2  
- ESP32 publishes data: 3  
- Node-RED dashboard works: 3  
- Debug payload verification: 2  

---

## Summary
Communication networks are central to CPS operation. Latency, jitter, reliability, and security determine system safety and performance. MQTT offers a scalable publish/subscribe model well suited for CPS, demonstrated through an ESP32–Node-RED lab.

---

## Review Questions
1. Compare wired and wireless communication in CPS.
2. How does MQTT differ from CoAP?
3. Why are latency and jitter critical in CPS control loops?
4. What is the role of an MQTT broker?

## Exercises
1. Extend the lab to support two ESP32 sensor nodes with different topics.
2. Experiment with MQTT QoS levels (0, 1, 2).
3. Propose security mechanisms for MQTT-based CPS.

## Further Reading
- A. Banks, R. Gupta, *MQTT Version 3.1.1*, OASIS  
- OPC Foundation, *OPC Unified Architecture Specification*  
- E. Kovacs, *Standards and IoT Protocols for CPS*  
- H. Kopetz, *Real-Time Systems: Design Principles*
