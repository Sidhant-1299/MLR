import pandas as pd
from mlr_wrapper import MLRWrapper

df = pd.DataFrame({
    "x1": [1, 3, 5],
    "x2": [0, 4, 6],
    "y": [6, 16, 22]
})

model = MLRWrapper(df, target_col="y")

import io
import contextlib

f = io.StringIO()
with contextlib.redirect_stdout(f):
    model.fit()
print("Captured:\n", f.getvalue())
