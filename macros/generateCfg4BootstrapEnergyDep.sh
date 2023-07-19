#!/bin/bash

export RUNFOLDER=/d/home/septian/Eta3PiDalitz/run

tag=t0106

echo 'Cut tag is -> '$tag
cd $RUNFOLDER

export CFGFOLDER=/d/home/septian/Eta3PiDalitz/run/cfg
export CFGBOOTSTRAPFOLDER=/d/home/septian/Eta3PiDalitz/run/cfgBootstrapEnergyDep
export ROOT4AMPTOOLSFOLDER=/d/home/septian/Eta3PiDalitz/run/root4AmptoolsEnergyDep/

# GlueX-I (2017)
# for iEnergy in {0..4}
# do
#     for iter in {0..999}
#     do
#         echo $iter
#         cp -f $CFGFOLDER/ana_2017_${tag}_${iEnergy}.cfg $CFGBOOTSTRAPFOLDER/ana_2017_${tag}_${iEnergy}_${iter}.cfg
#         sed -i 's/DalitzDataReader/DalitzDataReaderBootstrap/g' $CFGBOOTSTRAPFOLDER/ana_2017_${tag}_${iEnergy}_${iter}.cfg
#         sed -i 's/mc_thrown_2017_'${tag}'_'${iEnergy}'.root/mc_thrown_2017_'${tag}'_'${iEnergy}'.root 100'$iter'/g' $CFGBOOTSTRAPFOLDER/ana_2017_${tag}_${iEnergy}_${iter}.cfg
#         sed -i 's/mc_rec_2017_data_'${tag}'_'${iEnergy}'.root/mc_rec_2017_data_'${tag}'_'${iEnergy}'.root 100'$iter'/g' $CFGBOOTSTRAPFOLDER/ana_2017_${tag}_${iEnergy}_${iter}.cfg
#         sed -i 's/eta_2017_data_'${tag}'_'${iEnergy}'.root/eta_2017_data_'${tag}'_'${iEnergy}'.root 100'$iter'/g' $CFGBOOTSTRAPFOLDER/ana_2017_${tag}_${iEnergy}_${iter}.cfg
#     done
# done

# GlueX-I (2017) reweighted MC
for iEnergy in {2..4}
do
    for iter in {0..999}
    do
        echo $iter
        cp -f $CFGFOLDER/ana_2017_${tag}_${iEnergy}_reweightedMC.cfg $CFGBOOTSTRAPFOLDER/ana_2017_reweightedMC_${tag}_${iEnergy}_${iter}.cfg
        sed -i 's/DalitzDataReader/DalitzDataReaderBootstrap/g' $CFGBOOTSTRAPFOLDER/ana_2017_reweightedMC_${tag}_${iEnergy}_${iter}.cfg
        sed -i 's/mc_thrown_2017_'${tag}'_'${iEnergy}'.root/mc_thrown_2017_'${tag}'_'${iEnergy}'.root 100'$iter'/g' $CFGBOOTSTRAPFOLDER/ana_2017_reweightedMC_${tag}_${iEnergy}_${iter}.cfg
        sed -i 's/reweighted_mc_rec_2017_data_'${tag}'_'${iEnergy}'.root/reweighted_mc_rec_2017_data_'${tag}'_'${iEnergy}'.root 100'$iter'/g' $CFGBOOTSTRAPFOLDER/ana_2017_reweightedMC_${tag}_${iEnergy}_${iter}.cfg
        sed -i 's/eta_2017_data_'${tag}'_'${iEnergy}'.root/eta_2017_data_'${tag}'_'${iEnergy}'.root 100'$iter'/g' $CFGBOOTSTRAPFOLDER/ana_2017_reweightedMC_${tag}_${iEnergy}_${iter}.cfg
    done
done

# GlueX-I
# for iter in {0..999}
# do
#     echo $iter
#     cp -f $CFGFOLDER/ana_all_$tag.cfg $CFGBOOTSTRAPFOLDER/ana_all_${tag}_${iter}.cfg
#     sed -i 's/DalitzDataReader/DalitzDataReaderBootstrap/g' $CFGBOOTSTRAPFOLDER/ana_all_${tag}_${iter}.cfg
#     sed -i 's/mc_thrown_2017_2018S_data.root/mc_thrown_2017_2018S_data.root 100'$iter'/g' $CFGBOOTSTRAPFOLDER/ana_all_${tag}_${iter}.cfg
#     sed -i 's/mc_rec_all_data_sbs_'$tag'.root/mc_rec_all_data_sbs_'$tag'.root 100'$iter'/g' $CFGBOOTSTRAPFOLDER/ana_all_${tag}_${iter}.cfg
#     sed -i 's/eta_all_data_sbs_'$tag'.root/eta_all_data_sbs_'$tag'.root 100'$iter'/g' $CFGBOOTSTRAPFOLDER/ana_all_${tag}_${iter}.cfg
# done

cd $RUNFOLDER