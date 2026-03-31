import numpy as np
import tensorflow as tf

# =========================================================
# 1) Create example RSSI dataset
#    Classes:
#    0 = Strong
#    1 = Moderate
#    2 = Weak
# =========================================================
np.random.seed(42)
tf.random.set_seed(42)

n_per_class = 400

# Strong: around -55 to -30 dBm
rssi_strong = np.random.uniform(-55, -30, n_per_class)

# Moderate: around -75 to -56 dBm
rssi_moderate = np.random.uniform(-75, -56, n_per_class)

# Weak: around -100 to -76 dBm
rssi_weak = np.random.uniform(-100, -76, n_per_class)

X = np.concatenate([rssi_strong, rssi_moderate, rssi_weak]).astype(np.float32)
y = np.concatenate([
    np.zeros(n_per_class, dtype=np.int32),
    np.ones(n_per_class, dtype=np.int32),
    np.full(n_per_class, 2, dtype=np.int32)
])

# Shuffle
idx = np.arange(len(X))
np.random.shuffle(idx)
X = X[idx]
y = y[idx]

# =========================================================
# 2) Normalize RSSI to [0,1]
#    same formula as Arduino:
#    x = (rssi + 100) / 70
# =========================================================
Xn = (X + 100.0) / 70.0
Xn = np.clip(Xn, 0.0, 1.0)

# Reshape to [samples, 1]
Xn = Xn.reshape(-1, 1)

# Split train/test
split = int(0.8 * len(Xn))
X_train, X_test = Xn[:split], Xn[split:]
y_train, y_test = y[:split], y[split:]

# =========================================================
# 3) Build 1-8-3 neural network
# =========================================================
model = tf.keras.Sequential([
    tf.keras.layers.Input(shape=(1,)),
    tf.keras.layers.Dense(8, activation='sigmoid', name='hidden'),
    tf.keras.layers.Dense(3, activation='softmax', name='output')
])

model.compile(
    optimizer=tf.keras.optimizers.Adam(learning_rate=0.01),
    loss='sparse_categorical_crossentropy',
    metrics=['accuracy']
)

model.summary()

# =========================================================
# 4) Train
# =========================================================
history = model.fit(
    X_train, y_train,
    validation_split=0.2,
    epochs=80,
    batch_size=32,
    verbose=1
)

# =========================================================
# 5) Evaluate
# =========================================================
loss, acc = model.evaluate(X_test, y_test, verbose=0)
print(f"\nTest accuracy: {acc:.4f}")

# =========================================================
# 6) Show predictions for a few RSSI values
# =========================================================
sample_rssi = np.array([-40, -62, -85], dtype=np.float32)
sample_x = np.clip((sample_rssi + 100.0) / 70.0, 0.0, 1.0).reshape(-1, 1)

pred = model.predict(sample_x)

labels = ["Strong", "Moderate", "Weak"]
for rssi, p in zip(sample_rssi, pred):
    cls = np.argmax(p)
    print(f"RSSI = {rssi:5.1f} dBm -> {labels[cls]} | probs = {p}")

# =========================================================
# 7) Extract weights for Arduino
# =========================================================
W1, b1 = model.get_layer("hidden").get_weights()   # W1 shape: (1, 8)
W2, b2 = model.get_layer("output").get_weights()   # W2 shape: (8, 3)

print("\n=== Arduino-ready weights ===")

print("\n// Input -> Hidden weights (8)")
print("float W1[8] = {")
print(", ".join([f"{v:.8f}" for v in W1[:, 0] if False]), end="")
print(", ".join([f"{v:.8f}" for v in W1[0]]))
print("};")

print("\n// Hidden biases (8)")
print("float b1[8] = {")
print(", ".join([f"{v:.8f}" for v in b1]))
print("};")

print("\n// Hidden -> Output weights (3x8)")
print("float W2[3][8] = {")
for k in range(3):
    row = [W2[j][k] for j in range(8)]   # transpose for Arduino layout
    print("  {" + ", ".join([f"{v:.8f}" for v in row]) + "}" + ("," if k < 2 else ""))
print("};")

print("\n// Output biases (3)")
print("float b2[3] = {")
print(", ".join([f"{v:.8f}" for v in b2]))
print("};")
