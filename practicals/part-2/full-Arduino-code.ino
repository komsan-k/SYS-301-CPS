#include <WiFi.h>

// =====================================
// Wi-Fi credentials
// =====================================
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// =====================================
// Neural Network 1–8–3
// Input: normalized RSSI
// Output: Strong / Moderate / Weak
// =====================================

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

// =====================================
// Helper functions
// =====================================
float relu(float x) {
  return (x > 0) ? x : 0;
}

float clamp01(float x) {
  if (x < 0) return 0;
  if (x > 1) return 1;
  return x;
}

// Normalize RSSI from [-90, -30] → [0,1]
float normalizeRSSI(int rssi) {
  return clamp01((rssi + 90.0) / 60.0);
}

// Find maximum output index
int argmax3(float y[3]) {
  int idx = 0;
  if (y[1] > y[idx]) idx = 1;
  if (y[2] > y[idx]) idx = 2;
  return idx;
}

// Convert class index to text
const char* className(int c) {
  if (c == 0) return "Strong";
  if (c == 1) return "Moderate";
  return "Weak";
}

// =====================================
// Forward propagation
// =====================================
void nn_forward(float x, float y[3]) {
  float h[8];

  // Hidden layer
  for (int i = 0; i < 8; i++) {
    h[i] = relu(W1[i] * x + b1[i]);
  }

  // Output layer
  for (int k = 0; k < 3; k++) {
    y[k] = b2[k];

    for (int i = 0; i < 8; i++) {
      y[k] += W2[k][i] * h[i];
    }
  }
}

// =====================================
// Setup
// =====================================
void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Wi-Fi connected");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// =====================================
// Main loop
// =====================================
void loop() {
  int rssi = WiFi.RSSI();

  float x = normalizeRSSI(rssi);

  float y[3];
  nn_forward(x, y);

  int pred = argmax3(y);

  Serial.print("RSSI = ");
  Serial.print(rssi);
  Serial.print(" dBm");

  Serial.print(" | x = ");
  Serial.print(x, 3);

  Serial.print(" | Class = ");
  Serial.println(className(pred));

  delay(2000);
}
