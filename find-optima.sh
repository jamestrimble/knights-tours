#!/bin/bash

set -o pipefail

mkdir -p results

TIME_LIMIT=36000

task(){
    echo $1 $2 $3
    local knights=$1
    local induced=$2
    local TIME_LIMIT=$3
    for n in $(seq 8 40); do
        if ! timeout $TIME_LIMIT pypy3 kt.py $n $knights -1 $induced | jq -r '[.n,.knights,.k,.induced,.time] | @tsv'; then
            break
        fi
    done > results/$knights$induced.txt
}

echo a
for induced in '' '--induced'; do
    for knights in $(seq 2 4); do
        task "$knights" "$induced" "$TIME_LIMIT" &
    done
done
echo b

wait
echo c

rm -f some-results.txt

for induced in '' '--induced'; do
    for knights in $(seq 2 4); do
        cat results/$knights$induced.txt
    done
done > some-results.txt
