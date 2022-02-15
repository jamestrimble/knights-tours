//
// This program is a fairly direct translation of kt.py
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

bool square_used[max_n][max_n];
struct Pair trail[max_n * max_n];
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

void debug()
{
    printf("------\n");
    for (int i=0; i<trail_len; i++) printf(" %d,%d", trail[i].a, trail[i].b);
    printf("\n");
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            printf("%d ", square_used[i][j]);
        }
        printf("\n");
    }
    printf("------\n");
    printf("\n");
}

bool is_valid_square(struct Pair knight)
{
    if (knight.a < 0 || knight.a >= n || knight.b < 0 || knight.b >= n) return false;
    if (square_used[knight.a][knight.b]) return false;
    if (induced) {
        for (int i=0; i<moves_len; i++) {
            struct Pair move = moves[i];
            struct Pair s = difference(knight, move);
            if (s.a < 0 || s.a >= n || s.b < 0 || s.b >= n) continue;
            if (!square_used[s.a][s.b]) continue;
            if (trail_len > 0 && equals(s, trail[trail_len - 1])) continue;
            if (trail_len >= k && equals(s, trail[trail_len - k])) continue;
            return false;
        }
    }
    return true;
};

void set_square(struct Pair knight)
{
    square_used[knight.a][knight.b] = true;
    trail[trail_len++] = knight;
}

void unset_square()
{
    struct Pair knight = trail[--trail_len];
    square_used[knight.a][knight.b] = false;
}

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
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            knights[knights_len++] = (struct Pair) {i, j};
            set_square((struct Pair) {i, j});
            place_m_knights_and_search();
            unset_square();
            --knights_len;
        }
    }
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
