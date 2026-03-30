#include <WiFi.h>

// ======================================================
// Wi-Fi credentials
// ======================================================
const char* ssid     = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// ======================================================
// Optional LED pins for class indication
// Strong   -> GREEN
// Moderate -> YELLOW
// Weak     -> RED
// ======================================================
const int LED_STRONG   = 18;
const int LED_MODERATE = 19;
const int LED_WEAK     = 21;

// ======================================================
// Neural Network: 1-8-3
// Input  : normalized RSSI in [0,1]
// Hidden : 8 neurons with ReLU
// Output : 3 neurons -> [Strong, Moderate, Weak]
// ======================================================

// Hidden layer weights: 8 x 1
float W1[8] = {
  0.2f, -0.8f,  1.6f, -1.1f,
  0.9f,  1.8f, -1.5f,  0.7f
};

float b1[8] = {
 -0.2f,  0.7f, -0.4f,  0.9f,
  0.1f, -0.8f,  1.0f, -0.1f
};

// Output layer weights: 3 x 8
float W2[3][8] = {
  // Strong
  { 2.6f, -0.5f,  0.3f, -0.6f,  0.9f,  1.1f, -0.8f,  0.7f },

  // Moderate
  {-0.3f,  1.4f,  2.2f,  1.1f,  0.8f, -0.4f,  1.0f,  0.6f },

  // Weak
  {-1.2f,  1.1f, -1.0f,  1.5f, -0.6f, -1.3f,  1.4f, -2.5f }
};

float b2[3] = {0.1f, 0.2f, 0.0f};

// ======================================================
// RSSI smoothing buffer
// Helps reduce class flicker due to Wi-Fi fluctuations
// ======================================================
const int SMOOTH_N = 5;
float rssiBuf[SMOOTH_N] = {0};
int rssiIdx = 0;
bool rssiFilled = false;

// ======================================================
// Utility functions
// ======================================================
float relu(float x) {
  return (x > 0.0f) ? x : 0.0f;
}

float clamp01(float x) {
  if (x < 0.0f) return 0.0f;
  if (x > 1.0f) return 1.0f;
  return x;
}

// Normalize RSSI from [-90, -30] to [0, 1]
float normalizeRSSI(int rssi) {
  float x = (rssi + 90.0f) / 60.0f;
  return clamp01(x);
}

// Optional moving average smoothing
float smoothRSSI(float newVal) {
  rssiBuf[rssiIdx] = newVal;
  rssiIdx = (rssiIdx + 1) % SMOOTH_N;

  if (rssiIdx == 0) {
    rssiFilled = true;
  }

  int n = rssiFilled ? SMOOTH_N : rssiIdx;
  if (n <= 0) n = 1;

  float sum = 0.0f;
  for (int i = 0; i < n; i++) {
    sum += rssiBuf[i];
  }
  return sum / n;
}

int argmax3(float y[3]) {
  int idx = 0;
  if (y[1] > y[idx]) idx = 1;
  if (y[2] > y[idx]) idx = 2;
  return idx;
}

const char* className(int c) {
  if (c == 0) return "Strong";
  if (c == 1) return "Moderate";
  return "Weak";
}

void showClassLED(int pred) {
  digitalWrite(LED_STRONG, LOW);
  digitalWrite(LED_MODERATE, LOW);
  digitalWrite(LED_WEAK, LOW);

  if (pred == 0) {
    digitalWrite(LED_STRONG, HIGH);
  } else if (pred == 1) {
    digitalWrite(LED_MODERATE, HIGH);
  } else {
    digitalWrite(LED_WEAK, HIGH);
  }
}

// ======================================================
// Forward pass: 1-8-3
// ======================================================
void nn_forward(float x, float hidden[8], float out[3]) {
  // Hidden layer
  for (int i = 0; i < 8; i++) {
    hidden[i] = relu(W1[i] * x + b1[i]);
  }

  // Output layer
  for (int k = 0; k < 3; k++) {
    out[k] = b2[k];
    for (int i = 0; i < 8; i++) {
      out[k] += W2[k][i] * hidden[i];
    }
  }
}

// ======================================================
// Wi-Fi connection helper
// ======================================================
void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to Wi-Fi");
  int retry = 0;

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    retry++;

    if (retry >= 60) {  // about 30 seconds
      Serial.println();
      Serial.println("Wi-Fi connection timeout. Restarting attempt...");
      WiFi.disconnect(true);
      delay(1000);
      WiFi.begin(ssid, password);
      retry = 0;
      Serial.print("Connecting to Wi-Fi");
    }
  }

  Serial.println();
  Serial.println("Wi-Fi connected");
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Initial RSSI: ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
  Serial.println("========================================");
}

// ======================================================
// Setup
// ======================================================
void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(LED_STRONG, OUTPUT);
  pinMode(LED_MODERATE, OUTPUT);
  pinMode(LED_WEAK, OUTPUT);

  digitalWrite(LED_STRONG, LOW);
  digitalWrite(LED_MODERATE, LOW);
  digitalWrite(LED_WEAK, LOW);

  Serial.println("========================================");
  Serial.println("ESP32 RSSI Classifier - Client/STA Mode");
  Serial.println("Plain-Code Neural Network: 1-8-3");
  Serial.println("Classes: Strong / Moderate / Weak");
  Serial.println("========================================");

  connectWiFi();
}

// ======================================================
// Main loop
// ======================================================
void loop() {
  // Reconnect if Wi-Fi drops
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi disconnected. Reconnecting...");
    connectWiFi();
  }

  // Read raw RSSI from connected AP
  int rssiRaw = WiFi.RSSI();

  // Smooth the RSSI value
  float rssiSmooth = smoothRSSI((float)rssiRaw);

  // Normalize for NN input
  float x = normalizeRSSI((int)rssiSmooth);

  // Run NN inference
  float hidden[8];
  float out[3];
  nn_forward(x, hidden, out);

  int pred = argmax3(out);

  // Update LEDs
  showClassLED(pred);

  // Serial output
  Serial.print("Raw RSSI = ");
  Serial.print(rssiRaw);
  Serial.print(" dBm");

  Serial.print(" | Smoothed RSSI = ");
  Serial.print(rssiSmooth, 2);
  Serial.print(" dBm");

  Serial.print(" | x = ");
  Serial.print(x, 3);

  Serial.print(" | Scores = [");
  Serial.print(out[0], 3);
  Serial.print(", ");
  Serial.print(out[1], 3);
  Serial.print(", ");
  Serial.print(out[2], 3);
  Serial.print("]");

  Serial.print(" | Class = ");
  Serial.println(className(pred));

  delay(2000);
}
