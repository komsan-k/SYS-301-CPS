# 📱 VPython Smartphone Motion Lab

**Real-Time Smartphone Motion Digital Twin Using VPython**

------------------------------------------------------------------------

# 1. Lab Overview

This lab demonstrates how a **smartphone motion sensor** can control a
**3D object in VPython**.\
The smartphone acts as a **physical sensing device**, while Python
visualizes the motion as a **digital twin**.

When the smartphone is tilted or rotated, a **3D box in VPython rotates
accordingly**.

This experiment demonstrates core ideas in:

-   Cyber‑Physical Systems (CPS)
-   Digital Twins
-   Sensor data streaming
-   Real‑time visualization
-   Physics‑based motion sensing

------------------------------------------------------------------------

# 2. Learning Objectives

After completing this lab, students will be able to:

-   Understand smartphone motion sensors
-   Stream sensor data over WiFi
-   Process sensor data using Python
-   Visualize motion using VPython
-   Connect physical motion to a digital model

------------------------------------------------------------------------

# 3. System Architecture

Smartphone Motion Sensors\
(accelerometer / orientation)\
↓\
Sensor App (Phyphox)\
↓\
WiFi Streaming (HTTP)\
↓\
Python Program\
↓\
VPython Visualization\
↓\
3D Motion Digital Twin

------------------------------------------------------------------------

# 4. Required Hardware

  Component      Description
  -------------- -------------------------------------
  Smartphone     Android or iOS device
  Computer       Python installed
  WiFi Network   Same network for phone and computer

------------------------------------------------------------------------

# 5. Required Software

Install Python libraries:

    pip install vpython
    pip install requests

Example `requirements.txt`:

    vpython
    requests

------------------------------------------------------------------------

# 6. Smartphone Setup

Install the sensor application **Phyphox**.

Steps:

1.  Install Phyphox from the app store
2.  Open the application
3.  Select **Orientation** or **Accelerometer**
4.  Press **Start**
5.  Enable **Remote Access**

The app will display a link similar to:

    http://192.168.1.8:8080

Your computer must be on the **same WiFi network**.

------------------------------------------------------------------------

# 7. VPython Motion Visualization Code

Create a file:

    smartphone_motion_vpython.py

``` python
from vpython import box, vector, rate, canvas, color, label
import requests
import math

scene = canvas(title="Smartphone Motion Lab",
               width=900, height=600,
               background=color.white)

phone = box(size=vector(4,0.3,2), color=color.blue)

info = label(pos=vector(0,3,0),
             text="Waiting for data...",
             box=False)

URL = "http://192.168.1.8:8080/get?pitch&roll&yaw"

def read_angles():
    try:
        r = requests.get(URL, timeout=1)
        data = r.json()

        pitch = data["buffer"]["pitch"]["buffer"][0]
        roll  = data["buffer"]["roll"]["buffer"][0]
        yaw   = data["buffer"]["yaw"]["buffer"][0]

        return pitch, roll, yaw
    except:
        return None

while True:

    rate(30)

    angles = read_angles()
    if angles is None:
        continue

    pitch, roll, yaw = angles

    pitch = math.radians(pitch)
    roll  = math.radians(roll)
    yaw   = math.radians(yaw)

    axis_x = math.cos(yaw)*math.cos(pitch)
    axis_y = math.sin(pitch)
    axis_z = math.sin(yaw)*math.cos(pitch)

    up_x = -math.sin(roll)
    up_y = math.cos(roll)
    up_z = 0

    phone.axis = vector(axis_x,axis_y,axis_z)
    phone.up = vector(up_x,up_y,up_z)

    info.text = f"Pitch:{pitch:.2f} Roll:{roll:.2f} Yaw:{yaw:.2f}"
```

------------------------------------------------------------------------

# 8. Running the Lab

Run the Python script:

    python smartphone_motion_vpython.py

Then move or tilt the smartphone to observe the **3D box responding to
the motion**.

------------------------------------------------------------------------

# 9. Expected Result

The VPython box rotates according to smartphone motion.

  | Phone Motion  |  VPython Result |
  --------------- | --------------------
 | Tilt forward  |  Box tilts forward |
  | Tilt backward |  Box tilts backward |
 | Tilt left    |   Box rotates left |
 | Tilt right   |   Box rotates right |
 | Rotate flat  |   Box spins |

## 10. Observation Table


| Trial | Motion        | Sensor Response | Visualization       |
|------|---------------|----------------|---------------------|
| 1 | Tilt forward  | Pitch increases  | Box tilts forward  |
| 2 | Tilt backward | Pitch decreases  | Box tilts backward |
| 3 | Tilt left     | Roll increases   | Box rotates left   |
| 4 | Tilt right    | Roll decreases   | Box rotates right  |

# 11. CPS 5C Interpretation

|  CPS Layer   |    Role |
  --------------- | ------------------------------
  Connection   |   Smartphone sensors
  Conversion    |  Sensor app formats data
  Cyber       |    Python receives data
  Cognition    |   Visualization interpretation
  Configuration |  VPython object updates

------------------------------------------------------------------------

# 12. Extensions

Students can extend the lab with:

**Extension 1 --- Motion Trail** Add a trail showing motion history.

**Extension 2 --- Drone Simulator** Use phone tilt to control a drone
model.

**Extension 3 --- Motion Classification** Detect motion types such as: -
still - tilt - shake

**Extension 4 --- MQTT Integration**

Smartphone → MQTT → Node‑RED → VPython

**Extension 5 --- Physics Experiment** Use accelerometer data to
analyze: - gravity - acceleration vectors - motion in 3D space

------------------------------------------------------------------------

# 13. Troubleshooting

## Cannot connect to smartphone

Check:

-   Both devices are on the same WiFi network
-   Remote access is enabled in the phone app
-   Correct IP address is used

## JSON format different

Print the JSON response:

``` python
print(requests.get(URL).json())
```

Adjust field names accordingly.

------------------------------------------------------------------------

# 14. Advanced Improvement (Sensor Smoothing)

Add smoothing:

``` python
alpha = 0.2
smooth_pitch = alpha*pitch + (1-alpha)*smooth_pitch
```

This reduces jitter in the visualization.

------------------------------------------------------------------------

# 15. Suggested Mini Project

**Smartphone Motion Controller**

Use phone motion to control:

-   a robot
-   a drone simulator
-   a virtual vehicle
-   physics objects

------------------------------------------------------------------------

# 16. Educational Applications

This lab is suitable for courses in:

-   Cyber‑Physical Systems
-   Internet of Things
-   Digital Twin Systems
-   Sensor Networks
-   Computational Physics
