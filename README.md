# mlr_cpp – Fast C++ Multiple Linear Regression with Python API

`mlr_cpp` is a high-performance Multiple Linear Regression library implemented in C++ and exposed to Python using `pybind11`. It is designed for speed, delivering results over **10x faster** than `scikit-learn`, while providing a user-friendly `pandas`-compatible API.

---

## Key Features

- **Blazing fast** C++ core, with Pybind11 for seamless Python integration  
- Supports full model diagnostics: R², Adjusted R², MAE, MSE, F-test, T-statistics, P-values  
- Automatically checks for multicollinearity and sufficient data  
- Equation generation with smart rounding and absolute tolerance  
- Compatible with `pandas` DataFrames  

---

## Benchmarks with sklearn

#### coefficients
```
mlr_cpp: [3.98827526e+00 3.98919944e+00 8.77169801e+00 3.74320006e+00
          3.38302087e-04 1.10138110e-02]
sklearn: [3.98919944e+00 8.77169801e+00 3.74320006e+00 3.38302087e-04
          1.10138110e-02]
```

#### Runtimes

```
mlr_cpp: 0.000344s
sklearn: 0.004404s
```

#### $R^2$ Scores

```
mlr_cpp: 0.987661
sklearn: 0.987661
```

`mlr_cpp` provides identical accuracy while being ~10x faster than `scikit-learn`


##  Installation

```bash
pip install mlr_cpp
```

#### Optional Extras
```bash
pip install mlr_cpp[examples]  # for examples, plotting
pip install mlr_cpp[dev]       # for development tools
```


## Example Usage

```python
import pandas as pd
from mlr_wrapper import MLRWrapper

df = pd.read_csv("your_data.csv")
model = MLRWrapper(df, target_col="mpg")

model.fit()
eqn, predictors, tests = model.get_model_summary(tstats=True)

print(eqn)
print(predictors)
print(tests)
```

### Prediction
```python
model.predict(new_df)
```

### Model Summary

You can retrieve a complete summary of the model including the regression equation, coefficients, p-values, and test metrics:

```python
eqn, predictor_summary, model_tests = model.get_model_summary(tstats=True)

print(eqn)                  # Prints the regression equation
print(predictor_summary)    # DataFrame with coefficients, p-values, and t-statistics
print(model_tests)          # DataFrame with metrics like R², MAE, MSE, etc.
```

### Evaluation Metrics

After fitting the model, `mlr_cpp` provides access to standard evaluation metrics for performance diagnostics:

```python
model.get_R2()           # Coefficient of Determination (R²)
model.get_AdjustedR2()   # Adjusted R²
model.get_MAE()          # Mean Absolute Error
model.get_MSE()          # Mean Squared Error
model.get_ftest()        # F-statistic of the regression model
model.get_TStatistics()  # T-statistics for each predictor
model.get_PValues()      # P-values corresponding to each predictor
model.get_model_tests()  # Summary DataFrame containing major evaluation metrics
```

## 👨‍💻 Author

### Sidhant Raj Khati

🌐︎ Website: [sidhantkhati.com](https://sidhantkhati.com)\
💼 LinkedIn: [LinkedIn](https://www.linkedin.com/in/sidhant-raj-khati-728086245/)\
🔗 GitHub: [Sidhant-1299/mlr_cpp](https://github.com/Sidhant-1299/mlr_cpp)


## 📄 License

This project is licensed under the **MIT License**

