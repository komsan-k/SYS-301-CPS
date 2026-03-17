# 📱 Smartphone Motion Data Lab

**Real-Time Smartphone Sensor Data Streaming Using Python**

---

## 1. Lab Overview
This lab demonstrates how to read smartphone motion sensor data (pitch, roll, yaw) and display it in Python.

---

## 2. Learning Objectives
- Understand smartphone motion sensors  
- Stream sensor data over WiFi  
- Read JSON data in Python  
- Display real-time sensor values  

---

## 3. System Architecture
Smartphone Sensors → Phyphox → WiFi (HTTP) → Python → Console Output

---

## 4. Required Hardware
- Smartphone  
- Computer  
- WiFi Network  

---

## 5. Required Software
```
pip install requests
```

---

## 6. Smartphone Setup
1. Install Phyphox  
2. Open Orientation  
3. Start  
4. Enable Remote Access  

---

## 7. Python Code
```python
import requests
import time

URL = "http://192.168.1.8:8080/get?pitch&roll&yaw"

def read_angles():
    try:
        r = requests.get(URL, timeout=1)
        data = r.json()
        return data["buffer"]["pitch"]["buffer"][0],                data["buffer"]["roll"]["buffer"][0],                data["buffer"]["yaw"]["buffer"][0]
    except:
        return None

while True:
    angles = read_angles()
    if angles:
        pitch, roll, yaw = angles
        print(f"Pitch: {pitch:.2f}°, Roll: {roll:.2f}°, Yaw: {yaw:.2f}°")
    else:
        print("Waiting...")
    time.sleep(0.1)
```

---

## 8. Run
```
python smartphone_motion.py
```

---

## 9. Observation Table
| Trial | Motion | Sensor Response |
|------|--------|----------------|
| 1 | Tilt forward | Pitch increases |
| 2 | Tilt backward | Pitch decreases |
| 3 | Tilt left | Roll increases |
| 4 | Tilt right | Roll decreases |

---

## 10. Extensions
- Save to CSV  
- Plot data  
- MQTT integration  
