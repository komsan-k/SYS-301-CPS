#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASS";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);

  client.setServer(mqtt_server, 1883);
}

void reconnect() {
  while (!client.connected()) {
    client.connect("ESP32Client");
  }
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  int rssi = WiFi.RSSI();
  char msg[10];
  sprintf(msg, "%d", rssi);
  client.publish("esp32/rssi", msg);

  delay(1000);
}
