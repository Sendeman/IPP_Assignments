import subprocess
import re

values = [200000, 500000, 1_000_000, 5_000_000, 15_000_000, 20_000_000]
threads = [2**i for i in range(6)]


for thread in threads:
    for value in values:
        result = subprocess.check_output(
            f"./sieve_of_erastothenes {value} {thread}".split(" "), text=True
        )

        line = result.splitlines().pop().strip()
        time = re.search(r"Executed in: (\d*)", line)
        print(f"{value}, {thread}, {time.group(1)}")
