#!/bin/bash

set -eu

gcc -O3 -o kt{,.c}
gcc -O3 -o kt-infinite-plane{,.c}
