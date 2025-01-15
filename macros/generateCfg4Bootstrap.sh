#!/bin/bash

export RUNFOLDER=/d/home/septian/Eta3PiDalitz/run

tag=nominal
TREEDIR=/d/home/septian/Eta3PiDalitz/run/Eta3PiSelectorParallelTree_${tag}/
# echo "Enter first bootstrap tag (integer):"
# read fbtag

# echo "Enter last bootstrap tag (integer):"
# read lbtag

echo 'Cut tag is -> '$tag
# echo 'Generating bootstrap tag ('$fbtag'..'$lbtag')'


mkdir -p $TREEDIR/bootstrap

# echo 'Generating bootstrap cfg files for 2017-01 (cobremsFlux)'

# for iter in {0..999}
# do
#     echo $iter
#     cp -f $TREEDIR/ana_2017_${tag}_cobremsFlux_0.cfg $TREEDIR/bootstrap/ana_2017_${tag}_cobremsFlux_0_${iter}.cfg
#     sed -i 's/DalitzDataReader data/DalitzDataReaderBootstrap data/g' $TREEDIR/bootstrap/ana_2017_${tag}_cobremsFlux_0_${iter}.cfg
#     sed -i 's/data_2017_'$tag'_cobremsFlux_0.root/data_2017_'$tag'_cobremsFlux_0.root 100'$iter'/g' $TREEDIR/bootstrap/ana_2017_${tag}_cobremsFlux_0_${iter}.cfg
# done

# echo 'Generating bootstrap cfg files for 2017-01 (ccdbFlux)'

# for iter in {0..999}
# do
#     echo $iter
#     cp -f $TREEDIR/ana_2017_${tag}_ccdbFlux_0.cfg $TREEDIR/bootstrap/ana_2017_${tag}_ccdbFlux_0_${iter}.cfg
#     sed -i 's/DalitzDataReader data/DalitzDataReaderBootstrap data/g' $TREEDIR/bootstrap/ana_2017_${tag}_ccdbFlux_0_${iter}.cfg
#     sed -i 's/data_2017_'$tag'_ccdbFlux_0.root/data_2017_'$tag'_ccdbFlux_0.root 100'$iter'/g' $TREEDIR/bootstrap/ana_2017_${tag}_ccdbFlux_0_${iter}.cfg
# done

# echo 'Generating bootstrap cfg files for 2018-01'

# for iter in {0..999}
# do
#     echo $iter
#     cp -f $TREEDIR/ana_2018S_${tag}_None_0.cfg $TREEDIR/bootstrap/ana_2018S_${tag}_None_0_${iter}.cfg
#     sed -i 's/DalitzDataReader data/DalitzDataReaderBootstrap data/g' $TREEDIR/bootstrap/ana_2018S_${tag}_None_0_${iter}.cfg
#     sed -i 's/data_2018S_'$tag'_None_0.root/data_2018S_'$tag'_None_0.root 100'$iter'/g' $TREEDIR/bootstrap/ana_2018S_${tag}_None_0_${iter}.cfg
# done

# echo 'Generating bootstrap cfg files for 2018-08'

# for iter in {0..999}
# do
#     echo $iter
#     cp -f $TREEDIR/ana_2018F_${tag}_None_0.cfg $TREEDIR/bootstrap/ana_2018F_${tag}_None_0_${iter}.cfg
#     sed -i 's/DalitzDataReader data/DalitzDataReaderBootstrap data/g' $TREEDIR/bootstrap/ana_2018F_${tag}_None_0_${iter}.cfg
#     sed -i 's/data_2018F_'$tag'_None_0.root/data_2018F_'$tag'_None_0.root 100'$iter'/g' $TREEDIR/bootstrap/ana_2018F_${tag}_None_0_${iter}.cfg
# done

# c,e not fixed to zero

# echo 'Generating bootstrap cfg files for 2017-01 (cobremsFlux)'

# for iter in {0..999}
# do
#     echo $iter
#     cp -f $TREEDIR/ana_2017_${tag}_cobremsFlux_1.cfg $TREEDIR/bootstrap/ana_2017_${tag}_cobremsFlux_1_${iter}.cfg
#     sed -i 's/DalitzDataReader data/DalitzDataReaderBootstrap data/g' $TREEDIR/bootstrap/ana_2017_${tag}_cobremsFlux_1_${iter}.cfg
#     sed -i 's/data_2017_'$tag'_cobremsFlux_0.root/data_2017_'$tag'_cobremsFlux_0.root 100'$iter'/g' $TREEDIR/bootstrap/ana_2017_${tag}_cobremsFlux_1_${iter}.cfg
# done

