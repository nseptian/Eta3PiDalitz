#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include "IUAmpTools/FitResults.h"

using namespace std;

int main(int argc, char* argv[]){

  if (argc < 3){
    cout << "Usage:" << endl << endl;
    cout << "\textractFitPars <fit results name> <output txt file name>" << endl << endl;
    return 0;
  }

  string resultsname(argv[1]);
  // string outpdfname(argv[2]);
  string outtxtname(argv[2]);

  cout << "Fit results file name = " << resultsname << endl;
  cout << "Output txt file name  = " << outtxtname << endl;

  FitResults results(resultsname);
  
  vector<string> parameterName = results.parNameList();
  vector<double> parameterValues = results.parValueList();
  vector<vector<double>> errorMatrix = results.errorMatrix();
  cout << endl << "Size of vector parameters = " << parameterName.size() << endl;

  ofstream ofs(outtxtname, ofstream::out);

  ofs << " *** Parameter Values *** " << endl;
  ofs << "{";
  for (int i=3;i<parameterName.size();i++){
      cout << parameterName[i] << ": " << parameterValues[i] << "+-" << errorMatrix[i][i] << endl;
      ofs << parameterValues[i];
      if (i<parameterName.size()-1) ofs << ",";
  }
  ofs << "};" << endl;

  ofs << " *** Error Values *** " << endl;
  ofs << "{";
  for (int i=3;i<parameterName.size();i++){
      ofs << sqrt(errorMatrix[i][i]);
      if (i<parameterName.size()-1) ofs << ",";
  }
  ofs << "};" << endl;

  ofs.close();

  cout << endl << "Parameter values and the errors succesfully exported to -> " << outtxtname << endl << endl;
  cout << "Formatted as:" << endl << endl;

  cout << " *** Parameter Values *** " << endl;
  cout << "{";
  for (int i=3;i<parameterName.size();i++){
      cout << parameterName[i];
      if (i<parameterName.size()-1) cout << ",";
  }
  cout << "};" << endl;

  cout << " *** Error Values *** " << endl;
  cout << "{";
  for (int i=3;i<parameterName.size();i++){
      cout << parameterName[i] << ".err";
      if (i<parameterName.size()-1) cout << ",";
  }
  cout << "};" << endl << endl;

  // cout << "END OF PROGRAM" << endl;
  return 0;

}