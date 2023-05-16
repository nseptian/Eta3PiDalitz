#!/bin/bash

# read in arguments from command line (should do some sanity checking)
TAG=$1
ITER=$2

cd $DALITZ/macros
$DALITZ/DalitzExe/fitAmplitudesBootstrap.exe /d/home/septian/Eta3PiDalitz/run/cfgBootstrap/ana_all_$TAG $ITER $ITER
mv -f dalitz_all_data_${TAG}_${ITER}.fit fitBootstrap/