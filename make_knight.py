import sys

n = int(sys.argv[1])

steps = [-2, -1, 1, 2]
moves = []
for x in steps:
    for y in steps:
        if abs(x) + abs(y) == 3:
            moves.append((x, y))

square_to_vertex = {}
v = 1
for i in range(n):
    for j in range(n):
        square_to_vertex[(i,j)] = v
        v += 1

edges = []
for i in range(n):
    for j in range(n):
        for x, y in moves:
            a = i + x
            b = j + y
            if a < 0 or a >= n:
                continue
            if b < 0 or b >= n:
                continue
            v = square_to_vertex[(i,j)]
            w = square_to_vertex[(a,b)]
            if v < w:
                edges.append((v, w))

print("p edge {} {}".format(n * n, len(edges)))
for v, w in edges:
    print("e {} {}".format(v, w))
