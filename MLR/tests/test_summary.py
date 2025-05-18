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
    # print(eqn)
    assert isinstance(eqn ,str)

def test_equation_with_small_regressor(sample_data_2):
    model = MLRWrapper(sample_data_2, 'y')
    model.fit()
    eqn = model.get_eqn()
    # print(eqn)
    assert isinstance(eqn ,str)

def test_residuals_shapes(sample_data):
    model = MLRWrapper(sample_data, 'y')
    model.fit()

    residuals = model.get_residuals()

    # Check residuals shape
    assert residuals.shape == model.Y.shape


def test_residuals_values(sample_data):
    model = MLRWrapper(sample_data, 'y')
    model.fit()
    
    residuals = model.get_residuals()
    model_X = sample_data.drop(columns=['y']) #should be a dataframe
    # Check values match y - y_pred
    expected_residuals = model.Y - model.predict(model_X)
    # print(residuals.flatten())
    assert np.allclose(residuals, expected_residuals, atol=1e-6)


def test_get_RSS_and_TSS_after_fit(sample_data):
    model = MLRWrapper(sample_data, "y")
    model.fit()
    rss = model.get_RSS()
    tss = model.get_TSS()
    # RSS should be >= 0, same for TSS
    assert isinstance(rss, float) or isinstance(rss, np.float64) 
    assert isinstance(tss, float) or isinstance(tss, np.float64)
    assert rss >= 0
    assert tss >= 0

def test_get_RSS_and_TSS_before_fit_raises(sample_data):
    model = MLRWrapper(sample_data, "y")
    with pytest.raises(ValueError, match="Model not fit yet"):
        model.get_RSS()
    with pytest.raises(ValueError, match="Model not fit yet"):
        model.get_TSS()



def test_r_squared_raises_before_fit(sample_data):
    model = MLRWrapper(sample_data, target_col="y")
    with pytest.raises(ValueError, match="Model not fit yet"):
        model.get_Rsquared()




def test_r_squared(sample_data):
    model = MLRWrapper(sample_data, target_col="y")
    model.fit()
    r2 = model.get_Rsquared()
    # print(r2)
    # In this case, data is perfectly linear => R^2 should be ~1.0
    assert isinstance(r2, float) or isinstance(r2, np.float64)
    assert np.isclose(r2, 1.0, atol=1e-10)