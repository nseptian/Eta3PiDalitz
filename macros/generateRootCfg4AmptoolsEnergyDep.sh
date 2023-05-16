#!/bin/bash

# MACROSFOLDER=/d/home/septian/Eta3PiDalitz/macros
RUNFOLDER=/d/home/septian/Eta3PiDalitz/run
CFGFOLDER=$RUNFOLDER/cfg

tag=02202023_t_0506

echo 'Cut tag is -> '$tag

# cd $MACROSFOLDER


cd $RUNFOLDER
for iEnergy in {3}
do
    root -b -x -q ''$DALITZ'/macros/Eta3PiReconstruction.C(0,"root4AmptoolsEnergyDep/eta_2017_data",0,"'$tag'",1,'$iEnergy')'
    root -b -x -q ''$DALITZ'/macros/Eta3PiReconstruction.C(0,"root4AmptoolsEnergyDep/mc_rec_2017_data",1,"'$tag'",1,'$iEnergy')'
    cd $CFGFOLDER
    cp ana_2017_energyDep_template.cfg ana_2017_${tag}_${iEnergy}.cfg
    sed -i 's/dalitz_2017_data/dalitz_2017_data_'$tag'_'$iEnergy'/g' ana_2017_${tag}_${iEnergy}.cfg
    sed -i 's/mc_rec_2017_data/mc_rec_2017_data_'$tag'_'$iEnergy'/g' ana_2017_${tag}_${iEnergy}.cfg
    sed -i 's/eta_2017_data/eta_2017_data_'$tag'_'$iEnergy'/g' ana_2017_${tag}_${iEnergy}.cfg
    cd $RUNFOLDER
done