# echo 'Generating bootstrap cfg files for 2017-01 (ccdbFlux)'

# for iter in {0..999}
# do
#     echo $iter
#     cp -f $TREEDIR/ana_2017_${tag}_ccdbFlux_1.cfg $TREEDIR/bootstrap/ana_2017_${tag}_ccdbFlux_1_${iter}.cfg
#     sed -i 's/DalitzDataReader data/DalitzDataReaderBootstrap data/g' $TREEDIR/bootstrap/ana_2017_${tag}_ccdbFlux_1_${iter}.cfg
#     sed -i 's/data_2017_'$tag'_ccdbFlux_0.root/data_2017_'$tag'_ccdbFlux_0.root 100'$iter'/g' $TREEDIR/bootstrap/ana_2017_${tag}_ccdbFlux_1_${iter}.cfg
# done

# echo 'Generating bootstrap cfg files for 2018-01'

# for iter in {0..999}
# do
#     echo $iter
#     cp -f $TREEDIR/ana_2018S_${tag}_None_1.cfg $TREEDIR/bootstrap/ana_2018S_${tag}_None_1_${iter}.cfg
#     sed -i 's/DalitzDataReader data/DalitzDataReaderBootstrap data/g' $TREEDIR/bootstrap/ana_2018S_${tag}_None_1_${iter}.cfg
#     sed -i 's/data_2018S_'$tag'_None_0.root/data_2018S_'$tag'_None_0.root 100'$iter'/g' $TREEDIR/bootstrap/ana_2018S_${tag}_None_1_${iter}.cfg
# done

# echo 'Generating bootstrap cfg files for 2018-08'

# for iter in {0..999}
# do
#     echo $iter
#     cp -f $TREEDIR/ana_2018F_${tag}_None_1.cfg $TREEDIR/bootstrap/ana_2018F_${tag}_None_1_${iter}.cfg
#     sed -i 's/DalitzDataReader data/DalitzDataReaderBootstrap data/g' $TREEDIR/bootstrap/ana_2018F_${tag}_None_1_${iter}.cfg
#     sed -i 's/data_2018F_'$tag'_None_0.root/data_2018F_'$tag'_None_0.root 100'$iter'/g' $TREEDIR/bootstrap/ana_2018F_${tag}_None_1_${iter}.cfg
# done

# variation 2 c,e,g not fixed to zero
# variation 3 c,e,g,h,l not fixed to zero
# variation 4 c,e,h,l not fixed to zero, but g fixed to zero

# echo 'Generating bootstrap cfg files for 2017-01 (cobremsFlux)'

variation=4

for iter in {0..999}
do
    echo $iter
    cp -f $TREEDIR/ana_2017_${tag}_cobremsFlux_${variation}.cfg $TREEDIR/bootstrap/ana_2017_${tag}_cobremsFlux_${variation}_${iter}.cfg
    sed -i 's/DalitzDataReader data/DalitzDataReaderBootstrap data/g' $TREEDIR/bootstrap/ana_2017_${tag}_cobremsFlux_${variation}_${iter}.cfg
    sed -i 's/data_2017_'$tag'_cobremsFlux_0.root/data_2017_'$tag'_cobremsFlux_0.root 100'$iter'/g' $TREEDIR/bootstrap/ana_2017_${tag}_cobremsFlux_${variation}_${iter}.cfg
done

echo 'Generating bootstrap cfg files for 2017-01 (ccdbFlux)'

for iter in {0..999}
do
    echo $iter
    cp -f $TREEDIR/ana_2017_${tag}_ccdbFlux_${variation}.cfg $TREEDIR/bootstrap/ana_2017_${tag}_ccdbFlux_${variation}_${iter}.cfg
    sed -i 's/DalitzDataReader data/DalitzDataReaderBootstrap data/g' $TREEDIR/bootstrap/ana_2017_${tag}_ccdbFlux_${variation}_${iter}.cfg
    sed -i 's/data_2017_'$tag'_ccdbFlux_0.root/data_2017_'$tag'_ccdbFlux_0.root 100'$iter'/g' $TREEDIR/bootstrap/ana_2017_${tag}_ccdbFlux_${variation}_${iter}.cfg
