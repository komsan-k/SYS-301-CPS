# 📱 Smartphone Motion Lab (Google Colab Version)

**Real-Time Smartphone Sensor Data Streaming and Visualization in Colab**

---

## 1. Overview
This lab demonstrates how to stream smartphone motion sensor data (pitch, roll, yaw) into Google Colab and visualize it.

---

## 2. Requirements
- Smartphone with Phyphox
- Google Colab
- Public URL (ngrok)

---

## 3. Setup

### Install libraries (Colab usually has them)
```python
!pip install requests pandas matplotlib
```

---

## 4. Python Code

```python
import requests
import time
import pandas as pd
import matplotlib.pyplot as plt
from IPython.display import clear_output, display

URL = "https://your-public-url/get?pitch&roll&yaw"

data_log = []

def read_angles():
    try:
        r = requests.get(URL, timeout=2)
        data = r.json()
        return data["buffer"]["pitch"]["buffer"][0],                data["buffer"]["roll"]["buffer"][0],                data["buffer"]["yaw"]["buffer"][0]
    except:
        return None

for i in range(50):
    angles = read_angles()

    if angles is None:
        print("Waiting...")
        time.sleep(1)
        continue

    pitch, roll, yaw = angles
    data_log.append({"sample": i, "pitch": pitch, "roll": roll, "yaw": yaw})

    df = pd.DataFrame(data_log)

    clear_output(wait=True)
    print(f"Pitch: {pitch:.2f}, Roll: {roll:.2f}, Yaw: {yaw:.2f}")
    display(df.tail())

    plt.figure()
    plt.plot(df["sample"], df["pitch"], label="Pitch")
    plt.plot(df["sample"], df["roll"], label="Roll")
    plt.plot(df["sample"], df["yaw"], label="Yaw")
    plt.legend()
    plt.show()

    time.sleep(0.5)
```

---

## 5. Save Data

```python
df.to_csv("motion_data.csv", index=False)
```

---

## 6. Notes
- Replace URL with your ngrok/public URL
- Local IP (192.168.x.x) will NOT work in Colab

---

## 7. Extensions
- Real-time dashboard
- ML classification
- MQTT integration
