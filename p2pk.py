import sys

n = int(sys.argv[1])
knights_len = int(sys.argv[2])
k = int(sys.argv[3])

moves = [(-2,-1), (-2,1), (-1,-2), (-1,2), (1,-2), (1,2), (2,-1), (2,1)]

def valid_position(knight, square_used):
    return (
        knight[0] >= 0 and
        knight[0] < n and
        knight[1] >= 0 and
        knight[1] < n and
        not square_used[knight[0]][knight[1]]
    )

def revert_knights(knights, move, square_used):
    for knight in knights:
        square_used[knight[0]][knight[1]] = 0
        knight[0] -= move[0]
        knight[1] -= move[1]

def update_knights(knights, move, square_used, value):
    for i, knight in enumerate(knights):
        if not valid_position([knight[0] + move[0], knight[1] + move[1]], square_used):
            revert_knights(knights[:i], move, square_used)
            return False
        knight[0] += move[0]
        knight[1] += move[1]
        set_square(square_used, knight, (value, i))
    return True

def set_square(square_used, knight, value):
    square_used[knight[0]][knight[1]] = value

def search(knights, square_used, tour_len):
    if tour_len == k:
        return True
    for move in moves:
        if update_knights(knights, move, square_used, tour_len + 1):
            if search(knights, square_used, tour_len + 1):
                return True
            revert_knights(knights, move, square_used)
    return False

def initial_knights(square_used, knights_len, knights):
    if len(knights) == 0:
        for i in range(n):
            for j in range(n):
                set_square(square_used, [i, j], (1, 0))
                if knights_len == 1:
                    yield [i, j]
                else:
                    yield from initial_knights(square_used, knights_len, [[i, j]])
                square_used[i][j] = 0
    else:
        for move in moves if len(knights) > 1 else [(1, 2)]:
            prev_knight = knights[-1]
            new_knight = [prev_knight[0] + move[0], prev_knight[1] + move[1]]
            if valid_position(new_knight, square_used):
                knights.append(new_knight)
                set_square(square_used, new_knight, (1, len(knights)))
                if knights_len == len(knights):
                    yield knights
                else:
                    yield from initial_knights(square_used, knights_len, knights)
                square_used[new_knight[0]][new_knight[1]] = 0
                del knights[-1]

def find_tour():
    square_used = [[0] * n for _ in range(n)]
    for knights in initial_knights(square_used, knights_len, []):
        if search(knights, square_used, 1):
            return square_used

result = find_tour()
if result:
    for row in result:
        print(' '.join('{: >7}'.format(str(x)) for x in row))
else:
    print("No solution exists")
