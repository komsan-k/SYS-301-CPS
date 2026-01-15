# Chapter 2: CPS Architectures and Models

## Chapter Overview
This chapter explores architectural frameworks and system models that underpin Cyber-Physical Systems (CPS). Building on CPS foundations, it introduces reference architectures (5C, IIRA, RAMI 4.0), layered abstractions, discrete-event and hybrid models, and CPS co-design principles. Practical work uses Node-RED to model CPS data flows without physical hardware.

## Learning Outcomes
After completing this chapter, students will be able to:
1. Describe layered CPS architectures and functional decomposition.
2. Explain the 5C architecture (Connection, Conversion, Cyber, Cognition, Configuration).
3. Contrast IIRA with RAMI 4.0.
4. Model CPS processes using discrete-event and hybrid representations.
5. Build CPS dataflow models using Node-RED.

## Key Terms
5C architecture, IIRA, RAMI 4.0, discrete-event system (DES), hybrid system, layered CPS model, co-design, Node-RED.

## Reference Architectures

### 5C Architecture
- **Connection:** data acquisition from sensors and devices  
- **Conversion:** raw data to meaningful information  
- **Cyber:** data aggregation and analytics  
- **Cognition:** decision-making and knowledge extraction  
- **Configuration:** adaptive feedback to physical systems  

### Industrial Internet Reference Architecture (IIRA)
- **Business viewpoint:** value, stakeholders, compliance  
- **Usage viewpoint:** scenarios and workflows  
- **Functional viewpoint:** control, operations, information, business domains  

### RAMI 4.0
A three-dimensional reference cube:
- **Hierarchy levels:** field devices to enterprise  
- **Layers:** asset → business  
- **Life cycle:** type (design) to instance (runtime)  

## Layered CPS Model
Perception → Communication → Cyber → Application → Human  
Closed-loop interactions span sensing, computation, and actuation.

## System Models

### Discrete-Event Systems (DES)
State transitions triggered by events (alarms, packets, mode changes).

### Continuous Models
Physical processes described by differential equations.

### Hybrid Systems
Combination of continuous dynamics and discrete logic  
*Example:* traffic light control.

## CPS Co-Design Principles
1. Hardware–software co-design  
2. Timing-aware design  
3. Safety and security by design  
4. Scalability and extensibility  

## Case Study: Traffic Light CPS
- Sensors detect vehicles  
- Controllers compute signal phases  
- Actuators switch lights  
- Communication shares traffic data  
- Hybrid system model (continuous flow + discrete FSM)

---

# Lab 2: Modeling CPS Data Flow with Node-RED

## Lab Objectives
- Model a CPS pipeline using Node-RED
- Simulate sensors with inject nodes
- Process data using function nodes
- Visualize actuator states on dashboards

## Required Software
- Node-RED (with `node-red-dashboard`)

## Procedure
1. Create two **inject** nodes (Sensor A, Sensor B).
2. Connect to a **function** node with:
```javascript
let v = Number(msg.payload);
msg.payload = { source: msg.topic, value: v, score: v * 2 };
return msg;
```
3. Connect output to:
   - a **gauge** dashboard node
   - a **debug** node
4. Deploy and trigger inject nodes.

## Expected Output
- Sensor A (42) → Gauge displays 84  
- Sensor B (17) → Gauge displays 34  
- Debug panel shows structured JSON messages

## Assessment Rubric (10 points)
- Inject nodes with topics: 2  
- Correct function logic: 3  
- Dashboard gauge output: 3  
- Debug consistency: 2  

## Summary
CPS architectures organize sensing, communication, computation, and actuation into structured layers. Reference models guide scalable and interoperable designs, while Node-RED enables rapid CPS dataflow prototyping.

## Review Questions
1. List and explain the 5C architecture layers.
2. Compare IIRA and RAMI 4.0.
3. Distinguish DES, continuous, and hybrid models.
4. Why is co-design essential in CPS?

## Exercises
1. Add a third sensor with a different processing rule.
2. Map a smart factory use case onto RAMI 4.0.
3. Model a traffic light CPS as a hybrid system.

## Further Reading
- Monostori, *Cyber-Physical Production Systems*, 2014  
- Industrial Internet Consortium, *IIRA*, 2019  
- Plattform Industrie 4.0, *RAMI 4.0*, 2015  
- Derler et al., *Modeling Cyber-Physical Systems*, 2012  
