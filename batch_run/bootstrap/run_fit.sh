#!/bin/bash

# read in arguments from command line (should do some sanity checking)
TAG=$1
ITER=$2

cd $DALITZ/macros
$DALITZ/DalitzExe/fitAmplitudesBootstrap /d/home/septian/Eta3PiDalitz/macros/cfgBootstrap/ana_2017_sbs_$TAG $ITER $ITER
mv -f dalitz_2017_data_sbs_${TAG}_${ITER}.fit fitBootstrap/