done

echo 'Generating bootstrap cfg files for 2018-01'

for iter in {0..999}
do
    echo $iter
    cp -f $TREEDIR/ana_2018S_${tag}_None_${variation}.cfg $TREEDIR/bootstrap/ana_2018S_${tag}_None_${variation}_${iter}.cfg
    sed -i 's/DalitzDataReader data/DalitzDataReaderBootstrap data/g' $TREEDIR/bootstrap/ana_2018S_${tag}_None_${variation}_${iter}.cfg
    sed -i 's/data_2018S_'$tag'_None_0.root/data_2018S_'$tag'_None_0.root 100'$iter'/g' $TREEDIR/bootstrap/ana_2018S_${tag}_None_${variation}_${iter}.cfg
done

echo 'Generating bootstrap cfg files for 2018-08'

for iter in {0..999}
do
    echo $iter
    cp -f $TREEDIR/ana_2018F_${tag}_None_${variation}.cfg $TREEDIR/bootstrap/ana_2018F_${tag}_None_${variation}_${iter}.cfg
    sed -i 's/DalitzDataReader data/DalitzDataReaderBootstrap data/g' $TREEDIR/bootstrap/ana_2018F_${tag}_None_${variation}_${iter}.cfg
    sed -i 's/data_2018F_'$tag'_None_0.root/data_2018F_'$tag'_None_0.root 100'$iter'/g' $TREEDIR/bootstrap/ana_2018F_${tag}_None_${variation}_${iter}.cfg
done

# variation=0
# echo 'Generating bootstrap cfg files for GlueX-I variation 0'

# for iter in {0..999}
# do
#     echo $iter
#     cp -f $TREEDIR/ana_GlueX-I_${tag}_${variation}.cfg $TREEDIR/bootstrap/ana_GlueX-I_${tag}_${variation}_${iter}.cfg
#     sed -i 's/DalitzDataReader data/DalitzDataReaderBootstrap data/g' $TREEDIR/bootstrap/ana_GlueX-I_${tag}_${variation}_${iter}.cfg
#     sed -i 's/data_GlueX-I_'$tag'_0.root/data_GlueX-I_'$tag'_0.root 100'$iter'/g' $TREEDIR/bootstrap/ana_GlueX-I_${tag}_${variation}_${iter}.cfg
# done

# variation=1
# echo 'Generating bootstrap cfg files for GlueX-I variation '$variation
# for iter in {0..999}
# do
#     echo $iter
#     cp -f $TREEDIR/ana_GlueX-I_${tag}_${variation}.cfg $TREEDIR/bootstrap/ana_GlueX-I_${tag}_${variation}_${iter}.cfg
#     sed -i 's/DalitzDataReader data/DalitzDataReaderBootstrap data/g' $TREEDIR/bootstrap/ana_GlueX-I_${tag}_${variation}_${iter}.cfg
#     sed -i 's/data_GlueX-I_'$tag'_0.root/data_GlueX-I_'$tag'_0.root 100'$iter'/g' $TREEDIR/bootstrap/ana_GlueX-I_${tag}_${variation}_${iter}.cfg
# done

# variation=2
# echo 'Generating bootstrap cfg files for GlueX-I variation '$variation
# for iter in {0..999}
# do
#     echo $iter
#     cp -f $TREEDIR/ana_GlueX-I_${tag}_${variation}.cfg $TREEDIR/bootstrap/ana_GlueX-I_${tag}_${variation}_${iter}.cfg
#     sed -i 's/DalitzDataReader data/DalitzDataReaderBootstrap data/g' $TREEDIR/bootstrap/ana_GlueX-I_${tag}_${variation}_${iter}.cfg
#     sed -i 's/data_GlueX-I_'$tag'_0.root/data_GlueX-I_'$tag'_0.root 100'$iter'/g' $TREEDIR/bootstrap/ana_GlueX-I_${tag}_${variation}_${iter}.cfg
# done

