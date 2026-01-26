/*
  Lab 1 (5C) — Connection Layer
  Sensor/Actuator Bring-Up on ESP32

  - Reads an analog sensor (LDR divider or potentiometer) on GPIO34
  - Controls LED brightness on GPIO2 using PWM (LEDC)
  - Prints raw ADC and mapped PWM duty to Serial

  Hardware:
    Sensor out -> GPIO34
    LED -> GPIO2 (through resistor)
*/

#include <Arduino.h>

static const int PIN_ADC = 34;     // ADC1_CH6 (input only)
static const int PIN_LED = 2;      // On-board LED on many ESP32 dev boards

// LEDC (PWM) settings
static const int PWM_CHANNEL = 0;
static const int PWM_FREQ_HZ = 5000;
static const int PWM_RES_BITS = 8; // 0..255

static int mapToDuty(int adc)
{
  adc = constrain(adc, 0, 4095);
  int duty = (adc * 255) / 4095;
  return duty;
}

void setup() {
  Serial.begin(115200);
  delay(300);

  analogReadResolution(12);            // 0..4095
  analogSetAttenuation(ADC_11db);      // approx up to ~3.3V

  ledcSetup(PWM_CHANNEL, PWM_FREQ_HZ, PWM_RES_BITS);
  ledcAttachPin(PIN_LED, PWM_CHANNEL);

  Serial.println("Lab 1 — Connection Layer started.");
  Serial.println("ADC(GPIO34) -> PWM(GPIO2)");
}

void loop() {
  int adc = analogRead(PIN_ADC);
  int duty = mapToDuty(adc);

  // For LDR you may want inverted brightness (dark -> brighter LED)
  // duty = 255 - duty;

  ledcWrite(PWM_CHANNEL, duty);

  Serial.print("ADC=");
  Serial.print(adc);
  Serial.print("  PWM_DUTY=");
  Serial.println(duty);

  delay(100);
}
