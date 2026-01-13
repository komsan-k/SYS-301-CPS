#ifndef YOUR_MODEL_INFERENCING_H
#define YOUR_MODEL_INFERENCING_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// =====================================================
// Mock TinyML Inference Header
// Compatible with ESP32 Arduino Lab
// Sensor → TinyML Model → Actuator
// =====================================================

// ---------- Status codes ----------
typedef enum {
    EI_IMPULSE_OK = 0,
    EI_IMPULSE_ERROR = 1
} EI_IMPULSE_ERROR;

// ---------- Signal abstraction ----------
namespace ei {
    typedef struct {
        size_t total_length;
        int (*get_data)(size_t offset, size_t length, float *out_ptr);
    } signal_t;
}

// ---------- Minimal numpy helper ----------
namespace numpy {

    static const float* g_buf = nullptr;
    static size_t g_len = 0;

    static int get_data_cb(size_t offset, size_t length, float *out_ptr) {
        if (!g_buf) return -1;
        if (offset + length > g_len) return -2;
        for (size_t i = 0; i < length; i++) {
            out_ptr[i] = g_buf[offset + i];
        }
        return 0;
    }

    static int signal_from_buffer(const float *buffer,
                                  size_t buffer_len,
                                  ei::signal_t *signal) {
        if (!buffer || !signal || buffer_len == 0) return -1;
        g_buf = buffer;
        g_len = buffer_len;
        signal->total_length = buffer_len;
        signal->get_data = get_data_cb;
        return 0;
    }
}

// ---------- Inference result ----------
typedef struct {
    float anomaly;   // anomaly score (0.0 – 1.0)
} ei_impulse_result_t;

// ---------- Mock classifier ----------
static EI_IMPULSE_ERROR run_classifier(ei::signal_t *signal,
                                       ei_impulse_result_t *result,
                                       bool debug) {
    (void)debug;

    if (!signal || !signal->get_data || !result) return EI_IMPULSE_ERROR;
    size_t N = signal->total_length;
    if (N == 0) return EI_IMPULSE_ERROR;

    float mean = 0.0f;
    float x;

    for (size_t i = 0; i < N; i++) {
        if (signal->get_data(i, 1, &x) != 0) return EI_IMPULSE_ERROR;
        mean += x;
    }
    mean /= (float)N;

    float var = 0.0f;
    for (size_t i = 0; i < N; i++) {
        if (signal->get_data(i, 1, &x) != 0) return EI_IMPULSE_ERROR;
        float d = x - mean;
        var += d * d;
    }
    var /= (float)N;

    // Normalize variance into pseudo anomaly score
    float score = var / (var + 50000.0f);
    if (score < 0.0f) score = 0.0f;
    if (score > 1.0f) score = 1.0f;

    result->anomaly = score;
    return EI_IMPULSE_OK;
}

#endif // YOUR_MODEL_INFERENCING_H
