
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "IUAmpTools/FitResults.h"
#include "TH1F.h"
#include "TFitResult.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLine.h"

double GetStandardDeviation(std::vector<double> values);
void WriteToCSV(std::vector<double> StdDev[9], std::string fitResultsBaseName);

int main(int argc, char* argv[]){
    
    if (argc < 4){
        std::cout << "Usage:" << std::endl << std::endl;
        std::cout << "\tcalcStatErrorToCSV <fit result base file name> <number of samples> <number of bootstrap>" << std::endl << std::endl;
        return 1;
    }

    std::string fitResultsBaseName(argv[1]);
    int NSample = std::stoi(argv[2]);
    int NBootstrap = std::stoi(argv[3]);

    // int NACounter=0;
    int ImportedCounter=0;
    int ImportedCounter_0=0;

    std::vector<double> StdDev[9];

    for (int iSample=0;iSample<NSample;iSample++) {
        cout << "=============== Importing fit results for sample number " << iSample << " ===============" << endl;
        std::vector<double> MeanValues[9];
        for (int iBootstrap=0;iBootstrap<NBootstrap;iBootstrap++) {
            std::stringstream ss;
            std::string fitResultsName;
            ss << fitResultsBaseName << "_" << iSample << "_" << iBootstrap << ".fit";
            ss >> fitResultsName;

            // cout << fitResultsName << endl;
            

            std::ifstream fitFile;
            fitFile.open(fitResultsName);
            if (fitFile){
                FitResults results(fitResultsName);
                std::cout << fitResultsName << " is imported. eMatrixStatus = " << results.eMatrixStatus() << ", lastMinuitCommandStatus = " << results.lastMinuitCommandStatus() << std::endl;
                ImportedCounter++;
                if (results.eMatrixStatus() == 0) return 1;
                std::vector<std::string> ParameterName = results.parNameList();
                std::vector<double> ParameterValues = results.parValueList();

                if (results.lastMinuitCommandStatus()==0) {
                    for (int i=0;i<9;i++){
                        double Value = ParameterValues[i+3];
                        if (i==0) Value *= -1;
                        MeanValues[i].push_back(Value);
                    }
                    ImportedCounter_0++;
                }
            }
            else {
                cout << "File " << fitResultsName << " not found!" << endl;
            }
        }
        double StdDevValues[9];
        for (int i=0;i<9;i++){
            StdDevValues[i] = GetStandardDeviation(MeanValues[i]);
            StdDev[i].push_back(StdDevValues[i]);
        }
    }

    WriteToCSV(StdDev,fitResultsBaseName);

    return 0;
}

double GetStandardDeviation(std::vector<double> values){
    double sum = 0;
    double mean = 0;
    for (int i=0;i<values.size();i++){
        sum += values[i];
    }
    mean = sum/values.size();
    sum = 0;
    for (int i=0;i<values.size();i++){
        sum += (values[i]-mean)*(values[i]-mean);
    }
    return sqrt(sum/values.size());
}

void WriteToCSV(std::vector<double> StdDev[9], std::string fitResultsBaseName){
    std::ofstream csvFile;
    std::string csvFileName = fitResultsBaseName + "_stddev.csv";
    csvFile.open(csvFileName);
    for (int i=0;i<StdDev[0].size();i++){
        for (int j=0;j<9;j++){
            csvFile << StdDev[j][i];
            if (j!=8) csvFile << ",";
            else csvFile << std::endl;
        }
    }
    cout << "Standard deviation values are written to " << csvFileName << endl;
    csvFile.close();
}