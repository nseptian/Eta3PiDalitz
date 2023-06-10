#!/bin/sh

echo "Enter cut tag (make sure the cut tag is the same with the tag in generated root data):"
read tag

echo 'Cut tag is -> '$tag
# {216,222,228,234,240,246,432,433,438,439,444,445,450,451,456,457,462,463,468,474,480,486,492,498,648}
for iter in {0..10}
do
    $DALITZ/DalitzExe/fitAmplitudesSysUncInitVal.exe cfgSysUncInitVal/ana_all_$tag $iter $iter
done