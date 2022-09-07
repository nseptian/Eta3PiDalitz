#!/bin/bash

export MACROSFOLDER=/d/home/septian/Eta3PiDalitz/macros

echo "Enter cut tag:"
read tag

echo 'Cut tag is -> '$tag

cd $MACROSFOLDER

# root -x -q 'EtaTo3PiReconstructionInd.C(0,"root4Amptools/eta_2017_data_'$tag'",0)'
# root -x -q 'EtaTo3PiReconstructionInd.C(1,"root4Amptools/eta_2018S_data_'$tag'",0)'
# root -x -q 'EtaTo3PiReconstructionInd.C(2,"root4Amptools/eta_2018F_data_'$tag'",0)'
root -x -q 'EtaTo3PiReconstructionAll.C(3,"root4Amptools/eta_all_data_'$tag'",0)'

# root -x -q 'EtaTo3PiReconstructionInd.C(0,"root4Amptools/mc_rec_2017_data_'$tag'",1)'
# root -x -q 'EtaTo3PiReconstructionInd.C(1,"root4Amptools/mc_rec_2018S_data_'$tag'",1)'
# root -x -q 'EtaTo3PiReconstructionInd.C(2,"root4Amptools/mc_rec_2018F_data_'$tag'",1)'
root -x -q 'EtaTo3PiReconstructionAll.C(3,"root4Amptools/mc_rec_all_data_'$tag'",1)'

export CFGFOLDER=/d/home/septian/Eta3PiDalitz/macros/cfg

cd $CFGFOLDER

# cp ana_2017_template.cfg ana_2017_$tag.cfg
# sed -i 's/dalitz_2017_data/dalitz_2017_data_'$tag'/g' ana_2017_$tag.cfg
# sed -i 's/mc_rec_2017_data/mc_rec_2017_data_'$tag'/g' ana_2017_$tag.cfg
# sed -i 's/eta_2017_data/eta_2017_data_'$tag'/g' ana_2017_$tag.cfg

# cp ana_2018S_template.cfg ana_2018S_$tag.cfg
# sed -i 's/dalitz_2018S_data/dalitz_2018S_data_'$tag'/g' ana_2018S_$tag.cfg
# sed -i 's/mc_rec_2018S_data/mc_rec_2018S_data_'$tag'/g' ana_2018S_$tag.cfg
# sed -i 's/eta_2018S_data/eta_2018S_data_'$tag'/g' ana_2018S_$tag.cfg

# cp ana_2018F_template.cfg ana_2018F_$tag.cfg
# sed -i 's/dalitz_2018F_data/dalitz_2018F_data_'$tag'/g' ana_2018F_$tag.cfg
# sed -i 's/mc_rec_2018F_data/mc_rec_2018F_data_'$tag'/g' ana_2018F_$tag.cfg
# sed -i 's/eta_2018F_data/eta_2018F_data_'$tag'/g' ana_2018F_$tag.cfg

cp ana_all_template.cfg ana_all_$tag.cfg
sed -i 's/dalitz_all_data/dalitz_all_data_'$tag'/g' ana_all_$tag.cfg
sed -i 's/mc_rec_all_data/mc_rec_all_data_'$tag'/g' ana_all_$tag.cfg
sed -i 's/eta_all_data/eta_all_data_'$tag'/g' ana_all_$tag.cfg

cd $MACROSFOLDER