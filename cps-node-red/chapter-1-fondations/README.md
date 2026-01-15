# Foundations of Cyber-Physical Systems (CPS)

## Chapter Overview
Cyber-Physical Systems (CPS) integrate computation, communication, and control with physical processes. This chapter builds the conceptual foundation for the course, clarifies how CPS differs from embedded systems and IoT, introduces representative applications (smart cities, healthcare, energy, transportation, manufacturing), and frames the analytical lenses used throughout the course.

## Learning Outcomes
After completing this chapter, students will be able to:
1. Define CPS and explain the interplay among sensing, computation, control, actuation, and communication.
2. Differentiate CPS from embedded systems, IoT solutions, and digital twins.
3. Describe the CPS layered architecture.
4. Identify key design constraints: sampling, latency, jitter, reliability, and safety/security.
5. Explain discrete and hybrid modeling approaches in CPS.
6. Set up a basic Node-RED environment and implement a “Hello CPS” dashboard.

## Prerequisites
- Basic programming
- Introductory signals and systems
- Familiarity with microcontrollers (Arduino/ESP32)

## Key Terms
Cyber-Physical System, sensing chain, actuator, control loop, discrete-event system (DES), hybrid system, real-time constraint, latency, jitter, reliability, safety, security, digital twin, MQTT, Node-RED.

## What Is a Cyber-Physical System?
A CPS is a feedback system where computational elements observe and influence physical processes. Sensors capture the state of the physical world, computation produces decisions, and actuators apply control back to the system. Communication networks connect distributed components.

### CPS vs Embedded Systems
Embedded systems focus on computation within devices, while CPS emphasize tight feedback with physical dynamics, timing guarantees, and safety.

### CPS vs IoT
IoT focuses on connectivity and data exchange. CPS focus on closed-loop control and timing correctness.

### CPS vs Digital Twins
Digital twins are virtual representations that enhance CPS with prediction and analysis but are not CPS by themselves.

## Application Domains
- **Smart Manufacturing:** robotics, inspection, predictive maintenance  
- **Energy Systems:** microgrids, inverter control  
- **Transportation:** traffic control, V2I safety  
- **Healthcare:** patient monitoring, closed-loop therapy  
- **Smart Buildings/Cities:** HVAC optimization, lighting control  

## CPS Layered Architecture
**Perception → Network → Cyber → Actuation → Human-in-the-Loop**  
Closed-loop operation must be timed, reliable, and secure.

## Timing and Real-Time Constraints
End-to-end deadline:
```
D = d_sense + d_net + d_compute + d_act
```
Design rules:
- Sampling period Ts ≤ D  
- Low jitter is critical for stability

## Modeling Perspectives
- **Discrete-Event Systems (DES):** events and state changes  
- **Continuous Models:** physical dynamics  
- **Hybrid Systems:** combination of both  

## Toolchain and Platforms
- **Edge:** ESP32, Arduino, Raspberry Pi, PLCs  
- **Middleware:** Node-RED, MQTT, OPC UA  
- **Modeling:** Modelica, Simulink, Python, MATLAB  

## Security and Safety
Minimum practices:
1. Authentication and least privilege
2. Encrypted communication (TLS)
3. Data validation
4. Safety interlocks and emergency stops

## Case Study: Smart Room Controller
- Sensors: Temperature, Humidity, Occupancy  
- Update rate: 1 Hz  
- Latency: ≤ 500 ms  
- Fail-safe on missing data  

## Lab 1: Hello CPS World (Node-RED)
### Objectives
- Install Node-RED
- Create a simple CPS dataflow
- Understand message passing

### Software Requirements
- Node.js (LTS)
- Node-RED
- Web browser

### Procedure
1. Create flow: Inject → Function → Debug  
2. Use function:
```javascript
msg.payload = "Hello CPS: " + new Date().toISOString();
return msg;
```
3. Deploy and observe output
4. Add dashboard text node

### Assessment (10 points)
- Installation: 2  
- Working flow: 3  
- Dashboard output: 3  
- Explanation/screenshot: 2  

## Design Trade-offs
- Performance vs resources
- Latency vs sampling
- Security vs usability

## Scaling CPS
Challenges include heterogeneity, orchestration, observability, and governance.

## Summary
CPS engineering integrates computation with real-world physics, demanding attention to timing, reliability, and safety. Node-RED enables rapid CPS prototyping.

## Review Questions
1. What subsystems form the CPS loop?
2. How does CPS differ from IoT?
3. Why does jitter matter?
4. Give a hybrid system example.
5. What security measures are essential?

## Exercises
1. Compute latency budgets for a control loop.
2. Design MQTT topic hierarchy for a smart room.
3. Extend Lab 1 with periodic updates and charts.

## Further Reading
- E. A. Lee, *Cyber Physical Systems: Design Challenges*
- R. Rajkumar et al., *The Next Computing Revolution*
- P. Tabuada, *Verification and Control of Hybrid Systems*
- OPC Foundation, *OPC UA Specifications*
