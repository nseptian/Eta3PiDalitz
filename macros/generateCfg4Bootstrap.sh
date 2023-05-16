#!/bin/bash

export RUNFOLDER=/d/home/septian/Eta3PiDalitz/run

tag=10092022

# echo "Enter first bootstrap tag (integer):"
# read fbtag

# echo "Enter last bootstrap tag (integer):"
# read lbtag

echo 'Cut tag is -> '$tag
# echo 'Generating bootstrap tag ('$fbtag'..'$lbtag')'

cd $RUNFOLDER

# root -x -q 'EtaTo3PiReconstructionInd.C(0,"root4Amptools/eta_2017_data_'$tag'",0)'
# root -x -q 'EtaTo3PiReconstructionInd.C(1,"root4Amptools/eta_2018S_data_'$tag'",0)'
# root -x -q 'EtaTo3PiReconstructionInd.C(2,"root4Amptools/eta_2018F_data_'$tag'",0)'
# root -x -q 'EtaTo3PiReconstructionAll.C(3,"root4Amptools/eta_all_data_'$tag'",0)'

# root -x -q 'EtaTo3PiReconstructionInd.C(0,"root4Amptools/mc_rec_2017_data_'$tag'",1)'
# root -x -q 'EtaTo3PiReconstructionInd.C(1,"root4Amptools/mc_rec_2018S_data_'$tag'",1)'
# root -x -q 'EtaTo3PiReconstructionInd.C(2,"root4Amptools/mc_rec_2018F_data_'$tag'",1)'
# root -x -q 'EtaTo3PiReconstructionAll.C(3,"root4Amptools/mc_rec_all_data_'$tag'",1)'

export CFGFOLDER=/d/home/septian/Eta3PiDalitz/run/cfg
export CFGBOOTSTRAPFOLDER=/d/home/septian/Eta3PiDalitz/run/cfgBootstrap
export ROOT4AMPTOOLSFOLDER=/d/home/septian/Eta3PiDalitz/run/root4Amptools/

# cd $CFGFOLDER

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

# for iter in {0..999}
# do
#     echo $iter
#     cp -f $CFGFOLDER/ana_2017_sbs_$tag.cfg $CFGBOOTSTRAPFOLDER/ana_2017_sbs_${tag}_${iter}.cfg
#     sed -i 's/DalitzDataReader/DalitzDataReaderBootstrap/g' $CFGBOOTSTRAPFOLDER/ana_2017_sbs_${tag}_${iter}.cfg
#     sed -i 's/mc_thrown_2017_data.root/mc_thrown_2017_data.root 100'$iter'/g' $CFGBOOTSTRAPFOLDER/ana_2017_sbs_${tag}_${iter}.cfg
#     sed -i 's/mc_rec_2017_data_sbs_'$tag'.root/mc_rec_2017_data_sbs_'$tag'.root 100'$iter'/g' $CFGBOOTSTRAPFOLDER/ana_2017_sbs_${tag}_${iter}.cfg
#     sed -i 's/eta_2017_data_sbs_'$tag'.root/eta_2017_data_sbs_'$tag'.root 100'$iter'/g' $CFGBOOTSTRAPFOLDER/ana_2017_sbs_${tag}_${iter}.cfg
# done

for iter in {0..999}
do
    echo $iter
    cp -f $CFGFOLDER/ana_all_$tag.cfg $CFGBOOTSTRAPFOLDER/ana_all_${tag}_${iter}.cfg
    sed -i 's/DalitzDataReader/DalitzDataReaderBootstrap/g' $CFGBOOTSTRAPFOLDER/ana_all_${tag}_${iter}.cfg
    sed -i 's/mc_thrown_2017_2018S_data.root/mc_thrown_2017_2018S_data.root 100'$iter'/g' $CFGBOOTSTRAPFOLDER/ana_all_${tag}_${iter}.cfg
    sed -i 's/mc_rec_all_data_sbs_'$tag'.root/mc_rec_all_data_sbs_'$tag'.root 100'$iter'/g' $CFGBOOTSTRAPFOLDER/ana_all_${tag}_${iter}.cfg
    sed -i 's/eta_all_data_sbs_'$tag'.root/eta_all_data_sbs_'$tag'.root 100'$iter'/g' $CFGBOOTSTRAPFOLDER/ana_all_${tag}_${iter}.cfg
done
cd $RUNFOLDER
