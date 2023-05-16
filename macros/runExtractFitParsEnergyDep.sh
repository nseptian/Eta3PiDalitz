#!/bin/sh

RUNFOLDER=/d/home/septian/Eta3PiDalitz/run
FITFOLDER=$RUNFOLDER/fitEnergyDep
base_name=dalitz_2017_data_02202023

echo 'Base name is -> '$base_name
cd $FITFOLDER

# kFit variation
# for iKFit in {kfit001,kfit002,kfit003,kfit004,kfit005,kfit006,kfit007,kfit008,kfit009,kfit010}
# do
#     $DALITZ/DalitzExe/extractFitParsEnergyDep.exe ${base_name}_${iKFit} ${base_name}_${iKFit}.txt
# done

for iT_range in {t_0102,t_0203,t_0304,t_0405,t_0506,t_0607,t_0708,t_0809,t_0910}
do
    $DALITZ/DalitzExe/extractFitParsEnergyDep.exe ${base_name}_${iT_range} ${base_name}_${iT_range}.txt
done

cd $RUNFOLDER