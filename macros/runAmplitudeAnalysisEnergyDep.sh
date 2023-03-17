#!/bin/sh

RUNFOLDER=/d/home/septian/Eta3PiDalitz/run
CFGFOLDER=$RUNFOLDER/cfg
tag=02202023_kfit004

echo 'Cut tag is -> '$tag

for iEnergy in {0..4}
do
    $DALITZ/DalitzExe/fitAmplitudes.exe $CFGFOLDER/ana_2017_${tag}_${iEnergy}.cfg
done