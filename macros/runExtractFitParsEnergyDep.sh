#!/bin/sh

RUNFOLDER=/d/home/septian/Eta3PiDalitz/run
FITFOLDER=$RUNFOLDER/Eta3PiSelectorParallelTree_t01506_ccdbFlux_genEtaRegge
base_name=dalitz_2018S
tag=ccdbFlux_genEtaRegge

echo 'Base name is -> '$base_name
echo 'Tag is -> '$tag
cd $FITFOLDER

# kFit variation
# for iKFit in {kfit001,kfit002,kfit003,kfit004,kfit005,kfit006,kfit007,kfit008,kfit009,kfit010}
# do
#     $DALITZ/DalitzExe/extractFitParsEnergyDep.exe ${base_name}_${iKFit} ${base_name}_${iKFit}.txt
# done

for iT_range in t01506
do
    $DALITZ/DalitzExe/extractFitParsEnergyDep.exe ${base_name}_${iT_range}_${tag} ${base_name}_${iT_range}_${tag}.txt
done

cd $RUNFOLDER