#!/bin/bash

echo ===files before we start===
ls -lh

# source environment
# source /d/home/septian/.bashrc
# source /d/home/septian/Eta3PiDalitz/set_ana_env.sh
# echo ===environment===
# printenv

echo ===run the fit!===
apptainer exec --bind /d/grid16 --bind /d/grid17 --bind /d/grid13 --bind /cvmfs/oasis.opensciencegrid.org/gluex --bind /d/home/gluex --bind /cvmfs/oasis.opensciencegrid.org/gluex/group:/group /d/home/sdobbs/grid13/container/gluex_centos-7.7.1908_sng3.8_gxi2.20.sif /bin/bash -c 'source /d/home/septian/.bashrc;source /d/grid17/septian/Eta3PiDalitz/set_ana_env.sh;/d/grid17/septian/Eta3PiDalitz/batch_run/bootstrap/run_fit.sh '$1' '$2
# ./run_fit.sh $1 $2

echo ===files when we finish===
ls -lh