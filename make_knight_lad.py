import sys

def vertex(i, j):
    return i * n + j

n = int(sys.argv[1])

vtx_count = n * n

edges = [[] for _ in range(vtx_count)]

steps = [-2, -1, 1, 2]
moves = []
for x in steps:
    for y in steps:
        if abs(x) + abs(y) == 3:
            moves.append((x, y))

for i in range(n):
    for j in range(n):
        for x, y in moves:
            a = i + x
            b = j + y
            if a < 0 or a >= n:
                continue
            if b < 0 or b >= n:
                continue
            v = vertex(i,j)
            w = vertex(a,b)
            if v < w:
                edges[v].append(w)

print(vtx_count)

for row in edges:
    print("{}{}".format(len(row), "".join(" {}".format(w) for w in row)))
