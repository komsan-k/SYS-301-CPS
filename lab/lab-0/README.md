# Lab 1: Sensor → Decision → Actuator using ESP32 (Wokwi Simulation)

## Overview
This laboratory introduces the fundamental Cyber-Physical System (CPS) loop using an ESP32 simulated on Wokwi.
Students will implement a simple Sensor → Decision → Actuator workflow using a Light Dependent Resistor (LDR) and an LED.

## Learning Objectives
- Understand the CPS feedback loop
- Interface analog sensors using ESP32 ADC
- Implement rule-based decision logic
- Control actuators using GPIO
- Use Wokwi for virtual embedded system prototyping

## CPS Mapping
| CPS Layer | Implementation |
|----------|----------------|
| Physical | LDR, LED |
| Cyber | ESP32 firmware |
| Decision | Threshold-based logic |
| Actuation | Digital output (LED control) |

## Hardware Components (Simulated)
- ESP32 Dev Module
- LDR (Light Sensor)
- LED
- 10kΩ Resistor
- Breadboard & wires

## Software Requirements
- Web browser
- Wokwi online simulator (https://wokwi.com)

## Circuit Description
- LDR connected to GPIO 34 (ADC)
- LED connected to GPIO 2
- Voltage divider used for LDR measurement

## Arduino Code
```cpp
#define LDR_PIN 34
#define LED_PIN 2
#define THRESHOLD 2000

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  int ldrValue = analogRead(LDR_PIN);
  Serial.println(ldrValue);

  if (ldrValue < THRESHOLD) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
  delay(500);
}
```

## Expected Results
- LED turns ON when light intensity is low
- LED turns OFF when light intensity is high

## Outcome
Students gain hands-on experience with CPS fundamentals using ESP32.
