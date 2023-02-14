#!/bin/sh

echo "Enter cut tag (make sure the cut tag is the same with the tag in generated root data):"
read tag

echo 'Cut tag is -> '$tag

for iter in {0..0}
do
    $DALITZ/DalitzExe/fitAmplitudesBootstrap cfgBootstrap/ana_2017_sbs_$tag $iter $iter
done