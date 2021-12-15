import sys

if sys.argv[1] == "grid":
    m = int(sys.argv[2])
    n = int(sys.argv[3])
    filename = "../lad-graphs/grid{}-{}.lad".format(m, n)
    G = graphs.Grid2dGraph(m, n)
elif sys.argv[1] == "knight":
    n = int(sys.argv[2])
    filename = "../lad-graphs/knight{}.lad".format(n)
    G = graphs.KnightGraph([n, n])
else:
    sys.exit("Unexpected graph type")

with open(filename, "r") as f:
    for i, line in enumerate(f):
        line = [int(x) for x in line.strip().split()]
        if i == 0:
            lad_n = int(line[0])
            d = {}
        else:
            assert len(line) == line[0] + 1
            d[i-1] = line[1:]
    assert i == lad_n

H = Graph(d)

if G.is_isomorphic(H):
    print("Isomorphic :-)")
else:
    print("Not isomorphic :-(")
