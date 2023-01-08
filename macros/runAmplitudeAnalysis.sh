#!/bin/sh

# echo "Enter cut tag (make sure the cut tag is the same with the tag in generated root data):"
# read tag

tag=07012023

echo 'Cut tag is -> '$tag

MACROSFOLDER=/d/home/septian/Eta3PiDalitz/macros/
cd $MACROSFOLDER

FITFOLDER=/d/home/septian/Eta3PiDalitz/macros/fit

CFGFOLDER=/d/home/septian/Eta3PiDalitz/macros/cfg
$DALITZ/DalitzExe/fitAmplitudes $CFGFOLDER/ana_2017_sbs_$tag.cfg
$DALITZ/DalitzExe/fitAmplitudesRes $CFGFOLDER/ana_2017_sbs_${tag}_res.cfg

# $DALITZ/DalitzExe/fitAmplitudes cfg/ana_2018S_$tag.cfg 
# $DALITZ/DalitzExe/fitAmplitudes cfg/ana_2018F_$tag.cfg
# $DALITZ/DalitzExe/fitAmplitudes cfg/ana_all_$tag.cfg

mv -f dalitz_2017_data*.fit $FITFOLDER
mv -f dalitz_2018S_data_$tag.fit $FITFOLDER

$DALITZ/DalitzExe/plotResults fit/dalitz_2017_data_sbs_$tag.fit results/dalitzPlot_2017_sbs_$tag.root
$DALITZ/DalitzExe/plotResultsRes fit/dalitz_2017_data_sbs_${tag}_res.fit results/dalitzPlot_2017_sbs_${tag}_res.root

# $DALITZ/DalitzExe/plotResults fit/dalitz_2018S_data_$tag.fit results/dalitzPlot_2018S_$tag.root
# $DALITZ/DalitzExe/plotResults fit/dalitz_2018F_data_$tag.fit results/dalitzPlot_2018F_$tag.root
# $DALITZ/DalitzExe/plotResults fit/dalitz_all_data_$tag.fit results/dalitzPlot_all_$tag.root

root -b -x -q 'draw_fit_results.C("results/dalitzPlot_2017_sbs_'$tag'")'
root -b -x -q 'draw_fit_results.C("results/dalitzPlot_2017_sbs_'${tag}'_res")'
# root -b -x -q 'draw_fit_results.C("results/dalitzPlot_2018S_'$tag'")'
# root -b -x -q 'draw_fit_results.C("results/dalitzPlot_2018F_'$tag'")'
# root -b -x -q 'draw_fit_results.C("results/dalitzPlot_all_'$tag'")'