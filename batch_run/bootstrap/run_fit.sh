#!/bin/bash

# read in arguments from command line (should do some sanity checking)
TAG=$1
ITER=$2

cd $DALITZ/run/
$DALITZ/DalitzExe/fitAmplitudesBootstrap.exe /d/home/septian/Eta3PiDalitz/run/cfgBootstrap/ana_2017_${TAG} $ITER $ITER
# mv -f dalitz_2017_data_reweightedMC_${TAG}_0_${ITER}.fit fitBootstrap/
