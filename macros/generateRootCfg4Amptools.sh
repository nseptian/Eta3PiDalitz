#!/bin/bash

RUNFOLDER=/d/home/septian/Eta3PiDalitz/run
MACROSFOLDER=/d/home/septian/Eta3PiDalitz/macros

tag=test_new2017MC

echo 'Cut tag is -> '$tag

cd $RUNFOLDER

root -b -x -q $MACROSFOLDER/'Eta3PiReconstruction.C(0,"root4Amptools/eta_2017_data",0,"'$tag'",0,0)'
# root -b -x -q $MACROSFOLDER/'Eta3PiReconstruction.C(1,"root4Amptools/eta_2018S_data",0,"'$tag'",0,0)'
# root -b -x -q $MACROSFOLDER/'Eta3PiReconstruction.C(2,"root4Amptools/eta_2018F_data",0,"'$tag'",0,0)'
# root -x -q $MACROSFOLDER/'Eta3PiReconstruction.C(3,"root4Amptools/eta_all_data_'$tag'",0)'

root -b -x -q $MACROSFOLDER/'Eta3PiReconstruction.C(0,"/d/grid17/septian/Eta3PiDalitz/run/root4Amptools/mc_rec_2017_data",1,"'$tag'",0,0)'
# root -b -x -q $MACROSFOLDER/'Eta3PiReconstruction.C(1,"root4Amptools/mc_rec_2018S_data",1,"'$tag'",0,0)'
# root -b -x -q $MACROSFOLDER/'Eta3PiReconstruction.C(2,"root4Amptools/mc_rec_2018F_data",1,"'$tag'",0,0)'
# root -x -q 'Eta3PiReconstructionAll.C(3,"root4Amptools/mc_rec_all_data_'$tag'",1)'

# root -x -b -q 'drawHist.C("root4Amptools/mc_rec_2017_data_sbs_'$tag'.root",1,"'$tag'")'

CFGFOLDER=/d/home/septian/Eta3PiDalitz/run/cfg

cd $CFGFOLDER

# 2017
# cp ana_2017_template.cfg ana_2017_$tag.cfg
# sed -i 's/dalitz_2017_data/dalitz_2017_data_'$tag'/g' ana_2017_$tag.cfg
# sed -i 's/mc_rec_2017_data/mc_rec_2017_data_'$tag'/g' ana_2017_$tag.cfg
# sed -i 's/eta_2017_data/eta_2017_data_'$tag'/g' ana_2017_$tag.cfg

# sbs all=2017+2018S
# cp ana_all_template.cfg ana_all_$tag.cfg
# sed -i 's/dalitz_all_data/dalitz_all_data_'$tag'/g' ana_all_$tag.cfg
# sed -i 's/mc_rec_all_data/mc_rec_all_data_'$tag'/g' ana_all_$tag.cfg
# sed -i 's/eta_all_data/eta_all_data_'$tag'/g' ana_all_$tag.cfg

# resolution analysis
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