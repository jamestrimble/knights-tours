#!/bin/bash
set -euo pipefail

cat instances.txt | while read a b; do
    echo $a $b
    echo "pypy3 ../kt.py $a $b -1 > data/$a-$b.json"
    pypy3 ../kt.py $a $b -1 > data/$a-$b.json
done

python3 make-all.py < instances.txt > all.json
