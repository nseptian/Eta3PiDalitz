#!/bin/bash

# Navigate to the DalitzExe directory and run make
cd /d/home/septian/Eta3PiDalitz/DalitzExe || exit
make

# Define the path to the executable
EXECUTABLE="/d/home/septian/Eta3PiDalitz/DalitzExe/CollectBootstrapFits.exe"

# Check if the executable exists
if [ ! -f "$EXECUTABLE" ]; then
    echo "Executable not found: $EXECUTABLE"
    exit 1
fi

# Define input arguments
inputDir=/d/grid17/septian/Eta3PiDalitz/run/Eta3PiSelectorParallelTree_IO_test/bootstrap/
inputFileBaseName=dalitz_2018F_IO_test_None_0_
outputFile=/d/home/septian/Eta3PiDalitz/run/Eta3PiSelectorParallelTree_IO_test/Bootstrap_2018F_IO_test_None_0.root
centralFitFile=/d/home/septian/Eta3PiDalitz/run/Eta3PiSelectorParallelTree_IO_test/dalitz_2018F_IO_test_None_0.fit

# Check if input arguments are provided
if [ -z "$inputDir" ] || [ -z "$inputFileBaseName" ] || [ -z "$outputFile" ]; then
    echo "Usage: $0 <inputDir> <inputFileBaseName> <outputFile> <centralFitFile>"
    exit 1
fi

# Run the executable with input arguments
"$EXECUTABLE" "$inputDir" "$inputFileBaseName" "$outputFile" "$centralFitFile"