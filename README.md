# Eta3PiDalitz

Code for unbinned maximum likelihood fit for Eta->Pi+Pi-Pi0 Dalitz Plot analysis for GlueX Collaboration.
Based on AmpTools: https://github.com/mashephe/AmpTools

## Contents
- `batch_run/` - Scripts for running a batch job using HTCondor
- `DalitzAmp/` - Decay amplitude model for AmpTools
- `DalitzDataIO` - Data handling for AmpTools
- `DalitzExe/` - Programs
- `DalitzPlot/` - Generating Dalitz plots from the Decay amplitude model and data
- `macros` - ROOT and bash macros for event selection, plotting, running the fits, etc.
- `run/` - Run directory.

## Instructions for running the code
1. Clone the repository
2. Compile the code using `make`
3. cd into DalitzExe, then compile the executables using `make`

### Running a single fit
1. cd into macros, edit Eta3PiRecosntruction.C to set up the user configuration.
2. run the macro using `root -l -b -q Eta3PiReconstruction.C(<options>)` to generate flat root tree with the reconstructed events for AmpTools.
3. Write a config file for AmpTools, see the example in `run/cfg/*template.cfg`.
4. Run the fit using `DalitzExe/fitAmplitudes.exe <config file>`, the fit results (a .fit file) will be saved in the working directory.
5. It is encourage for the user to run into AmpTools fit tutorial before doing the fit using this code. See https://github.com/mashephe/AmpTools