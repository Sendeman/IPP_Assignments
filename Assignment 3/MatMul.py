import os
import subprocess


threadnum = 1
os.putenv("OMP_NUM_THREADS", str(threadnum))
result = subprocess.run(['./MatMul'], stdout=subprocess.PIPE)
print(result.stdout)
print(result.stderr)

# for t, threadnum in enumerate([1,2,4,8,16]):
#     os.putenv("OMP_NUM_THREADS", str(threadnum))
#     result = subprocess.run(['MatMul'], stdout=subprocess.PIPE)
#     print(result.stdout)
