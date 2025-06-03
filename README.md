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
#### intercepts

```
mlr_cpp: 3.9882752613173915
sklearn: 3.988275261317881
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
