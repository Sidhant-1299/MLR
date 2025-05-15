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
        self.X = df[target_col].to_numpy()
        self.Y = df.drop(columns=[target_col]).to_numpy() #DROP THE TARGET COLUMN
