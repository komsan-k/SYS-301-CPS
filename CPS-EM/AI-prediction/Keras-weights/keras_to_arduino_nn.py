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
