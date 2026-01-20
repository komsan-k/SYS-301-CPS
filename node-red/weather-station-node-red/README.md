# Live Weather Station using Node-RED and OpenWeather

## Overview
This project demonstrates how to build a **live weather station** using **Node-RED** and the **OpenWeather API**.
The system periodically retrieves real-time weather data from OpenWeather and visualizes it on a Node-RED dashboard.
It is suitable for **Cyber-Physical Systems (CPS)**, **IoT**, and **smart city** laboratory exercises.

---

## System Architecture
```
OpenWeather API → Node-RED Flow → Dashboard Visualization
```

- Environment sensing (virtual sensor): OpenWeather API
- Cyber layer: Node-RED flow-based processing
- Human interface: Node-RED Dashboard

---

## Prerequisites

### 1. Install Node-RED
Ensure that **Node.js** is installed, then install Node-RED:

```bash
npm install -g node-red
```

Start Node-RED:
```bash
node-red
```

Access the editor at:
```
http://localhost:1880
```

---

### 2. Create an OpenWeather API Key
1. Register at https://openweathermap.org
2. Obtain an API key from the developer dashboard

---

### 3. Install Required Node-RED Nodes
Install the following nodes:
- node-red-dashboard
- node-red-node-openweathermap (optional)

Using npm:
```bash
npm install node-red-dashboard node-red-node-openweathermap
```

---

## Building the Node-RED Flow

### Nodes Used
- Inject Node – Triggers periodic API requests
- HTTP Request Node – Fetches weather data
- JSON Node – Parses JSON response
- Function Node – Extracts useful data
- Dashboard UI Nodes – Display data

---

### Flow Configuration

#### HTTP Request Node
```
https://api.openweathermap.org/data/2.5/weather?q=London&appid=YOUR_API_KEY&units=metric
```

#### Function Node Code
```javascript
var weather = msg.payload.weather[0].description;
var temp = msg.payload.main.temp;
var humidity = msg.payload.main.humidity;
var wind = msg.payload.wind.speed;

msg.payload = {
    temperature: temp + " C",
    humidity: humidity + " %",
    wind_speed: wind + " m/s",
    condition: weather
};
return msg;
```

---

## Dashboard
- Text node: weather condition
- Gauge node: temperature
- Chart node: temperature over time

---

## Deploy and Run
1. Click **Deploy**
2. Open http://localhost:1880/ui
3. View live weather updates

---

## CPS Perspective
OpenWeather acts as a virtual sensor, Node-RED performs computation, and the dashboard provides actuation through visualization.

---

## References
1. Node-RED Documentation – https://nodered.org/docs
2. Node-RED Guide – https://noderedguide.com/
3. OpenWeather API – https://openweathermap.org/api

---

## Summary
This README provides a practical guide for building a Node-RED–based live weather station and demonstrates a simple CPS architecture using external environmental data.
