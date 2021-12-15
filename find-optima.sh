#!/bin/bash

# non-induced

TIME_LIMIT=10

(
for knights in $(seq 2 2); do
    for n in $(seq 8 9); do
        timeout $TIME_LIMIT pypy3 kt.py $n $knights -1 | jq -r '[.n,.knights,.k,.induced] | @tsv'
    done
done

for knights in $(seq 3 4); do
    for n in $(seq 8 15); do
        timeout $TIME_LIMIT pypy3 kt.py $n $knights -1 | jq -r '[.n,.knights,.k,.induced] | @tsv'
    done
done

# induced

for knights in $(seq 2 2); do
    for n in $(seq 8 12); do
        timeout $TIME_LIMIT pypy3 kt.py $n $knights -1 --induced | jq -r '[.n,.knights,.k,.induced] | @tsv'
    done
done

for knights in $(seq 3 4); do
    for n in $(seq 8 20); do
        timeout $TIME_LIMIT pypy3 kt.py $n $knights -1 --induced | jq -r '[.n,.knights,.k,.induced] | @tsv'
    done
done
) | tee some-results.txt
