import sys

def vertex(i, j):
    return i * n + j

m = int(sys.argv[1])
n = int(sys.argv[2])

vtx_count = m * n

edges = [[] for _ in range(vtx_count)]

for i in range(m):
    for j in range(1, n):
        v, w = sorted([vertex(i, j-1), vertex(i, j)])
        edges[v].append(w)

for i in range(1, m):
    for j in range(n):
        v, w = sorted([vertex(i-1, j), vertex(i, j)])
        edges[v].append(w)

print(vtx_count)

for row in edges:
    print("{}{}".format(len(row), "".join(" {}".format(w) for w in row)))
