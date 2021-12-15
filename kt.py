import json
import sys

class TourFinder(object):
    moves = [(-2,-1), (-2,1), (-1,-2), (-1,2), (1,-2), (1,2), (2,-1), (2,1)]

    def __init__(self, n, knights_len, k, induced):
        self.n = n
        self.knights_len = knights_len
        self.k = k
        self.induced = induced
        self.square_used = [[0] * n for _ in range(n)]

    def valid_position(self, knight, value):
        if self.induced:
            for move in TourFinder.moves:
                s0 = knight[0] - move[0]
                s1 = knight[1] - move[1]
                if s0 < 0 or s0 >= n or s1 < 0 or s1 >= n:
                    continue
                square = self.square_used[s0][s1]
                if not square:
                    continue
                if square == (value[0], value[1]-1):
                    continue
                if square == (value[0]-1, value[1]):
                    continue
                return False
        return (
            knight[0] >= 0 and
            knight[0] < n and
            knight[1] >= 0 and
            knight[1] < n and
            not self.square_used[knight[0]][knight[1]]
        )

    def revert_knights(self, knights, move):
        for knight in knights:
            self.square_used[knight[0]][knight[1]] = 0
            knight[0] -= move[0]
            knight[1] -= move[1]

    def update_knights(self, knights, move, value):
        for i, knight in enumerate(knights):
            if not self.valid_position([knight[0] + move[0], knight[1] + move[1]], (value, i)):
                self.revert_knights(knights[:i], move)
                return False
            knight[0] += move[0]
            knight[1] += move[1]
            self.set_square(knight, (value, i))
        return True

    def set_square(self, knight, value):
        self.square_used[knight[0]][knight[1]] = value

    def search(self, knights, tour_len):
        if tour_len == k:
            return True
        for move in TourFinder.moves:
            if self.update_knights(knights, move, tour_len + 1):
                if self.search(knights, tour_len + 1):
                    return True
                self.revert_knights(knights, move)
        return False

    def initial_knights(self, knights_len, knights):
        if len(knights) == 0:
            for i in range(n):
                for j in range(n):
                    self.set_square([i, j], (1, 0))
                    if knights_len == 1:
                        yield [[i, j]]
                    else:
                        yield from self.initial_knights(knights_len, [[i, j]])
                    self.square_used[i][j] = 0
        else:
            for move in TourFinder.moves if len(knights) > 1 else [(1, 2)]:
                prev_knight = knights[-1]
                new_knight = [prev_knight[0] + move[0], prev_knight[1] + move[1]]
                if self.valid_position(new_knight, (1, len(knights))):
                    self.set_square(new_knight, (1, len(knights)))
                    knights.append(new_knight)
                    if knights_len == len(knights):
                        yield knights
                    else:
                        yield from self.initial_knights(knights_len, knights)
                    self.square_used[new_knight[0]][new_knight[1]] = 0
                    del knights[-1]

    def find_tour(self):
        for knights in self.initial_knights(knights_len, []):
            if self.search(knights, 1):
                return self.square_used

def format_result(n, knights_len, k, induced, result):
    mat = [[None] * knights_len for _ in range(k)]
    for i in range(n):
        for j in range(n):
            d = result[i][j]
            if d != 0:
                mat[d[0]-1][d[1]] = (i, j)
    return {"n": n, "knights": knights_len, "k": k, "induced": induced, "tour": mat}

if __name__ == "__main__":
    n = int(sys.argv[1])
    knights_len = int(sys.argv[2])
    k = int(sys.argv[3])
    induced = len(sys.argv) > 4 and sys.argv[4] == "--induced"

    if k > 0:
        result = TourFinder(n, knights_len, k, induced).find_tour()
    else:
        # optimise
        result = None
        k = 1
        while True:
            result_ = TourFinder(n, knights_len, k, induced).find_tour()
            if result_ is None:
                k -= 1
                break
            else:
                result = result_
                k += 1
    if result:
        print(json.dumps(format_result(n, knights_len, k, induced, result)))
    else:
        print("No solution exists")
