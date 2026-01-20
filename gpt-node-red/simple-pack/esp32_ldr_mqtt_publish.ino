#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASS";

const char* mqtt_server = "192.168.1.10";  // Mosquitto broker IP
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

const int LDR_PIN = 34;  // ESP32 ADC pin
unsigned long lastSend = 0;

void setupWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(300);
}

void reconnectMQTT() {
  while (!client.connected()) {
    String cid = "esp32-" + String((uint32_t)ESP.getEfuseMac(), HEX);
    if (client.connect(cid.c_str())) break;
    delay(1000);
  }
}

void setup() {
  Serial.begin(115200);
  setupWifi();
  client.setServer(mqtt_server, mqtt_port);
  analogReadResolution(12); // 0..4095
}

void loop() {
  if (!client.connected()) reconnectMQTT();
  client.loop();

  if (millis() - lastSend > 2000) {
    lastSend = millis();

    int ldr = analogRead(LDR_PIN);
    unsigned long ts = millis() / 1000;

    char payload[128];
    snprintf(payload, sizeof(payload),
             "{\"device\":\"esp32-01\",\"ts\":%lu,\"ldr\":%d}",
             ts, ldr);

    client.publish("cps/esp32/sensor", payload);
    Serial.println(payload);
  }
}
