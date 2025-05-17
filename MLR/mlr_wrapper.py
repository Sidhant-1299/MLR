import numpy
import pandas as pd
import mlr_cpp


class MLRWrapper:
    """
        Wrapper for fit() using pandas dataframe
    """
    def __init__(self, df: pd.DataFrame, target_col: str):
        if not isinstance(df, pd.DataFrame):
            raise ValueError("Input df should be an instance of Pandas DataFrame")
        if not target_col in df.columns:
            raise ValueError("Target column not in dataframe")
        self.Y = df[[target_col]].to_numpy(dtype=numpy.float64)
        self.X = df.drop(columns=[target_col]).to_numpy(dtype=numpy.float64) #DROP THE TARGET COLUMN
        self.predictors = [col for col in df.columns if col != target_col]
        self.target = target_col
        self.fitted = False
        self.model = mlr_cpp.MLR()

    def fit(self):
        """
            Pass our C++ fit method
        """
        self.model.fit(self.X,self.Y)
        self.fitted = True

    def predict(self, x):
        if not self.fitted:
            assert ValueError("Model not fit yet")
        if not isinstance(x, pd.DataFrame):
            assert ValueError("Predictor is not a Pandas DataFrame")
        #might have to change x shape if it is of shape (rows,)
        return self.model.predict(x.to_numpy())

    def get_coefficients(self):
        """
            Get the summary of our model
        """
        return self.model.coefficients()