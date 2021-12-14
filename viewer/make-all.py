import json
import sys

all_data = []

for line in sys.stdin:
    tokens = line.strip().split()
    filename = "data/{}-{}.json".format(tokens[0], tokens[1])
    with open(filename, "r") as f:
        d = json.load(f)
        all_data.append({"name": ",".join(tokens), "data": d})

print(json.dumps(all_data))
