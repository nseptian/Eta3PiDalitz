#!/bin/bash

# MACROSFOLDER=/d/home/septian/Eta3PiDalitz/macros
RUNFOLDER=/d/home/septian/Eta3PiDalitz/run
CFGFOLDER=$RUNFOLDER/cfg
tMin=0.15
tMax=0.6
tag=t01506_ccdbFlux_EgBin_genEtaRegge

echo 'Cut tag is -> '$tag

# cd $MACROSFOLDER


cd $RUNFOLDER
for iEnergy in {0..0}

do
    # root -b -x -q ''$DALITZ'/macros/selectMCThrown.C("/d/home/septian/Eta3PiDalitz/DSelectors/Eta3Pi_Thrown_2017_genEtaRegge_flat.root","'$tag'",'$iEnergy',0.15,0.6)'
    root -b -x -q ''$DALITZ'/macros/Eta3PiReconstruction.C(0,"root4AmptoolsEnergyDep/eta_2017_data",0,"'$tag'",1,'$iEnergy')'
    root -b -x -q ''$DALITZ'/macros/Eta3PiReconstruction.C(0,"root4AmptoolsEnergyDep/mc_rec_2017_data",1,"'$tag'",1,'$iEnergy')'
    # cd $CFGFOLDER 
    # cp ana_2017_energyDep_template.cfg ana_2017_${tag}_${iEnergy}.cfg
    # sed -i 's/dalitz_2017_data/dalitz_2017_data_'$tag'_'$iEnergy'/g' ana_2017_${tag}_${iEnergy}.cfg
    # sed -i 's/mc_thrown_2017_data/mc_thrown_2017_'$tag'_'$iEnergy'/g' ana_2017_${tag}_${iEnergy}.cfg
    # sed -i 's/mc_rec_2017_data/mc_rec_2017_data_'$tag'_'$iEnergy'/g' ana_2017_${tag}_${iEnergy}.cfg
    # sed -i 's/eta_2017_data/eta_2017_data_'$tag'_'$iEnergy'/g' ana_2017_${tag}_${iEnergy}.cfg
    # cd $RUNFOLDER
    # root -b -x -q ''$DALITZ'/macros/drawHistEnergyDep.C("root4AmptoolsEnergyDep/eta_2017_data_'$tag'_'$iEnergy'.root","root4AmptoolsEnergyDep/mc_rec_2017_data_'$tag'_'$iEnergy'.root","t01506_ccdbFlux_genEtaRegge","t01506_ccdbFlux_genEtaRegge_'$iEnergy'")'
    # root -b -x -q ''$DALITZ'/macros/drawHistEnergyDep.C("root4AmptoolsEnergyDep/mc_rec_2017_data_'$tag'_'$iEnergy'.root","t0106_ccdbFlux")'
done