/*
  Lab 2 (5C) — Conversion Layer
  Signal Conditioning + Feature Extraction

  - Reads ADC on GPIO34
  - Applies moving-average-like filter (small K MA)
  - Computes window features:
      avg, std, trend
  - Prints CSV:
      ms,raw,filt,avg,std,trend,calMin,calMax
*/

#include <Arduino.h>
#include <math.h>

static const int PIN_ADC = 34;

static const uint32_t SAMPLE_MS = 50;     // sampling period
static const uint32_t WINDOW_MS = 1000;   // feature window period
static const int WINDOW_SAMPLES = WINDOW_MS / SAMPLE_MS;

static int rawBuf[WINDOW_SAMPLES];
static float filtBuf[WINDOW_SAMPLES];

static int idx = 0;
static bool filled = false;

// Calibration
static int calMin = 4095;
static int calMax = 0;

void setup() {
  Serial.begin(115200);
  delay(300);

  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);

  for (int i = 0; i < WINDOW_SAMPLES; i++) {
    rawBuf[i] = 0;
    filtBuf[i] = 0.0f;
  }

  Serial.println("Lab 2 — Conversion Layer started.");
  Serial.println("CSV: ms,raw,filt,avg,std,trend,calMin,calMax");
}

void loop() {
  static uint32_t lastSample = 0;
  static uint32_t lastWindow = 0;

  uint32_t now = millis();

  // Sampling
  if (now - lastSample >= SAMPLE_MS) {
    lastSample = now;

    int raw = analogRead(PIN_ADC);

    // calibration tracking
    if (raw < calMin) calMin = raw;
    if (raw > calMax) calMax = raw;

    rawBuf[idx] = raw;

    // Small-K moving average filter (K=5) for real-time smoothing
    const int K = 5;
    long sum = 0;
    for (int k = 0; k < K; k++) {
      int j = (idx - k + WINDOW_SAMPLES) % WINDOW_SAMPLES;
      sum += rawBuf[j];
    }
    float filt = (float)sum / (float)K;
    filtBuf[idx] = filt;

    idx++;
    if (idx >= WINDOW_SAMPLES) {
      idx = 0;
      filled = true;
    }
  }

  // Feature window report
  if (now - lastWindow >= WINDOW_MS) {
    lastWindow = now;

    int n = filled ? WINDOW_SAMPLES : idx;
    if (n < 5) return;

    // linearize ring buffer (oldest -> newest)
    static float window[WINDOW_SAMPLES];
    int start = filled ? idx : 0;
    for (int i = 0; i < n; i++) {
      int j = (start + i) % WINDOW_SAMPLES;
      window[i] = filtBuf[j];
    }

    // mean
    double mean = 0.0;
    for (int i = 0; i < n; i++) mean += window[i];
    mean /= (double)n;

    // std
    double acc = 0.0;
    for (int i = 0; i < n; i++) {
      double d = window[i] - mean;
      acc += d * d;
    }
    double stdv = sqrt(acc / (double)n);

    // trend
    double trend = window[n - 1] - window[0];

    // latest samples
    int lastIdx = (idx - 1 + WINDOW_SAMPLES) % WINDOW_SAMPLES;
    int rawLatest = rawBuf[lastIdx];
    float filtLatest = filtBuf[lastIdx];

    Serial.print(now);
    Serial.print(",");
    Serial.print(rawLatest);
    Serial.print(",");
    Serial.print(filtLatest, 2);
    Serial.print(",");
    Serial.print((float)mean, 2);
    Serial.print(",");
    Serial.print((float)stdv, 2);
    Serial.print(",");
    Serial.print((float)trend, 2);
    Serial.print(",");
    Serial.print(calMin);
    Serial.print(",");
    Serial.println(calMax);
  }
}
