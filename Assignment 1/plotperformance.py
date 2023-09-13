import plotly.express as px
import pandas as pd
import re
# import plotly.io as pio
def main():
    plot_line()

def plot_bar():
    data = ""
    x = []
    y = []

    with open("performance_exp.txt") as f:
        for line in f:
            data += line

    patt = re.compile(r"T=(?P<T>\d+), N=(?P<N>\d+)\nFinished in (?P<seconds>\d+.\d+) seconds")

    for match in patt.finditer(data):
        x.append(f"T={match['T']}, N={match['N']}")
        y.append(float(match['seconds']))

    #Plotting BAR GRAPH
    df = pd.DataFrame(dict(
        experiment = x,
        time = y))
    print(df.to_latex())

    fig = px.bar(df, x = 'experiment', y = 'time',
                title = "Performance Measurements Graph",
                labels = {'experiment': 'experiment', 'time': 'runtime (in seconds)'})
    # fig.show()
    fig.write_image("performance_exp.png")
    # fig.write_html('fig.html', auto_open=True)

def plot_line():
    df = pd.read_csv('data.csv', index_col=0, header=0)
    fig = px.line(df, 
                  title="Performance Measurements for Threads")
    fig.update_layout(yaxis_title="Time(s)",
)

    fig.write_image("performance_exp_line.png")

main()