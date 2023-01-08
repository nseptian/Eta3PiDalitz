#!/bin/bash

export MACROSFOLDER=/d/home/septian/Eta3PiDalitz/macros

echo "Enter cut tag:"
read tag

# echo "Enter first bootstrap tag (integer):"
# read fbtag

# echo "Enter last bootstrap tag (integer):"
# read lbtag

echo 'Cut tag is -> '$tag
# echo 'Generating bootstrap tag ('$fbtag'..'$lbtag')'

cd $MACROSFOLDER

# root -x -q 'EtaTo3PiReconstructionInd.C(0,"root4Amptools/eta_2017_data_'$tag'",0)'
# root -x -q 'EtaTo3PiReconstructionInd.C(1,"root4Amptools/eta_2018S_data_'$tag'",0)'
# root -x -q 'EtaTo3PiReconstructionInd.C(2,"root4Amptools/eta_2018F_data_'$tag'",0)'
# root -x -q 'EtaTo3PiReconstructionAll.C(3,"root4Amptools/eta_all_data_'$tag'",0)'

# root -x -q 'EtaTo3PiReconstructionInd.C(0,"root4Amptools/mc_rec_2017_data_'$tag'",1)'
# root -x -q 'EtaTo3PiReconstructionInd.C(1,"root4Amptools/mc_rec_2018S_data_'$tag'",1)'
# root -x -q 'EtaTo3PiReconstructionInd.C(2,"root4Amptools/mc_rec_2018F_data_'$tag'",1)'
# root -x -q 'EtaTo3PiReconstructionAll.C(3,"root4Amptools/mc_rec_all_data_'$tag'",1)'

export CFGFOLDER=/d/home/septian/Eta3PiDalitz/macros/cfg
export CFGSYSUNCINITVAL=/d/home/septian/Eta3PiDalitz/macros/cfgSysUncInitVal

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
let iter=0
for iter_a in $(seq 0.9 0.1 1.4)
do
    for iter_b in $(seq 0. 0.1 0.5)
    do
        for iter_d in $(seq 0. 0.1 0.5)
        do
            for iter_f in $(seq 0. 0.1 0.5)
            do
                cp -f $CFGFOLDER/ana_all_$tag.cfg $CFGSYSUNCINITVAL/ana_all_${tag}_${iter}.cfg
                sed -i 's/par_A -1.095 gaussian -1.095 /par_A -'$iter_a' gaussian -'$iter_a' /g' $CFGSYSUNCINITVAL/ana_all_${tag}_${iter}.cfg
                sed -i 's/par_B 0.145  gaussian 0.145 /par_B '$iter_b' gaussian '$iter_b' /g' $CFGSYSUNCINITVAL/ana_all_${tag}_${iter}.cfg
                sed -i 's/par_D 0.081 gaussian 0.081 /par_D '$iter_d' gaussian '$iter_d' /g' $CFGSYSUNCINITVAL/ana_all_${tag}_${iter}.cfg
                sed -i 's/par_F 0.141 gaussian 0.141 /par_F '$iter_f' gaussian '$iter_f' /g' $CFGSYSUNCINITVAL/ana_all_${tag}_${iter}.cfg
                echo $iter 
                let iter++
            done
        done
    done
done
cd $MACROSFOLDER

# 29082022: all gaussian standard deviation limit changed from 0.03 -> 0.5
# 12092022: change back all gaussian standard deviation