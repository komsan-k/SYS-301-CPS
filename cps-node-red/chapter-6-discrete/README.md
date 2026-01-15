# Chapter 6: Discrete Control Algorithms in Cyber-Physical Systems (CPS)

## Chapter Overview
Cyber-Physical Systems (CPS) operate through control loops that map sensor data to actuator commands. Control algorithms act as the *brains* of CPS, ensuring stability, performance, and safety. This chapter introduces discrete control principles, rule-based logic, finite state machines (FSMs), and classical P/PI/PID controllers in digital implementations. A hands-on lab demonstrates automated control using Node-RED, MQTT, and ESP32.

## Learning Outcomes
After completing this chapter, students will be able to:
1. Describe the role of discrete control algorithms in CPS.
2. Implement rule-based (bang-bang) controllers.
3. Model CPS behavior using finite state machines (FSMs).
4. Apply basic P/PI/PID controllers in discrete time.
5. Build automated CPS control flows using Node-RED and ESP32.

## Key Terms
Control loop, setpoint, error signal, finite state machine (FSM), bang-bang control, proportional (P), integral (I), derivative (D), sampling period, stability.

---

## From Continuous to Discrete Control
Continuous-time control:
```
u(t) = f(x_desired(t) − x_measured(t))
```
Discrete-time (digital) implementation:
```
u[k] = f(x_d[k] − x_m[k])
```
The sampling period `Ts` must satisfy real-time deadlines:
```
Ts ≤ D
```

---

## Rule-Based (Bang-Bang) Control
A simple threshold-based controller:
```
u[k] = ON   if x[k] > x_high
u[k] = OFF  if x[k] < x_low
```
**Example:** A fan turns ON when temperature > 30°C and OFF when < 25°C.

---

## Finite State Machines (FSMs)
FSMs explicitly model CPS operating modes:
- **States:** OFF, IDLE, ACTIVE
- **Transitions:** triggered by events or thresholds
- **Actions:** executed on state entry/exit

FSMs improve clarity and safety in CPS logic design.

---

## Classical Controllers (P, PI, PID)

### Proportional (P)
```
u[k] = Kp · e[k]
```

### Proportional–Integral (PI)
```
u[k] = Kp · e[k] + Ki · Σ e[i] · Ts
```

### Proportional–Integral–Derivative (PID)
```
u[k] = Kp · e[k] + Ki · Σ e[i] · Ts + Kd · (e[k] − e[k−1]) / Ts
```

Used in temperature, motor speed, and position control.

---

## Timing and Stability Considerations
- Large `Ts` → instability
- Network jitter → oscillations
- Quantization → dead zones and limit cycles

---

## Case Study: Room Temperature Control
- **Sensor:** ESP32 temperature reading
- **Controller:** Node-RED rule (IF temp > 30°C → fan ON)
- **Actuator:** Fan or LED via ESP32 GPIO

This bang-bang controller maintains temperature within a safe band.

---

# Lab 6: Automated CPS Control (Rule-Based)

## Lab Objectives
- Implement threshold-based control logic
- Automatically actuate devices based on sensor data
- Visualize control actions on Node-RED dashboard

## Required Hardware & Software
- ESP32 with temperature sensor (LM73 or simulated)
- LED or fan actuator
- MQTT broker (Mosquitto)
- Node-RED with dashboard

---

## Lab Procedure

### 1. Sensor → MQTT
ESP32 publishes temperature to:
```
/cps/temp/room
```

### 2. Node-RED Control Logic
Flow:
```
MQTT-in → JSON → Function (IF logic) → MQTT-out → UI Text
```

Function node logic:
```javascript
let t = Number(msg.payload.t);
msg.topic = "/cps/cmd/fan";
msg.payload = (t > 30) ? "ON" : "OFF";
return msg;
```

### 3. Actuation
ESP32 subscribes to `/cps/cmd/fan` and controls the actuator.

---

## Expected Output
- Fan turns **ON** when temperature > 30°C
- Fan turns **OFF** otherwise
- Dashboard displays fan status

---

## Assessment Rubric (10 points)
- ESP32 publishes valid data: 2  
- Correct IF-logic in Node-RED: 3  
- Actuator responds correctly: 3  
- Dashboard status correct: 2  

---

## Summary
Discrete control algorithms are fundamental to CPS operation. Rule-based logic and FSMs handle events and modes, while P/PI/PID controllers regulate continuous dynamics. Lab 6 demonstrates automatic cyber-to-physical control using Node-RED and ESP32.

---

## Review Questions
1. Continuous vs. discrete control: what is the difference?
2. How does an FSM represent CPS modes?
3. Write and explain the discrete PID equation.
4. Why are timing constraints critical in digital control?

## Exercises
1. Add hysteresis: fan ON > 30°C, OFF < 28°C.
2. Implement a three-state FSM (OFF/DIM/BRIGHT) in Node-RED.
3. Create a proportional controller with adjustable `Kp` using a dashboard slider.

## Further Reading
- K. Ogata, *Discrete-Time Control Systems*  
- E. A. Lee & S. Tripakis, *Foundations of CPS Modeling*  
- H. K. Khalil, *Nonlinear Systems*
