#if !defined(DALITZDATAREADERBOOTSTRAP)
#define DALITZDATAREADERBOOTSTRAP

#include <string>
#include <set>

#include "TString.h"
#include "TRandom2.h"
#include "TFile.h"
#include "TTree.h"
#include "IUAmpTools/DataReader.h"
#include "IUAmpTools/UserDataReader.h"


using namespace std;

class DalitzDataReaderBootstrap : public UserDataReader< DalitzDataReaderBootstrap >{

public:

  DalitzDataReaderBootstrap() : UserDataReader< DalitzDataReaderBootstrap >(), m_inFile(NULL), m_randGenerator(NULL) { }

  ~DalitzDataReaderBootstrap();

  DalitzDataReaderBootstrap( const vector< string >& args );

  string name() const { return "DalitzDataReaderBootstrap"; }

  virtual Kinematics* getEvent();

  virtual void resetSource();

  virtual unsigned int numEvents() const;

  int eventCounter() const { return m_eventCounter; }


private:

  TFile* m_inFile;
  TTree* m_inTree;
  int m_eventCounter;

  TRandom2* m_randGenerator;

  double m_EnP1;
  double m_PxP1;
  double m_PyP1;
  double m_PzP1;

  double m_EnP2;
  double m_PxP2;
  double m_PyP2;
  double m_PzP2;

  double m_EnP3;
  double m_PxP3;
  double m_PyP3;
  double m_PzP3;
  
  double m_weight;

  multiset<unsigned int> m_entryOrder;
  mutable multiset<unsigned int>::const_iterator m_nextEntry;

};

#endif
