import os
import subprocess
import re
import matplotlib.pyplot as plt
import numpy as np
import plotly.express as px
import plotly.io as pio
pio.renderers.default = "firefox"
import pandas as pd
import plotly.graph_objects as go

with open('MatMul.txt') as f:
    data = ''.join(f.readlines())

patt = re.compile(r'OMP_NUM_THREADS=(?P<num_threads>\d+)\nCollapsenum=0, time=(?P<time_no_collapse>\d+.\d+)\nCollapsenum=1, time=(?P<time_collapse_1>\d+.\d+)\nCollapsenum=(\d+), time=(?P<time_collapse_2>\d+.\d+)\nCollapsenum=(\d+), time=(?P<time_collapse_3>\d+.\d+)')
data = [match.groupdict() for match in patt.finditer(data)]
df = pd.DataFrame().from_records(data)
types = {'num_threads': np.int64, 'time_no_collapse': np.float32, 'time_collapse_1': np.float32, 'time_collapse_2': np.float32, 'time_collapse_1':np.float32}
df = df.astype(types)
df = df.set_index('num_threads')

# df['speedup'] = df['normaltime']/df['OMPtime']
# print(df)

# # for size in ['64', '1024', '4096']:
# #     for T in ['1000', '2000']:
# #         #  x axis should be the number of cores/threads, 
# #         #  y axis the speedup you get
# #         # sequential = [match for match in matches if match.group('nthreads') == '1']
# #         # seq = next(filter(lambda match: match.group('size') == size 
# #         #                  and match.group('steps') == T, sequentials), None)
# #         # matches = [match for match in data if match.group('size') == size and match.group('steps') == T]
        
# df['size-steps'] = df['size'].astype('str') + ", " + df['steps'].astype('str')
# fig = px.line(df, x="nthreads", y="speedup", title='Speed-up compared to #threads', color='size-steps')
# fig.update_yaxes(tick0=2, dtick=2)
fig = go.Figure()
fig.update_layout(title_text="Matrix Multiplication calculation time", 
                  xaxis_title="Number of threads", 
                  yaxis_title="Calculation time(seconds)"
)
fig.update_layout(
    xaxis = dict(
        tickmode = 'array',
        tickvals = [1, 2, 4, 8, 16]
        # ticktext = ['One', 'Three', 'Five', 'Seven', 'Nine', 'Eleven']
    )
)
for col in df.columns:
    fig.add_trace(go.Scatter(
    x=[1,2,4,8,16], y=df[col],
    name=col
    # line=dict(width=max_thickness - (i*10) ) 
    ))
# print(df)
fig.write_html('first_figure.html', auto_open=True)
# # print(data[0].group('size'))
# # plt.legend()
# # plt.show()