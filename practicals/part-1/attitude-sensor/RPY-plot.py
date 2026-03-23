import requests
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from collections import deque

# --- CONFIGURATION ---
IP_ADDRESS = "172.19.177.163"
URL = f"http://{IP_ADDRESS}/get?yaw&pitch&roll"
MAX_POINTS = 100  # Number of data points to show on the graph at once

# Data buffers
times = deque(maxlen=MAX_POINTS)
y_data = deque(maxlen=MAX_POINTS)
p_data = deque(maxlen=MAX_POINTS)
r_data = deque(maxlen=MAX_POINTS)

# Initialize Plot
fig, ax = plt.subplots(figsize=(10, 6))
line_y, = ax.plot([], [], label='Yaw', color='red')
line_p, = ax.plot([], [], label='Pitch', color='green')
line_r, = ax.plot([], [], label='Roll', color='blue')

ax.set_ylim(-180, 360)  # Angles usually range between these
ax.set_xlim(0, MAX_POINTS)
ax.legend(loc='upper right')
ax.set_title("Live MPU/Attitude Data from Phyphox")
ax.set_xlabel("Samples")
ax.set_ylabel("Degrees (°)")

count = 0


def update(frame):
    global count
    try:
        response = requests.get(URL, timeout=0.1)
        if response.status_code == 200:
            data = response.json()

            # Get latest values from buffer
            yaw = data['buffer']['yaw']['buffer'][0]
            pitch = data['buffer']['pitch']['buffer'][0]
            roll = data['buffer']['roll']['buffer'][0]

            # Update deques
            times.append(count)
            y_data.append(yaw)
            p_data.append(pitch)
            r_data.append(roll)
            count += 1

            # Update lines
            line_y.set_data(range(len(y_data)), y_data)
            line_p.set_data(range(len(p_data)), p_data)
            line_r.set_data(range(len(r_data)), r_data)

            # Auto-scroll X-axis
            if count > MAX_POINTS:
                ax.set_xlim(0, MAX_POINTS)  # Keep view static or adjust for moving window

    except Exception as e:
        pass  # Ignore minor connection hiccups

    return line_y, line_p, line_r


# Interval is in milliseconds (50ms = 20Hz)
ani = FuncAnimation(fig, update, interval=50, blit=True)

plt.tight_layout()
plt.show()
