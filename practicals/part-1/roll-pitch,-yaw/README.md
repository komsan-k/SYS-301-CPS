# MPU6050: Roll, Pitch, and Yaw Estimation (Complementary Filter)

---

## 📘 Overview

Converting raw data from an **MPU6050** into stable **Roll, Pitch, and Yaw (RPY)** values is a classic engineering challenge.

The MPU6050 is a **6-axis motion tracking device** that includes:

- 3-axis Accelerometer  
- 3-axis Gyroscope  

⚠️ It **does NOT include a magnetometer**, which limits absolute Yaw estimation.

---

## 🔷 1. Roll and Pitch from Accelerometer

The accelerometer measures the **gravity vector**, allowing estimation of tilt.

### 📐 Equations

Roll (rotation around X-axis):

```
Roll = atan2(Ay, sqrt(Ax² + Az²)) × (180 / π)
```

Pitch (rotation around Y-axis):

```
Pitch = atan2(-Ax, sqrt(Ay² + Az²)) × (180 / π)
```

### ⚠️ Problem

- Accelerometer readings are **noisy**
- Vibrations cause spikes in angle estimation

---

## 🔷 2. Gyroscope Integration

The gyroscope measures **angular velocity (°/s)**.

### 📐 Equation

```
Angle = Angle_previous + (Gyro_Rate × dt)
```

### ⚠️ Problem

- Gyroscope suffers from **drift**
- Small errors accumulate over time

---

## 🔷 3. Sensor Fusion (Complementary Filter)

To combine strengths:

- Accelerometer → stable long-term reference  
- Gyroscope → smooth short-term motion  

### 📐 Complementary Filter

```
Angle = 0.98 × (Angle + Gyro_Rate × dt) + 0.02 × Accel_Angle
```

### 🧠 Insight

- Gyro → high-pass behavior (fast response)  
- Accel → low-pass behavior (stable reference)  

---

## 🔷 4. The Yaw Problem

### ❗ Key Limitation

The MPU6050 **cannot calculate absolute Yaw**.

### Why?

- Accelerometer detects only gravity (vertical direction)
- No reference to Earth's magnetic field (North)

### Result

- Yaw is **relative only**
- Yaw will **drift indefinitely**

---

## 🔷 5. Solution for Absolute Yaw

To obtain stable Yaw:

- Use **MPU9250** (with built-in magnetometer)  
- Or add **HMC5883L** external magnetometer  

---

## 📊 Summary Table

| Metric       | Source         | Strength                  | Weakness              |
|-------------|--------------|--------------------------|-----------------------|
| Roll/Pitch  | Accel + Gyro | Very stable with fusion  | Sensitive to vibration |
| Yaw         | Gyro only    | Good short-term tracking | Drifts over time      |

---

## 🚀 Applications

- Robotics orientation  
- Drone stabilization  
- Camera gimbal systems  
- CPS motion tracking  
- Pan–Tilt platforms  

---

## ✅ Summary

The MPU6050 provides:

- Reliable **Roll and Pitch** via sensor fusion  
- Limited **Yaw** due to lack of magnetometer  

The **Complementary Filter** is a simple and effective solution for real-time orientation estimation on embedded systems.

---

## 🔧 Next Step

You can implement this using:

- Arduino (ESP32 / UNO)  
- Python (for visualization / digital twin)  

👉 Ask for ready-to-use code if needed
