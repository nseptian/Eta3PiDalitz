#!/bin/bash

# Check if the correct number of arguments is provided
if [ "$#" -ne 7 ]; then
    echo "Usage: $0 <data_set> <outName> <is_mc> <cutTag> <SystematicVariation> <SystematicVariationIdx> <nThreads>"
    exit 1
fi

MACRODIR=/d/grid17/septian/Eta3PiDalitz/macros/

# Assign arguments to variables
DATA_SET=$1
OUT_NAME=$2
IS_MC=$3
CUT_TAG=$4
SYSTEMATIC_VARIATION=$5
SYSTEMATIC_VARIATION_IDX=$6
N_THREADS=$7

# Run the ROOT macro
root -l -b -q ${MACRODIR}"Eta3PiSelectorParallel.C(${DATA_SET}, \"${OUT_NAME}\", ${IS_MC}, \"${CUT_TAG}\", \"${SYSTEMATIC_VARIATION}\", ${SYSTEMATIC_VARIATION_IDX}, ${N_THREADS})"