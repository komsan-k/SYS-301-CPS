# phyphox Orientation (Roll, Pitch, Yaw) Guide

---

## 📘 Overview

If you are using the **phyphox app** to obtain orientation data (similar to an MPU6050), there are **two main approaches**:

1. Using built-in "smart" sensors (easy)
2. Computing orientation manually from raw data

---

## 🔷 Option 1: Easy Way (Built-in Experiments)

phyphox includes built-in sensor fusion algorithms.

### 📱 Roll & Pitch → Inclination Experiment

- Uses accelerometer
- Measures tilt relative to gravity

Typical mapping:
- **Roll** → Rotation
- **Pitch** → Inclination

---

### 🧭 Yaw (Heading) → Compass / Attitude

- Uses magnetometer + gyro + accelerometer
- Provides **absolute heading (North reference)**

### ⭐ Best Option → Attitude Sensor

- Combines all sensors
- Provides stable **3D orientation (RPY)**

---

## 🔷 Option 2: Manual Way (Raw Data)

Use exported data (Accelerometer + Gyroscope) in:

- Excel
- Python
- MATLAB

---

### 📐 1. Accelerometer (Static Angles)

When motion is slow:

```
Roll  = atan2(Ay, Az) × (180/π)
Pitch = atan2(-Ax, sqrt(Ay² + Az²)) × (180/π)
```

---

### 📐 2. Gyroscope (Dynamic Motion)

Angular velocity:

```
New Angle = Old Angle + (ω × dt)
```

---

## ⚠️ Notes

- Accelerometer → noisy
- Gyroscope → drift
- Best practice → sensor fusion

---

## 🔧 Pro Tips for phyphox

### 📍 Coordinate System

- X → right
- Y → top
- Z → out of screen

---

### 🌐 Remote Access

- Enable **"Allow Remote Access"**
- View live data on browser
- Useful for CPS / experiments

---

### 🧪 Custom Experiments

Use phyphox editor:

- Combine Roll, Pitch, Yaw
- Create custom dashboards
- Integrate multi-sensor data

---

## 🚀 Applications

- Motion tracking
- CPS labs
- Robotics control
- Digital twin visualization
- Pan–Tilt systems

---

## ✅ Summary

phyphox provides:

- Easy RPY via built-in sensors
- Full control via raw data processing
- Real-time + remote visualization

---

## 🔧 Next Step

You can extend this with:

- ESP32 + MQTT integration
- Node-RED dashboard
- Digital Twin systems

👉 Ask for full CPS lab setup if needed
