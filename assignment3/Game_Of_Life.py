import os
import subprocess
import re
import matplotlib.pyplot as plt
import numpy as np
import plotly.express as px
import plotly.io as pio
pio.renderers.default = "firefox"
import pandas as pd

with open('Game_Of_Life.txt') as f:
    data = ''.join(f.readlines())

patt = re.compile(r'OMP_NUM_THREADS=(?P<nthreads>\d+)\nGameOfLife: Size (?P<size>\d+) Steps (?P<steps>\d+) Time (?P<normaltime>\d*.\d*)\nGameOfLifeOMP: Size \d+ Steps \d+ Time (?P<OMPtime>\d*.\d*)')
data = [match.groupdict() for match in patt.finditer(data)]
df = pd.DataFrame().from_records(data)
types = {'nthreads': np.int64, 'size': np.int64, 'steps': np.int32, 'normaltime': np.float32, 'OMPtime':np.float32}
df = df.astype(types)
df['speedup'] = df['normaltime']/df['OMPtime']
print(df)

# for size in ['64', '1024', '4096']:
#     for T in ['1000', '2000']:
#         #  x axis should be the number of cores/threads, 
#         #  y axis the speedup you get
#         # sequential = [match for match in matches if match.group('nthreads') == '1']
#         # seq = next(filter(lambda match: match.group('size') == size 
#         #                  and match.group('steps') == T, sequentials), None)
#         # matches = [match for match in data if match.group('size') == size and match.group('steps') == T]
        
df['size-steps'] = df['size'].astype('str') + ", " + df['steps'].astype('str')
fig = px.line(df, x="nthreads", y="speedup", title='Speed-up compared to #threads', color='size-steps')
fig.update_yaxes(tick0=2, dtick=2)
fig.update_layout(
    xaxis = dict(
        tickmode = 'array',
        tickvals = [2, 4, 8, 16]
        # ticktext = ['One', 'Three', 'Five', 'Seven', 'Nine', 'Eleven']
    )
)


fig.write_html('first_figure.html', auto_open=True)
# print(data[0].group('size'))
# plt.legend()
# plt.show()