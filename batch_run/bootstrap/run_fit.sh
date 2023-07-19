#!/bin/bash

# read in arguments from command line (should do some sanity checking)
TAG=$1
ITER=$2

cd $DALITZ/run/
$DALITZ/DalitzExe/fitAmplitudesBootstrap.exe /d/home/septian/Eta3PiDalitz/run/cfgBootstrapEnergyDep/ana_2017_reweightedMC_${TAG}_4 $ITER $ITER
# mv -f dalitz_2017_data_${TAG}_${ITER}.fit fitBootstrap/
