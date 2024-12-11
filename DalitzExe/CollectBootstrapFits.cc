#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TDirectory.h>
#include <TSystemDirectory.h>
#include <TSystemFile.h>
#include <TList.h>
#include <TSystem.h>
#include <TVirtualStreamerInfo.h> // Include the TVirtualStreamerInfo header
#include "IUAmpTools/FitResults.h" // Include the AmpTools FitResults header

struct FitResult {
    double likelihood;
    int minuitStatus;
    int ematrixStatus;
    std::vector<double> parameters;
};

void CollectBootstrapFits(const std::string& inputDir, const std::string& inputFileBaseName, const std::string& outputFile, const std::string& centralValueFitFile) {
    std::vector<FitResult> fitResultsVec;
    std::vector<std::string> fitFiles;

    // Collect all fit files from the input directory
    TSystemDirectory dir(inputDir.c_str(), inputDir.c_str());
    TList* files = dir.GetListOfFiles();
    if (files) {
        TSystemFile* file;
        TString fname;
        std::cout << "Reading fit files from " << inputDir << std::endl;
        TIter next(files);
        while ((file = (TSystemFile*)next())) {
            fname = file->GetName();
            if (!file->IsDirectory() && fname.EndsWith(".fit") && fname.Contains(inputFileBaseName.c_str()) && !fname.Contains("nan")) {
                cout << "Found file: " << fname << endl;
                fitFiles.push_back(inputDir + "/" + fname.Data());
            }
        }
    }

    // Read fit files and collect fit results
    for (const auto& fitFile : fitFiles) {
        cout << "Reading fit file: " << fitFile << endl;
        FitResults fitResultsObj(fitFile); // Use AmpTools FitResults to read the file
        FitResult result;
        result.likelihood = fitResultsObj.likelihood();
        result.minuitStatus = fitResultsObj.lastMinuitCommandStatus();
        result.ematrixStatus = fitResultsObj.eMatrixStatus();
        result.parameters = fitResultsObj.parValueList();
        fitResultsVec.push_back(result);
    }

    // Create output ROOT file

    cout << "Creating output file: " << outputFile << endl;
    TFile* outFile = new TFile(outputFile.c_str(), "RECREATE");
    if (!outFile->IsOpen()) {
        std::cerr << "Error creating output file: " << outputFile << std::endl;
        return;
    }

    // Create TTree to store fit results
    cout << "Creating TTree: FitResults" << endl;
    TTree* tree = new TTree("FitResults", "Fit Results from Bootstrap");
    double likelihood;
    int minuitStatus;
    int ematrixStatus;
    std::vector<double> parameters;
    tree->Branch("likelihood", &likelihood);
    tree->Branch("minuitStatus", &minuitStatus);
    tree->Branch("ematrixStatus", &ematrixStatus);
    tree->Branch("parameters", &parameters);

    FitResults centralValueResults(centralValueFitFile);
    std::vector<double> centralValue = centralValueResults.parValueList();
    std::vector<std::vector<double>> centralValueErrorMatrix = centralValueResults.errorMatrix();
    std::vector<double> centralValueError;
    for (size_t i = 0; i < centralValue.size(); ++i) {
        centralValueError.push_back(sqrt(centralValueErrorMatrix[i][i]));
    }

    
    // Create histograms for fit parameters
    std::vector<TH1D*> histograms;
    cout << "Creating histograms for fit parameters" << endl;
    if (!fitResultsVec.empty() && !fitResultsVec[0].parameters.empty()) {
        size_t numParams = fitResultsVec[0].parameters.size();
        for (size_t i = 0; i < numParams; ++i) {
            double min = centralValue[i] - 10 * centralValueError[i];
            double max = centralValue[i] + 10 * centralValueError[i];
            histograms.push_back(new TH1D(Form("param_%zu", i), Form("Parameter %zu", i), 40, min, max));
        }
    }

    // Fill TTree and histograms
    // cout << "Filling TTree and histograms" << endl;
    for (const auto& result : fitResultsVec) {
        likelihood = result.likelihood;
        minuitStatus = result.minuitStatus;
        ematrixStatus = result.ematrixStatus;
        cout << "likelihood: " << likelihood << ", minuitStatus: " << minuitStatus << ", ematrixStatus: " << ematrixStatus << endl;
        parameters = result.parameters;
        tree->Fill();
        if (minuitStatus == 0 && ematrixStatus == 3) {
            for (size_t i = 0; i < result.parameters.size(); ++i) {
            histograms[i]->Fill(result.parameters[i]);
            }
        }
    }

    // Write TTree and histograms to file
    tree->Write();
    // get mean and standard deviation from the histogram
    vector<double> meanValues;
    vector<double> sigmaValues;

    cout << "Writing histograms to file" << endl;
    for (auto hist : histograms) {
        meanValues.push_back(hist->GetMean());
        sigmaValues.push_back(hist->GetStdDev());
        cout << "Writing histogram: " << hist->GetName() << endl;
        hist->Write();
    }

    // Write mean and standard deviation to file
    cout << "Mean values (-a,b,c,d,e,f,g,h,l): \n \t {";
    for (size_t i = 3; i < meanValues.size(); ++i) {
        if (i==3) {
            cout << -1.0*meanValues[i] << ",";
        } else
        if (i != meanValues.size() - 1) {
            cout << meanValues[i] << ",";
        } else {
            cout << meanValues[i] << "};" << endl;
        }
    }

    cout << "Sigma values (-a,b,c,d,e,f,g,h,l): \n \t {";
    for (size_t i = 3; i < sigmaValues.size(); ++i) {
        if (i != sigmaValues.size() - 1) {
            cout << sigmaValues[i] << ",";
        } else {
            cout << sigmaValues[i] << "};" << endl;
        }
    }

    outFile->Close();
    // delete outFile;
    // for (auto hist : histograms) {
    //     delete hist;
    // }
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <inputDir> <inputFileBaseName> <outputFile> <centralValueFitFile>" << std::endl;
        return 1;
    }

    std::string inputDir = argv[1];
    std::string inputFileBaseName = argv[2];
    std::string outputFile = argv[3];
    std::string centralValueFitFile = argv[4];

    CollectBootstrapFits(inputDir, inputFileBaseName, outputFile, centralValueFitFile);
    return 0;
}