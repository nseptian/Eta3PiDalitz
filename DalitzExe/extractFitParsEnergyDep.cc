#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "IUAmpTools/FitResults.h"

using namespace std;

int main(int argc, char* argv[]){

  if (argc < 3){
    cout << "Usage:" << endl << endl;
    cout << "\textractFitPars <fit results base name> <base output file name>" << endl << endl;
    cout << "Example:" << endl << endl;
    cout << "\textractFitPars dalitz_2017_data_02202023_kfit001 fitPars_2017_data_02202023_kfit001.txt" << endl << endl;
    return 0;
  }

  string resultsname(argv[1]);
  string outtxtname(argv[2]);
  cout << "Output txt file name  = " << outtxtname << endl << endl;

  ofstream ofs(outtxtname, ofstream::out);
  ofs << "// Parameter Values *** " << resultsname << endl;
  for (size_t i = 0; i < 5; i++)
  {
    cout << "Extracting parameter values for energy bin-" << i << "..." << endl;
    string iResultsName = resultsname + "_" + to_string(i) + ".fit";
    cout << "Fit result file name = " << iResultsName << endl << endl;
    FitResults results(iResultsName);
    vector<double> parameterValues = results.parValueList();
    ofs << "{";
    for (int i=3;i<parameterValues.size();i++){
        if (i==3) parameterValues[i]*=-1;
        ofs << parameterValues[i];
        if (i<parameterValues.size()-1) ofs << ",";
    }
    ofs << "};" << endl;
  }

  ofs << "// Error Values *** " << resultsname << endl;
  for (size_t i = 0; i < 4; i++)
  {
    cout << "Extracting error values for energy bin-" << i << endl;
    string iResultsName = resultsname + "_" + to_string(i) + ".fit";
    cout << "\tFit results file name = " << iResultsName << endl << endl;
    FitResults results(iResultsName);
    vector<double> parameterValues = results.parValueList();
    vector<vector<double>> errorMatrix = results.errorMatrix();
    ofs << "{";
    for (int i=3;i<parameterValues.size();i++){
        ofs << errorMatrix[i][i];
        if (i<parameterValues.size()-1) ofs << ",";
    }
    ofs << "}" << endl;
  }

  ofs.close();

  cout << endl << "Parameter values and the errors succesfully exported to -> " << outtxtname << endl << endl;
  return 0;

}