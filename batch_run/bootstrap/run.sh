#!/bin/bash

echo ===files before we start===
ls -lh

source env.sh
echo ===environment===
printenv

echo ===run the fit!===
./run_fit.sh $1 $2

echo ===files when we finish===
ls -lh

