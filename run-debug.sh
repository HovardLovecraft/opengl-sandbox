#!/bin/bash
set -euo pipefail

cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/sandbox
