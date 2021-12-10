import sys

m = int(sys.argv[1])
n = int(sys.argv[2])

print("p edge {} {}".format(n * m, 2 * m * n - m - n))
for i in range(m):
    for j in range(1, n):
        print("e {} {}".format(i * n + j, i * n + j + 1))

for i in range(m - 1):
    for j in range(n):
        print("e {} {}".format(i * n + j + 1, (i+1) * n + j + 1))
