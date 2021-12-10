import sys

n = int(sys.argv[1])
k = int(sys.argv[2])

steps = [-2, -1, 1, 2]
moves = []
for x in steps:
    for y in steps:
        if abs(x) + abs(y) == 3:
            moves.append((x, y))

def update_knights(knights, move):
    for knight in knights:
        knight[0] += move[0]
        knight[1] += move[1]

def revert_knights(knights, move):
    for knight in knights:
        knight[0] -= move[0]
        knight[1] -= move[1]

def valid_positions(knights, square_used):
    for knight in knights:
        if knight[0] < 0 or knight[0] >= n:
            return False
        if knight[1] < 0 or knight[1] >= n:
            return False
        if square_used[knight[0]][knight[1]]:
            return False
    return True

def set_square(square_used, knight, value):
    square_used[knight[0]][knight[1]] = value

def unset_square(square_used, knight):
    square_used[knight[0]][knight[1]] = 0

def search(knights, square_used, tour_len):
    if tour_len == k:
        return square_used
    for move in moves:
        update_knights(knights, move)
        if valid_positions(knights, square_used):
            tour_len += 1
            set_square(square_used, knights[0], tour_len)
            set_square(square_used, knights[1], tour_len + 1000)
            if search(knights, square_used, tour_len):
                return square_used
            unset_square(square_used, knights[0])
            unset_square(square_used, knights[1])
            tour_len -= 1
        revert_knights(knights, move)
    return False

def find_tour():
    square_used = [[0] * n for _ in range(n)]
    for i in range(1, n):
        for j in range(2, n):
            knights = [[i-1, j-2], [i, j]]
            set_square(square_used, knights[0], 1)
            set_square(square_used, knights[1], 1001)
            result = search(knights, square_used, 1)
            if result:
                return result
            unset_square(square_used, knights[0])
            unset_square(square_used, knights[1])

result = find_tour()
if result:
    for row in result:
        print(' '.join('{: >3}'.format(str(x) + ' ' if x < 1000 else str(x-1000) + "'") for x in row))
else:
    print("No solution exists")
