import subprocess
import re

threads = [2**i for i in range(2)]
execs = [
    # "std_fgl_benchmark",
    "std_cgl_benchmark",
    "mine_cgl_benchmark",
    # "mine_fgl_benchmark",
]

for exec in execs:
    for thread in threads:
        result = None
        try:
            proc = subprocess.run(
                f"./{exec} {thread}".split(" "),
                stdout=subprocess.PIPE,
                timeout=20,
                text=True,
            )
            result = proc.stdout
        except subprocess.TimeoutExpired as time_err:
            raise time_err
        
        for line in result.splitlines():
            line = line.strip()
            operation = re.search(r".*hread-safe (\w*)", line)
            time = line[line.rfind(" ") + 1 :]
            print(f"{exec}, {operation.group(1)}, {thread}, {time}")
