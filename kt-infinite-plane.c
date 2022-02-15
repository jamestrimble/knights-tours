//
// This program is based on kt.c
//
// In this version, I've used Donald Knuth's idea from the answer to exercise
// 284 in TAOCP vol 4, pre-fascicle 7a: the first knight is placed in a fixed
// square, and the region containing knights must have at most n rows and n
// columns.
//
// `trail` keeps track of the knights placed so far,and `bounds_trail`
// keeps track of the bounding box of the knights in the trail.
//
// This program is, very roughly, an order or magnitude slower than Knuth's.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define max_n 64
#define min_k 2
#define max_k 4

long search_calls;

int n, k, induced;  // command-line arguments

struct Pair
{
    int a;
    int b;
};

struct Bounds
{
    // left, right, top, bottom
    int l;
    int r;
    int t;
    int b;
};

bool square_used[max_n*2][max_n*2];
struct Pair trail[max_n * max_n];
struct Bounds bounds_trail[max_n * max_n];
int trail_len = 0;
int incumbent_len = 0;
struct Pair knights[max_k];
int knights_len = 0;

struct Pair moves[] = {
    {1, 2},
    {-1, -2},
    {-2, -1},
    {-2, 1},
    {-1, 2},
    {1, -2},
    {2, -1},
    {2, 1}
};
int moves_len = 8;

struct Pair sum(struct Pair p, struct Pair q)
{
    return (struct Pair) {p.a + q.a, p.b + q.b};
};

struct Pair difference(struct Pair p, struct Pair q)
{
    return (struct Pair) {p.a - q.a, p.b - q.b};
};

bool equals(struct Pair p, struct Pair q)
{
    return p.a == q.a && p.b == q.b;
};

bool get_square(struct Pair square)
{
    return square_used[square.a][square.b];
};

void set_square(struct Pair knight)
{
    square_used[knight.a][knight.b] = true;
    struct Bounds b;
    if (trail_len) {
        b = bounds_trail[trail_len - 1];
        if (knight.a < b.l) b.l = knight.a;
        if (knight.a > b.r) b.r = knight.a;
        if (knight.b < b.t) b.t = knight.b;
        if (knight.b > b.b) b.b = knight.b;
    } else {
        b = (struct Bounds) {knight.a, knight.a, knight.b, knight.b};
    }
    bounds_trail[trail_len] = b;
    trail[trail_len] = knight;
    ++trail_len;
}

void unset_square()
{
    struct Pair knight = trail[--trail_len];
    square_used[knight.a][knight.b] = false;
}

void debug()
{
    printf("------\n");
    for (int i=0; i<trail_len; i++) printf(" %d,%d", trail[i].a, trail[i].b);
    printf("\n");
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            printf("%d ", get_square((struct Pair) {i, j}));
        }
        printf("\n");
    }
    printf("------\n");
    printf("\n");
}

bool is_in_bounds(struct Pair square)
{
    if (trail_len == 0)
        return true;

    struct Bounds b = bounds_trail[trail_len - 1];
    if (square.a >= b.l + n) return false;
    if (square.a <= b.r - n) return false;
    if (square.b >= b.t + n) return false;
    if (square.b <= b.b - n) return false;

    return true;
}

bool is_valid_square(struct Pair knight)
{
    if (!is_in_bounds(knight)) return false;
    if (get_square(knight)) return false;
    if (induced) {
        for (int i=0; i<moves_len; i++) {
            struct Pair move = moves[i];
            struct Pair s = difference(knight, move);
            if (!is_in_bounds(s)) continue;
            if (!get_square(s)) continue;
            if (trail_len > 0 && equals(s, trail[trail_len - 1])) continue;
            if (trail_len >= k && equals(s, trail[trail_len - k])) continue;
            return false;
        }
    }
    return true;
};

void revert_knights(int count, struct Pair move)
{
    for (int i=0; i<count; i++) {
        unset_square();
        knights[i] = difference(knights[i], move);
    }
}

bool update_knights(struct Pair move)
{
    for (int i=0; i<k; i++) {
        struct Pair knight = knights[i];
        if (!is_valid_square(sum(knight, move))) {
            revert_knights(i, move);
            return false;
        }
        knights[i] = sum(knight, move);
        set_square(knights[i]);
    }
    return true;
}

void search(int tour_len)
{
    ++search_calls;
    if (tour_len > incumbent_len) {
        incumbent_len = tour_len;
        for (int i=0; i<trail_len; i++) printf(" %d,%d", trail[i].a, trail[i].b);
        printf("\n");
    }
    for (int i=0; i<moves_len; i++) {
        struct Pair move = moves[i];
        if (update_knights(move)) {
            search(tour_len + 1);
            revert_knights(k, move);
        }
    }
}

void place_m_knights_and_search()
{
    if (knights_len == k) {
        search(1);
    } else {
        for (int i=0; i<moves_len; i++) {
            if (knights_len == 1 && i != 0) {
                // Symmetry breaking; use move {1,2}
                continue;
            }
            struct Pair move = moves[i];
            struct Pair prev_knight = knights[knights_len - 1];
            struct Pair new_knight = sum(prev_knight, move);
            if (is_valid_square(new_knight)) {
                set_square(new_knight);
                knights[knights_len++] = new_knight;
                place_m_knights_and_search();
                --knights_len;
                unset_square();
            }
        }
    }
}

void find_maximum_tour()
{
    knights[knights_len++] = (struct Pair) {n-3, n-2};
    set_square((struct Pair) {n-3, n-2});
    place_m_knights_and_search();
    unset_square();
    --knights_len;
    printf("Max m is %d\n", incumbent_len);
}

int main(int argc, char *argv[])
{
    if (argc != 4 || sscanf(argv[1], "%d", &n) != 1
            || sscanf(argv[2], "%d", &k) != 1 || sscanf(argv[3], "%d", &induced) != 1) {
        fprintf(stderr, "Usage: %s n k induced    (where induced is 0 or 1)\n", argv[0]);
        exit(-1);
    }
    if (n < 3 || n > max_n) {
        fprintf(stderr, "n must be between 3 and %d!\n", max_n);
        exit(-2);
    }
    if (k < min_k || k > max_k) {
        fprintf(stderr, "k must be between %d and %d!\n", min_k, max_k);
        exit(-3);
    }
    printf("%dxm knights in a %dx%d square\n", k, n, n);
    find_maximum_tour();
    printf("%ld search() calls\n", search_calls);
}
