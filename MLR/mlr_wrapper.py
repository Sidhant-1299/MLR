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
        
        self.Y = df[[target_col]].to_numpy()
        self.X = df.drop(columns=[target_col]).to_numpy()

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
            raise ValueError("Model not fit yet")
        if not isinstance(x, pd.DataFrame):
            raise ValueError("Predictor is not a Pandas DataFrame")
        
        if x.shape[1] != self.X.shape[1]: #check if the data has the same number of columns as the data
            raise ValueError("Data has different num of parameters than model parameters")
        
        return self.model.predict(x.to_numpy())

    def get_coefficients(self):
        """
            Get the summary of our model
        """
        return self.model.coefficients()