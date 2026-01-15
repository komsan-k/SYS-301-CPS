# Chapter 7: Prototyping Systems in Cyber-Physical Systems (CPS)

## Chapter Overview
Prototyping is a critical phase in Cyber-Physical Systems (CPS) engineering. It bridges theoretical models and final deployments, allowing engineers to validate concepts, test interactions between cyber and physical components, and iterate designs rapidly. This chapter covers prototyping methodologies, hardware/software platforms, hardware-in-the-loop (HIL) testing, and simulation tools. A practical lab demonstrates building a hybrid CPS prototype using Node-RED and ESP32.

## Learning Outcomes
After completing this chapter, students will be able to:
1. Explain the role of prototyping in CPS design workflows.
2. Identify common hardware and software platforms for CPS prototyping.
3. Distinguish between rapid prototyping and hardware-in-the-loop (HIL) testing.
4. Integrate real and simulated components into a CPS prototype.
5. Implement a Node-RED flow that synchronizes physical and virtual CPS states.

## Key Terms
Prototyping, rapid prototyping, hardware-in-the-loop (HIL), co-simulation, Model-in-the-Loop (MIL), Software-in-the-Loop (SIL), real-time testing.

---

## The Role of Prototyping in CPS
CPS are complex and often safety-critical. Prototyping reduces risk by validating design decisions early:
- Rapid exploration of architectures
- Validation of sensing, actuation, and networking
- Early detection of integration issues
- Iterative, user-centered design

---

## Prototyping Methodologies

### Model-in-the-Loop (MIL)
Mathematical models tested entirely in simulation (e.g., Simulink, Modelica).

### Software-in-the-Loop (SIL)
Controller software tested with a simulated plant.

### Hardware-in-the-Loop (HIL)
Real hardware controllers interact with a simulated environment, enabling safe testing of extreme scenarios.

### Rapid Prototyping
Flexible platforms (ESP32, Arduino, Raspberry Pi, Node-RED) enable fast CPS experimentation.

---

## Hardware Platforms for CPS Prototyping
- **Microcontrollers:** Arduino, ESP32  
- **Single-board computers:** Raspberry Pi  
- **FPGAs:** deterministic real-time control and acceleration  
- **Industrial controllers:** PLCs  

---

## Software Platforms and Tools
- **Node-RED:** flow-based CPS/IoT prototyping  
- **MATLAB/Simulink:** plant–controller co-simulation  
- **Modelica:** object-oriented physical modeling  
- **Docker/Kubernetes:** scalable cyber-service deployment  

---

## Hardware-in-the-Loop (HIL) Prototyping
- Real controllers + simulated environments
- Safe validation of dangerous or rare scenarios
- Example: EV battery controller tested against simulated battery model

---

## Case Study: Smart Traffic Light Prototype
- **Virtual:** Node-RED simulates vehicle arrivals  
- **Physical:** ESP32 drives real LEDs  
- **Integration:** MQTT synchronizes virtual and physical states  

---

# Lab 7: Hardware-in-the-Loop CPS Prototype

## Lab Objectives
- Build a hybrid CPS prototype (simulated + physical)
- Synchronize simulated sensor data with real actuators
- Compare virtual and physical CPS states

## Required Hardware & Software
- ESP32 with LED (actuator)
- Node-RED with dashboard
- MQTT broker (Mosquitto)
- Simulated sensors (Node-RED inject nodes)

---

## Lab Procedure

### Node-RED
- Inject node simulates sensor data
- Function node publishes to `/cps/sensors/real`
- MQTT-out node sends commands to `/cps/cmd/led`
- Dashboard chart compares simulated vs. real values

### ESP32
- Subscribes to `/cps/cmd/led`
- Controls LED ON/OFF accordingly

---

## Expected Output
- Dashboard shows simulated and real data traces
- ESP32 LED reflects control logic driven by simulation

---

## Assessment Rubric (10 points)
- Sensor simulation correct: 2  
- ESP32 actuator response: 3  
- Dashboard comparison works: 3  
- Documentation/screenshots: 2  

---

## Summary
Prototyping enables rapid, low-risk CPS development. Techniques such as HIL testing and rapid prototyping platforms (ESP32, Node-RED) help engineers validate designs before full deployment. Lab 7 demonstrates integration of simulated and physical CPS components.

---

## Review Questions
1. What are the differences between MIL, SIL, and HIL?
2. Why is prototyping critical in CPS?
3. List advantages and limitations of rapid prototyping platforms.
4. How does HIL improve safety testing?

## Exercises
1. Add a second actuator (servo) controlled by simulated data.
2. Build a smart traffic light prototype with simulated arrivals.
3. Add dashboard controls to tune simulation parameters.

## Further Reading
- P. Fritzson, *Principles of Object-Oriented Modeling and Simulation with Modelica*  
- R. Rajkumar et al., *Cyber-Physical Systems: The Next Computing Revolution*  
- L. Monostori, *Cyber-Physical Production Systems*
