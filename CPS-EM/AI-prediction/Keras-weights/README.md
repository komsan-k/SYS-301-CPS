# Keras → Arduino Weights Auto-Converter

This package provides a ready-to-use Python script to convert a simple Keras MLP model into Arduino-compatible weight arrays.

It is designed for **plain-code neural networks** on ESP32 or Arduino, where you do not want to use TensorFlow Lite at runtime.

---

## What it does

The script:

- loads a Keras model (`.h5` or `.keras`)
- extracts weights and biases from the first two Dense layers
- transposes weight matrices into Arduino-friendly format
- exports a header file such as `nn_weights.h`

---

## Supported model structure

This script is intended for models such as:

```text
Input → Dense(hidden, relu) → Dense(output, softmax)
```

Examples:

- `1 → 8 → 3`
- `5 → 8 → 3`
- `7 → 8 → 3`

---

## File included

- `keras_to_arduino_nn.py`

---

## Python script

```python
import numpy as np
import tensorflow as tf
import sys

MODEL_PATH = "model.h5"
OUTPUT_PATH = "nn_weights.h"

if len(sys.argv) >= 2:
    MODEL_PATH = sys.argv[1]
if len(sys.argv) >= 3:
    OUTPUT_PATH = sys.argv[2]

model = tf.keras.models.load_model(MODEL_PATH)

# Assumes:
# Input -> Dense(hidden) -> Dense(output)
W1, b1 = model.layers[0].get_weights()
W2, b2 = model.layers[1].get_weights()

# Transpose for Arduino-style arrays:
# Keras: W1 = (input_dim, hidden_dim), W2 = (hidden_dim, output_dim)
# Arduino code here uses: W1[hidden][input], W2[output][hidden]
W1 = W1.T
W2 = W2.T

def format_1d(name, arr):
    s = f"const float {name}[{len(arr)}] = {{\n"
    s += "  " + ", ".join(f"{x:.8f}f" for x in arr) + "\n"
    s += "};\n\n"
    return s

def format_2d(name, arr):
    rows, cols = arr.shape
    s = f"const float {name}[{rows}][{cols}] = {{\n"
    for i in range(rows):
        row = ", ".join(f"{x:.8f}f" for x in arr[i])
        s += f"  {{ {row} }}"
        if i < rows - 1:
            s += ","
        s += "\n"
    s += "};\n\n"
    return s

header = ""
header += "#pragma once\n\n"
header += "// Auto-generated from Keras model\n"
header += f"// Source model: {MODEL_PATH}\n\n"
header += format_2d("W1", W1)
header += format_1d("b1", b1)
header += format_2d("W2", W2)
header += format_1d("b2", b2)

with open(OUTPUT_PATH, "w", encoding="utf-8") as f:
    f.write(header)

print(f"Saved Arduino weights to: {OUTPUT_PATH}")

```

---

## Usage

### Example 1: `.h5` model

```bash
python keras_to_arduino_nn.py model.h5 nn_weights.h
```

### Example 2: `.keras` model

```bash
python keras_to_arduino_nn.py model.keras nn_weights.h
```

Output:

```text
nn_weights.h
```

---

## Example output header

```cpp
#pragma once

const float W1[8][5] = {
  { 0.12345678f, -0.04567891f, 0.11111111f, 0.22222222f, -0.33333334f },
  { ... }
};

const float b1[8] = {
  0.01234567f, -0.02345678f, ...
};

const float W2[3][8] = {
  { 0.10101010f, -0.20202020f, ... },
  { ... },
  { ... }
};

const float b2[3] = {
  0.01010101f, -0.02020202f, 0.03030303f
};
```

---

## How to use in Arduino

In your `.ino` file:

```cpp
#include "nn_weights.h"
```

Then remove your manually typed `W1`, `b1`, `W2`, and `b2` arrays.

---

## Important note about shapes

Keras stores Dense weights as:

- `W1`: `(input_dim, hidden_dim)`
- `W2`: `(hidden_dim, output_dim)`

But many Arduino plain-code neural network implementations use:

- `W1[hidden][input]`
- `W2[output][hidden]`

So the script automatically applies transpose:

- `W1 = W1.T`
- `W2 = W2.T`

---

## Typical workflow

1. Train your Keras model
2. Save model as `.h5` or `.keras`
3. Run the converter script
4. Copy `nn_weights.h` into your Arduino sketch folder
5. Compile and run your plain-code neural network

---

## Recommended use cases

- ESP32 RSSI predictor
- LDR light classifier
- simple CPS edge AI experiments
- teaching neural network internals
- plain-code embedded inference

---

## Next extensions

A more advanced version could also:

- detect layer sizes automatically
- support multiple hidden layers
- generate `predict_nn()` automatically
- export a full `nn_model.h` file with both weights and inference functions

---
