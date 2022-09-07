#!/bin/sh

echo "Enter cut tag (make sure the cut tag is the same with the tag in generated root data):"
read tag

echo 'Cut tag is -> '$tag

# $DALITZ/DalitzExe/fitAmplitudes cfg/ana_2017_$tag.cfg
# $DALITZ/DalitzExe/fitAmplitudes cfg/ana_2018S_$tag.cfg 
# $DALITZ/DalitzExe/fitAmplitudes cfg/ana_2018F_$tag.cfg
$DALITZ/DalitzExe/fitAmplitudes cfg/ana_all_$tag.cfg

mv -f *.fit fit/

# $DALITZ/DalitzExe/plotResults fit/dalitz_2017_data_$tag.fit results/dalitzPlot_2017_$tag.root
# $DALITZ/DalitzExe/plotResults fit/dalitz_2018S_data_$tag.fit results/dalitzPlot_2018S_$tag.root
# $DALITZ/DalitzExe/plotResults fit/dalitz_2018F_data_$tag.fit results/dalitzPlot_2018F_$tag.root
$DALITZ/DalitzExe/plotResults fit/dalitz_all_data_$tag.fit results/dalitzPlot_all_$tag.root

# root -b -x -q 'draw_fit_results.C("results/dalitzPlot_2017_'$tag'")'
# root -b -x -q 'draw_fit_results.C("results/dalitzPlot_2018S_'$tag'")'
# root -b -x -q 'draw_fit_results.C("results/dalitzPlot_2018F_'$tag'")'
root -b -x -q 'draw_fit_results.C("results/dalitzPlot_all_'$tag'")'