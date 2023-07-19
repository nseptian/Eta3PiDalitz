#!/bin/bash

echo ===files before we start===
ls -lh

# source environment
source /d/home/septian/.bashrc
source /d/home/septian/Eta3PiDalitz/set_ana_env.sh
# echo ===environment===
# printenv

echo ===run the fit!===
./run_fit.sh $1 $2

echo ===files when we finish===
ls -lh