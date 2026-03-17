import requests
import math
import time

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
    angles = read_angles()
    
    if angles is None:
        print("Waiting for data...")
        time.sleep(0.5)
        continue

    pitch, roll, yaw = angles

    # Convert to radians (optional)
    pitch_rad = math.radians(pitch)
    roll_rad  = math.radians(roll)
    yaw_rad   = math.radians(yaw)

    print(f"Pitch: {pitch:.2f}°, Roll: {roll:.2f}°, Yaw: {yaw:.2f}°")
    # Optional: also print radians
    # print(f"(rad) Pitch: {pitch_rad:.2f}, Roll: {roll_rad:.2f}, Yaw: {yaw_rad:.2f}")

    time.sleep(0.1)
