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

  if (argc < 4){
    cout << "Usage:" << endl << endl;
    cout << "\tfitAmplitudesSysUncInitVal <config file name> <first variation tag> <last variation tag>" << endl << endl;
    return 0;
  }

  int first_tag = stoi(argv[2]);
  int last_tag = stoi(argv[3]);

  if (first_tag > last_tag) {
    cout << "Error! <first variation tag> has to be less than <last variation tag>!" << endl;
    return 0;
  }

  if (last_tag-first_tag+1 > 20) {
    cout << "Error! Samples cannot more than 20!" << endl;
    return 0;
  }

    // ************************
    // AmpToolsInterface
    // ************************

  AmpToolsInterface::registerAmplitude(DecayAmp());
  AmpToolsInterface::registerDataReader(DalitzDataReader());

  AmpToolsInterface *ATI[20];

  // ATI->registerAmplitude(DecayAmp());
  // ATI->registerDataReader(DalitzDataReaderBootstrap());

  for (int iFit=first_tag;iFit<=last_tag;iFit++){ 
    cout << endl << endl << "======================================SAMPLE " << iFit <<"======================================" << endl;
      // ************************
      // parse the command line parameters
      // ************************
    string cfgname;
    stringstream sst;
    sst << argv[1] << "_" << iFit << ".cfg";
    sst >> cfgname;

    cout << "Config file name = " << cfgname << endl << endl;


      // ************************
      // parse the config file
      // ************************

    ConfigFileParser parser(cfgname);
    ConfigurationInfo* cfgInfo = parser.getConfigurationInfo();
    cfgInfo->display();
    ATI[iFit-first_tag] = new AmpToolsInterface(cfgInfo);
    // AmpToolsInterface ATI(cfgInfo);
    // if (iFit>0) {
      // ATI[iFit]->resetConfigurationInfo(cfgInfo);
    //   cout << "================================ERROR????????????=================================" << endl;
    // }
    cout << "LIKELIHOOD BEFORE MINIMIZATION:  " << ATI[iFit-first_tag]->likelihood() << endl;

    MinuitMinimizationManager* fitManager = ATI[iFit-first_tag]->minuitMinimizationManager();
    fitManager->setPrecision(1E-13);
    fitManager->setStrategy(1);

    fitManager->migradMinimization();

    if( fitManager->status() != 0 && fitManager->eMatrixStatus() != 3 ){
      cout << "ERROR: fit failed..." << endl;
    }

    cout << "LIKELIHOOD AFTER MINIMIZATION:  " << ATI[iFit-first_tag]->likelihood() << endl;
    stringstream ss;
    string fitTag;
    ss << "val_" << iFit;
    ss >> fitTag;
    if (!std::isnan(ATI[iFit-first_tag]->likelihood())) ATI[iFit-first_tag]->finalizeFit(fitTag);
  }
  
  // delete ATI[];

  return 0;

}


