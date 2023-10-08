import subprocess
import re

values = [200000, 500000, 1_000_000, 5_000_000, 15_000_000, 20_000_000]
threads = [2**i for i in range(6)]

outputs = []

for thread in threads:
    for value in values:
        result = subprocess.check_output(
            f"./sieve_of_erastothenes {value} {thread}".split(" "), text=True
        )

        line = result.splitlines().pop().strip()
        time = re.search(r"Executed in: (\d*)", line)
        output = f"{value}, {thread}, {time.group(1)}"
        print(output)
        outputs.append(output)

with open('sieve_data.csv') as file:
    file.writelines(outputs)
