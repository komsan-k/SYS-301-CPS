#include <WiFi.h>
#include <PubSubClient.h>
#include "TensorFlowLite.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"
#include "rssi_model_data.h"

const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";
const char* mqtt_server = "192.168.1.10";

WiFiClient espClient;
PubSubClient client(espClient);

tflite::MicroErrorReporter micro_error_reporter;
tflite::ErrorReporter* error_reporter = &micro_error_reporter;

const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;

constexpr int kTensorArenaSize = 10 * 1024;
static uint8_t tensor_arena[kTensorArenaSize];

TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;

float rssi_window[5] = {-60, -60, -60, -60, -60};

static inline int8_t clamp_int8(int v) {
  if (v < -128) return -128;
  if (v > 127) return 127;
  return (int8_t)v;
}

const char* label_from_idx(int idx) {
  switch (idx) {
    case 0: return "STRONG";
    case 1: return "MEDIUM";
    case 2: return "WEAK";
    default: return "UNKNOWN";
  }
}

int argmax3(const float p[3]) {
  int best = 0;
  if (p[1] > p[best]) best = 1;
  if (p[2] > p[best]) best = 2;
  return best;
}

void setup_wifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void setup_tflm() {
  model = tflite::GetModel(g_model);

  static tflite::MicroMutableOpResolver<3> resolver;
  resolver.AddFullyConnected();
  resolver.AddRelu();
  resolver.AddSoftmax();

  static tflite::MicroInterpreter static_interpreter(
      model, resolver, tensor_arena, kTensorArenaSize, error_reporter);

  interpreter = &static_interpreter;
  interpreter->AllocateTensors();

  input = interpreter->input(0);
  output = interpreter->output(0);
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  setup_tflm();
}

void loop() {
  // shift window
  for (int i = 4; i > 0; i--) {
    rssi_window[i] = rssi_window[i - 1];
  }
  rssi_window[0] = WiFi.RSSI();

  // quantize 5 inputs
  const float in_scale = input->params.scale;
  const int in_zero = input->params.zero_point;

  for (int i = 0; i < 5; i++) {
    float x = (rssi_window[i] + 100.0f) / 70.0f;
    x = constrain(x, 0.0f, 1.0f);
    int q = (int)roundf(x / in_scale) + in_zero;
    input->data.int8[i] = clamp_int8(q);
  }

  interpreter->Invoke();

  const float out_scale = output->params.scale;
  const int out_zero = output->params.zero_point;

  float p[3];
  for (int i = 0; i < 3; i++) {
    p[i] = ((int)output->data.int8[i] - out_zero) * out_scale;
  }

  int cls = argmax3(p);
  const char* label = label_from_idx(cls);

  char payload[128];
  snprintf(payload, sizeof(payload),
           "{"rssi":%.1f,"predicted_state":"%s","p0":%.3f,"p1":%.3f,"p2":%.3f}",
           rssi_window[0], label, p[0], p[1], p[2]);

  client.publish("cps/lab/rssi/prediction", payload);
  Serial.println(payload);

  delay(500);
}