# variation=3
# echo 'Generating bootstrap cfg files for GlueX-I variation '$variation
# for iter in {0..999}
# do
#     echo $iter
#     cp -f $TREEDIR/ana_GlueX-I_${tag}_${variation}.cfg $TREEDIR/bootstrap/ana_GlueX-I_${tag}_${variation}_${iter}.cfg
#     sed -i 's/DalitzDataReader data/DalitzDataReaderBootstrap data/g' $TREEDIR/bootstrap/ana_GlueX-I_${tag}_${variation}_${iter}.cfg
#     sed -i 's/data_GlueX-I_'$tag'_0.root/data_GlueX-I_'$tag'_0.root 100'$iter'/g' $TREEDIR/bootstrap/ana_GlueX-I_${tag}_${variation}_${iter}.cfg
# done

# for iter in {0..999}
# do
#     echo $iter
#     mkdir -p $TREEDIR/bootstrap
#     cp -f $TREEDIR/ana_2018F_${tag}_None_0.cfg $TREEDIR/bootstrap/ana_2018F_${tag}_None_0_${iter}.cfg
#     sed -i 's/DalitzDataReader data/DalitzDataReaderBootstrap data/g' $TREEDIR/bootstrap/ana_2018F_${tag}_None_0_${iter}.cfg
#     # sed -i 's/0.root/0.root 100'$iter'/g' $TREEDIR/bootstrap/ana_2018F_${tag}_None_0_${iter}.cfg
#     # sed -i 's/mc_thrown_2018F_'$tag'_0.root/mc_thrown_2018F_'$tag'_0.root 100'$iter'/g' $CFGBOOTSTRAPFOLDER/ana_2018F_${tag}_reweightedMC_${iter}.cfg
#     # sed -i 's/mc_rec_2018F_'$tag'_0.root/mc_rec_2018F_'$tag'_0.root 100'$iter'/g' $CFGBOOTSTRAPFOLDER/ana_2018F_${tag}_reweightedMC_${iter}.cfg
#     sed -i 's/data_2018F_'$tag'_None_0.root/data_2018F_'$tag'_None_0.root 100'$iter'/g' $TREEDIR/bootstrap/ana_2018F_${tag}_None_0_${iter}.cfg
# done

# for iter in {0..999}
# do
#     echo $iter
#     mkdir -p $TREEDIR/bootstrap
#     cp -f $TREEDIR/ana_2018F_${tag}_None_0.cfg $TREEDIR/bootstrap/ana_2018F_${tag}_None_0_${iter}.cfg
#     sed -i 's/DalitzDataReader DP/DalitzDataReaderBootstrap DP/g' $TREEDIR/bootstrap/ana_2018F_${tag}_None_0_${iter}.cfg
#     sed -i 's/DP_mc_rec_2018F_'$tag'_None_0.root/DP_mc_rec_2018F_'$tag'_None_0.root 100'$iter'/g' $TREEDIR/bootstrap/ana_2018F_${tag}_None_0_${iter}.cfg
# done

# UNC_DIR=${TREEDIR}uncertainty

# for unc_iter in {0..99}
# do
#     for iter in {0..199}
#     do
#         echo $unc_iter $iter
#         mkdir -p $UNC_DIR/bootstrap
#         cp -f $UNC_DIR/ana_2017_${tag}_None_0_${unc_iter}.cfg $UNC_DIR/bootstrap/ana_2017_${tag}_None_0_${unc_iter}_${iter}.cfg
#         sed -i 's|DalitzDataReader /d/|DalitzDataReaderBootstrap /d/|g' $UNC_DIR/bootstrap/ana_2017_${tag}_None_0_${unc_iter}_${iter}.cfg
#         # sed -i 's/0.root/0.root 100'$iter'/g' $TREEDIR/bootstrap/ana_2017_${tag}_None_0_${iter}.cfg
#         # sed -i 's/mc_thrown_2017_'$tag'_0.root/mc_thrown_2017_'$tag'_0.root 100'$iter'/g' $CFGBOOTSTRAPFOLDER/ana_2017_${tag}_reweightedMC_${iter}.cfg
#         # sed -i 's/mc_rec_2017_'$tag'_0.root/mc_rec_2017_'$tag'_0.root 100'$iter'/g' $CFGBOOTSTRAPFOLDER/ana_2017_${tag}_reweightedMC_${iter}.cfg
#         sed -i 's/output_part_'${unc_iter}'.root/output_part_'${unc_iter}'.root 100'$iter'/g' $UNC_DIR/bootstrap/ana_2017_${tag}_None_0_${unc_iter}_${iter}.cfg
#     done
# done