# OpenWeather API for Cyber-Physical Systems (CPS)

## Overview
OpenWeather is a global weather data provider offering APIs for real-time, forecast, and historical weather information. It is widely used in Cyber-Physical Systems (CPS) applications such as smart cities, agriculture monitoring, transportation systems, and environmental sensing.

This README provides a concise, practical introduction to using the OpenWeather API and integrating it into CPS and IoT workflows.

---

## Features of OpenWeather

### Weather APIs
OpenWeather provides several APIs, including:
- **Current Weather API** – Real-time weather data for any location
- **5-Day / 3-Hour Forecast API** – Weather forecasts at 3-hour intervals
- **16-Day Forecast API** – Long-range weather predictions
- **Historical Weather API** – Access to past weather data
- **Air Pollution API** – CO, NO₂, SO₂, PM2.5, PM10 levels
- **Weather Alerts API** – Severe weather alerts from official sources

---

## Data Sources and Accuracy
OpenWeather aggregates data from multiple sources:
- Satellite imagery
- Ground-based weather stations
- Radar systems
- Machine learning models for predictive analysis

This multi-source approach improves data reliability and spatial coverage, making it suitable for CPS decision-making.

---

## Pricing Plans
- **Free Plan** – Limited API requests per minute
- **Paid Plans** – Higher request limits, improved accuracy, and extended datasets

---

## How to Use the OpenWeather API

### Step 1: Obtain an API Key
1. Register at https://openweathermap.org  
2. Generate an API key from the developer dashboard

---

### Step 2: Make an API Request
Example using `curl` to fetch current weather data:

```bash
curl "https://api.openweathermap.org/data/2.5/weather?q=London&appid=YOUR_API_KEY&units=metric"
```

---

### Step 3: Parse the JSON Response
Sample response structure:
```json
{
  "weather": [{ "description": "clear sky" }],
  "main": { "temp": 20.5, "humidity": 60 },
  "wind": { "speed": 3.2 }
}
```

---

### Step 4: Extract Data in Python
```python
import requests

API_KEY = "YOUR_API_KEY"
url = f"https://api.openweathermap.org/data/2.5/weather?q=London&appid={API_KEY}&units=metric"

response = requests.get(url)
data = response.json()

temperature = data["main"]["temp"]
humidity = data["main"]["humidity"]
condition = data["weather"][0]["description"]

print(f"Temperature: {temperature} °C")
print(f"Humidity: {humidity} %")
print(f"Condition: {condition}")
```

---

## OpenWeather in CPS Architecture
In a CPS context, OpenWeather typically acts as an **environmental sensing service**:

```
OpenWeather API → Data Processing → Decision Logic → Actuation / Dashboard
```

Example CPS use cases:
- Adaptive traffic control based on weather conditions
- Smart irrigation systems using rainfall forecasts
- Environmental risk monitoring (heatwaves, air pollution)
- Energy-aware building management systems

---

## Alternatives to OpenWeather
Other weather data providers include:
- WeatherAPI (formerly WeatherStack)
- AccuWeather API
- Tomorrow.io (ClimaCell) API
- NOAA National Weather Service API (US-based)

---

## Summary
OpenWeather provides a flexible and reliable API for accessing weather and environmental data. When integrated into Cyber-Physical Systems, it enables context-aware decision-making, improved resilience, and sustainable operation in smart environments.

---

## Next Steps
- Integrate OpenWeather API with Node-RED
- Use weather data as input to CPS decision logic
- Visualize real-time and forecast data on dashboards
- Combine with ESP32 and MQTT for edge-to-cloud CPS applications
