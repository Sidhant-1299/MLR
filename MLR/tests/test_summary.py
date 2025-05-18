from MLR.mlr_wrapper import MLRWrapper
import pytest
import pandas as pd
import numpy as np

@pytest.fixture
def sample_data():
    return pd.DataFrame({
        "x1": [1, 2, 3, 4],
        "x2": [5, 6, 0, 8],
        "y": [14, 17, 6, 23] #3 + x1 + 2*x2
    })


@pytest.fixture
def sample_data_2():
    x1 = np.array([1,2,3,4])
    x2 = np.array([3,4,1,2])
    x3 = np.array([2,3,5,7])
    y = 1 + x1 + 2*x2 + 0.0021*x3
    return pd.DataFrame({'x1':x1, 'x2':x2, "x3":x3, 'y':y})

def test_equation_returns_str(sample_data):
    model = MLRWrapper(sample_data, 'y')
    model.fit()
    eqn = model.get_eqn()
    print(eqn)
    assert isinstance(eqn ,str)

def test_equation_with_small_regressor(sample_data_2):
    model = MLRWrapper(sample_data_2, 'y')
    model.fit()
    eqn = model.get_eqn()
    print(eqn)
    assert isinstance(eqn ,str)
