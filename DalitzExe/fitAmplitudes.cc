#include <iostream>
#include <fstream>
#include <complex>
#include <string>
#include <vector>
#include <utility>
#include <map>
#include "IUAmpTools/ConfigFileParser.h"
#include "IUAmpTools/ConfigurationInfo.h"
#include "IUAmpTools/AmpToolsInterface.h"
#include "DalitzDataIO/DalitzDataReader.h"
#include "DalitzAmp/DecayAmp.h"


using std::complex;
using namespace std;

int main( int argc, char* argv[] ){


    // ************************
    // usage
    // ************************

  cout << endl << " *** Performing the Fit *** " << endl << endl;

  if (argc <= 1){
    cout << "Usage:" << endl << endl;
    cout << "\tfitAmplitudes <config file name>" << endl << endl;
    return 0;
  }


    // ************************
    // parse the command line parameters
    // ************************

  string cfgname(argv[1]);

  cout << "Config file name = " << cfgname << endl << endl;


    // ************************
    // parse the config file
    // ************************

  ConfigFileParser parser(cfgname);
  ConfigurationInfo* cfgInfo = parser.getConfigurationInfo();
  cfgInfo->display();


    // ************************
    // AmpToolsInterface
    // ************************

  AmpToolsInterface::registerAmplitude(DecayAmp());
  AmpToolsInterface::registerDataReader(DalitzDataReader());

  // Construct AmpToolsInterface dynamically to temporary fix the segfault problem when fitting with gaussian errors, this fix introduces a memory leak problem
  AmpToolsInterface* ATI = new AmpToolsInterface(cfgInfo);

  cout << "LIKELIHOOD BEFORE MINIMIZATION:  " << ATI->likelihood() << endl;

  MinuitMinimizationManager* fitManager = ATI->minuitMinimizationManager();
  fitManager->setPrecision(1E-13);
  fitManager->setStrategy(1);

  fitManager->migradMinimization();
  fitManager->hesseEvaluation();

  if( fitManager->status() != 0 && fitManager->eMatrixStatus() != 3 ){
    cout << "ERROR: fit failed..." << endl;
  }

  cout << "LIKELIHOOD AFTER MINIMIZATION:  " << ATI->likelihood() << endl;

  ATI->finalizeFit();

  return 0;

}


