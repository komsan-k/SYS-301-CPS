#include <WiFi.h>
#include <PubSubClient.h>

/*
  ESP32 Closed-Loop CPS
  - Publishes sensor data to:  cps/esp32/sensor
  - Subscribes actuator cmds: cps/esp32/actuator
  - Controls LED deterministically (GPT does NOT control hardware)

  Payloads:
  Sensor:   {"device":"esp32-01","ts":1700000000,"ldr":1234}
  Actuator: {"device":"esp32-01","led":1,"reason":"Rule: ..."}   // led: 1=ON, 0=OFF
*/

// ====== Wi‑Fi ======
const char* ssid     = "YOUR_WIFI";
const char* password = "YOUR_PASS";

// ====== MQTT Broker ======
const char* mqtt_server = "192.168.1.10";   // <-- change to your broker IP
const int   mqtt_port   = 1883;

// ====== Topics ======
const char* TOPIC_SENSOR   = "cps/esp32/sensor";
const char* TOPIC_ACTUATOR = "cps/esp32/actuator";

// ====== Pins ======
const int LDR_PIN = 34;   // ADC pin (GPIO34)
const int LED_PIN = 2;    // Built‑in LED on many ESP32 boards (change if needed)

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastSend = 0;

// ---------- Wi‑Fi ----------
void setupWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
  }
}

// ---------- MQTT ----------
void reconnectMQTT() {
  while (!client.connected()) {
    String cid = "esp32-" + String((uint32_t)ESP.getEfuseMac(), HEX);
    if (client.connect(cid.c_str())) {
      client.subscribe(TOPIC_ACTUATOR);   // listen for LED commands
      break;
    }
    delay(1000);
  }
}

// Very small parser for {"led":1} or {"led":0}
int parseLedCommand(const char* payload) {
  const char* p = strstr(payload, "\"led\"");
  if (!p) return -1;
  p = strchr(p, ':');
  if (!p) return -1;
  p++;
  while (*p == ' ' || *p == '\t') p++;
  if (*p == '1') return 1;
  if (*p == '0') return 0;
  return -1;
}

// ---------- MQTT Callback ----------
void callback(char* topic, byte* payload, unsigned int length) {
  static char buf[256];
  unsigned int n = (length < sizeof(buf)-1) ? length : (sizeof(buf)-1);
  memcpy(buf, payload, n);
  buf[n] = '\0';

  Serial.print("MQTT IN [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.println(buf);

  if (strcmp(topic, TOPIC_ACTUATOR) == 0) {
    int led = parseLedCommand(buf);
    if (led == 1) {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("LED -> ON (rule-based)");
    } else if (led == 0) {
      digitalWrite(LED_PIN, LOW);
      Serial.println("LED -> OFF (rule-based)");
    } else {
      Serial.println("Invalid actuator payload (expected {\"led\":0/1})");
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  setupWifi();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  analogReadResolution(12);   // ADC range 0..4095
}

void loop() {
  if (!client.connected()) reconnectMQTT();
  client.loop();

  // Publish sensor every 2 seconds
  if (millis() - lastSend > 2000) {
    lastSend = millis();

    int ldr = analogRead(LDR_PIN);
    unsigned long ts = millis() / 1000;

    char payload[128];
    snprintf(payload, sizeof(payload),
             "{\"device\":\"esp32-01\",\"ts\":%lu,\"ldr\":%d}",
             ts, ldr);

    client.publish(TOPIC_SENSOR, payload);
    Serial.println(payload);
  }
}
