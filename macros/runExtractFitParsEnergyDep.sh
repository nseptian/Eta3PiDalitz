#!/bin/sh

RUNFOLDER=/d/home/septian/Eta3PiDalitz/run
FITFOLDER=$RUNFOLDER/fitEnergyDep
base_name=dalitz_2017_data_02202023

echo 'Base name is -> '$base_name
cd $FITFOLDER
for iKFit in {kfit001,kfit002,kfit003,kfit004,kfit005,kfit006,kfit007,kfit008,kfit009,kfit010}
do
    $DALITZ/DalitzExe/extractFitParsEnergyDep.exe ${base_name}_${iKFit} ${base_name}_${iKFit}.txt
done
cd $RUNFOLDER