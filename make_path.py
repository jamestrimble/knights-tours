import sys

n = int(sys.argv[1])

print("p edge {} {}".format(n, n-1))
for v in range(1, n):
    print("e {} {}".format(v, v + 1))
