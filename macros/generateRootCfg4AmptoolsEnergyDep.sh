#!/bin/bash

# MACROSFOLDER=/d/home/septian/Eta3PiDalitz/macros
RUNFOLDER=/d/home/septian/Eta3PiDalitz/run
CFGFOLDER=$RUNFOLDER/cfg

# echo "Enter cut tag:"
# read tag
# tag=12262022a E = {0,7}
# tag=12262022b E = {7,8}
# tag=12262022c E = {8,9}
# tag=12262022d E = {9,10}
# tag=12262022d E = {11,...}
tag=02202023_kfit002

echo 'Cut tag is -> '$tag

# cd $MACROSFOLDER

# let iEnergy=0

for iEnergy in {0..4}
do
    root -b -x -q ''$DALITZ'/macros/Eta3PiReconstruction.C(0,"root4AmptoolsEnergyDep/eta_2017_data",0,"'$tag'",1,'$iEnergy')'
    root -b -x -q ''$DALITZ'/macros/Eta3PiReconstruction.C(0,"root4AmptoolsEnergyDep/mc_rec_2017_data",1,"'$tag'",1,'$iEnergy')'
    cd $CFGFOLDER
    cp ana_2017_template.cfg ana_2017_${tag}_${iEnergy}.cfg
    sed -i 's/dalitz_2017_data/dalitz_2017_data_'$tag'_'$iEnergy'/g' ana_2017_${tag}_${iEnergy}.cfg
    sed -i 's/mc_rec_2017_data/mc_rec_2017_data_'$tag'_'$iEnergy'/g' ana_2017_${tag}_${iEnergy}.cfg
    sed -i 's/eta_2017_data/eta_2017_data_'$tag'_'$iEnergy'/g' ana_2017_${tag}_${iEnergy}.cfg
    cd $RUNFOLDER
done
# root -b -x -q 'EtaTo3PiReconstructionAllBkgsubs.C(1,"root4Amptools/eta_2018S_data",0,"'$tag'")'
# root -x -q 'EtaTo3PiReconstructionInd.C(2,"root4Amptools/eta_2018F_data_'$tag'",0)'
# root -x -q 'EtaTo3PiReconstructionAll.C(3,"root4Amptools/eta_all_data_'$tag'",0)'

# root -b -x -q 'Eta3PiReconstruction.C(0,"root4Amptools/mc_rec_2017_data",1,"'$tag'")'
# root -b -x -q 'EtaTo3PiReconstructionAllBkgsubs.C(1,"root4Amptools/mc_rec_2018S_data",1,"'$tag'")'
# root -x -q 'EtaTo3PiReconstructionInd.C(2,"root4Amptools/mc_rec_2018F_data_'$tag'",1)'
# root -x -q 'EtaTo3PiReconstructionAll.C(3,"root4Amptools/mc_rec_all_data_'$tag'",1)'

# root -x -b -q ''$DALITZ'/macros/drawHist.C("root4Amptools/eta_2017_data_'$tag'.root","'$tag'")'
# root -x -b -q 'drawHist.C("root4Amptools/mc_rec_2017_data_sbs_'$tag'.root",1,"'$tag'")'

# CFGFOLDER=/d/home/septian/Eta3PiDalitz/macros/cfg

# cd $CFGFOLDER

# cp ana_2017_template.cfg ana_2017_sbs_$tag.cfg
# sed -i 's/dalitz_2017_data/dalitz_2017_data_sbs_'$tag'/g' ana_2017_sbs_$tag.cfg
# sed -i 's/mc_rec_2017_data/mc_rec_2017_data_sbs_'$tag'/g' ana_2017_sbs_$tag.cfg
# sed -i 's/eta_2017_data/eta_2017_data_sbs_'$tag'/g' ana_2017_sbs_$tag.cfg

# cp ana_2017_sbs_$tag.cfg ana_2017_sbs_${tag}_res.cfg
# sed -i 's/dalitz_2017_data_sbs_'$tag'/dalitz_2017_data_sbs_'$tag'_res/g' ana_2017_sbs_${tag}_res.cfg
# sed -i 's/DecayAmp/DecayAmpRes/g' ana_2017_sbs_${tag}_res.cfg


# cp ana_2018S_template.cfg ana_2018S_$tag.cfg
# sed -i 's/dalitz_2018S_data/dalitz_2018S_data_'$tag'/g' ana_2018S_$tag.cfg
# sed -i 's/mc_rec_2018S_data/mc_rec_2018S_data_'$tag'/g' ana_2018S_$tag.cfg
# sed -i 's/eta_2018S_data/eta_2018S_data_'$tag'/g' ana_2018S_$tag.cfg

# cp ana_2018F_template.cfg ana_2018F_$tag.cfg
# sed -i 's/dalitz_2018F_data/dalitz_2018F_data_'$tag'/g' ana_2018F_$tag.cfg
# sed -i 's/mc_rec_2018F_data/mc_rec_2018F_data_'$tag'/g' ana_2018F_$tag.cfg
# sed -i 's/eta_2018F_data/eta_2018F_data_'$tag'/g' ana_2018F_$tag.cfg

# cp ana_all_template.cfg ana_all_$tag.cfg
# sed -i 's/dalitz_all_data/dalitz_all_data_'$tag'/g' ana_all_$tag.cfg
# sed -i 's/mc_rec_all_data/mc_rec_all_data_'$tag'/g' ana_all_$tag.cfg
# sed -i 's/eta_all_data/eta_all_data_'$tag'/g' ana_all_$tag.cfg

# cd $MACROSFOLDER