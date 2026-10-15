import tempfile
from pathlib import Path

import h5py
import numpy as np
import tensorflow as tf

model = tf.keras.Sequential([
    tf.keras.Input(shape=(2,)),
    tf.keras.layers.Dense(1, kernel_initializer="ones", bias_initializer="zeros"),
])
inputs = np.array([[1.0, 2.0]], dtype=np.float32)
np.testing.assert_allclose(model(inputs).numpy(), [[3.0]])

with tempfile.TemporaryDirectory() as directory:
    path = Path(directory) / "model.keras"
    model.save(path)
    restored = tf.keras.models.load_model(path)
    np.testing.assert_allclose(restored(inputs).numpy(), [[3.0]])

    with h5py.File(Path(directory) / "data.h5", "w") as data:
        data["inputs"] = inputs
        np.testing.assert_array_equal(data["inputs"][:], inputs)
