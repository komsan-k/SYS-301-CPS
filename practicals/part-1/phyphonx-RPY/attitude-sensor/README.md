# phyphox Attitude Sensor (Roll, Pitch, Yaw)

---

## 📘 Overview

To obtain **Roll, Pitch, and Yaw (RPY)** in phyphox, the best approach is to use a **Virtual Sensor called the Attitude Sensor**.

Unlike raw accelerometer data, the Attitude Sensor uses **Sensor Fusion** by combining:

- Accelerometer  
- Gyroscope  
- Magnetometer  

This provides a **stable 3D orientation**.

---

## 🔷 1. Using Pre-Made Attitude Experiment

### Steps

1. Open the following link on your phone:
   https://phyphox.org/wiki/index.php/Attitude_sensor

2. Scroll to:
   - *"Attitude with Euler angles"*

3. Open via phyphox app

### Result

You will get live:

- Roll  
- Pitch  
- Yaw  

---

## 🔷 2. Build Your Own (Visual Editor)

### A. Input

- Go to **Inputs tab**
- Add **Attitude Sensor**

This outputs:

```
q0, q1, q2, q3 (Quaternion)
```

---

### B. Analysis (Convert to Angles)

Use:

- **Quaternion → Euler block**

Outputs:

- Yaw  
- Pitch  
- Roll  

---

### C. Coordinate System

| Axis | Direction |
|------|----------|
| X | left → right |
| Y | bottom → top |
| Z | out of screen |

---

## ⚠️ Important Note (Yaw)

- Uses magnetometer → references **Magnetic North**
- Can be noisy indoors (metal interference)

---

## 🔷 3. Remote Access (Recommended)

### Steps

1. Tap menu (⋮)
2. Select **Allow Remote Access**
3. Open browser:

```
http://<phone-ip>:8080
```

---

### Benefits

- View graphs on laptop  
- Export CSV data  
- Real-time monitoring  

---

## 🚀 Applications

- CPS labs  
- Motion tracking  
- Robotics control  
- Digital Twin systems  
- Pan–Tilt control  

---

## ✅ Summary

phyphox Attitude Sensor provides:

- Full 3D orientation (RPY)
- Sensor fusion (Accel + Gyro + Magnetometer)
- Real-time visualization
- Remote monitoring capability

---

## 🔧 Next Step

You can integrate this with:

- ESP32 (MQTT)
- Node-RED dashboard
- Digital Twin visualization

👉 Ask for full CPS integration lab if needed
