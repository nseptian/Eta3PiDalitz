#!/bin/bash

# read in arguments from command line (should do some sanity checking)
TAG=$1
ITER=$2

$DALITZ/DalitzExe/fitAmplitudesBootstrap /d/home/septian/Eta3PiDalitz/macros/cfgBootstrap/ana_all_$TAG $ITER $ITER
