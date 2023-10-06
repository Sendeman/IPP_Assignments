import plotly.express as px
import pandas as pd
import re
# import plotly.io as pio
def main():
    df = pd.read_csv("performance_exp.txt", index_col=1)
    print(df.to_latex())

main()