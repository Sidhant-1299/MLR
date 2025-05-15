import pandas as pd

class MLRWrapper:
    """
        Wrapper for fit() using pandas dataframe
    """
    def __init__(self, df: pd.DataFrame, target_col: str):
        if not isinstance(df, pd.DataFrame):
            raise ValueError("Input df should be an instance of Pandas DataFrame")
        if not target_col in df.columns:
            raise ValueError("Target column not in dataframe")
        self.Y = df[target_col].to_numpy().reshape(-1,1) #figures out the rows and (-1) and it's length is one
        self.X = df.drop(columns=[target_col]).to_numpy() #DROP THE TARGET COLUMN
        self.predictors = [col for col in df.columns if col != target_col]
        self.target = target_col

    def fit(self):
        """
            Pass our C++ fit method
        """
        pass

    def predict(self):
        pass

    def summary(self):
        """
            Get the summary of our model
        """