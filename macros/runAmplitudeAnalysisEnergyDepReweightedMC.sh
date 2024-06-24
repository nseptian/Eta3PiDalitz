#!/bin/sh

RUNFOLDER=/d/home/septian/Eta3PiDalitz/run
CFGFOLDER=$RUNFOLDER/cfg
FITFOLDER=$RUNFOLDER/fitEnergyDep
RESULTSFOLDER=/d/home/septian/Eta3PiDalitzPlots/

tag=t01506_ccdbFlux_EgBin_genEtaRegge

echo 'Cut tag is -> '$tag
cd $RUNFOLDER
for iEnergy in {0..0}
do
    $DALITZ/DalitzExe/fitAmplitudes.exe $CFGFOLDER/ana_2017_${tag}_${iEnergy}.cfg
    mv -f dalitz_2017_data_${tag}_${iEnergy}.fit $FITFOLDER
    $DALITZ/DalitzExe/plotResults.exe $FITFOLDER/dalitz_2017_data_${tag}_${iEnergy}.fit $RESULTSFOLDER/dalitzPlot_2017_${tag}_${iEnergy}.root
    root -b -x -q $RUNFOLDER'/draw_fit_results.C("'$RESULTSFOLDER'/dalitzPlot_2017_'${tag}'_'${iEnergy}'")'

    $DALITZ/DalitzExe/fitAmplitudes.exe $CFGFOLDER/ana_2017_${tag}_${iEnergy}_reweightedMC.cfg
    mv -f dalitz_2017_data_reweightedMC_${tag}_${iEnergy}.fit $FITFOLDER
    $DALITZ/DalitzExe/plotResults.exe $FITFOLDER/dalitz_2017_data_reweightedMC_${tag}_${iEnergy}.fit $RESULTSFOLDER/dalitzPlot_2017_${tag}_${iEnergy}_reweightedMC.root
    root -b -x -q $RUNFOLDER'/draw_fit_results.C("'$RESULTSFOLDER'/dalitzPlot_2017_'${tag}'_'${iEnergy}'_reweightedMC")'
done