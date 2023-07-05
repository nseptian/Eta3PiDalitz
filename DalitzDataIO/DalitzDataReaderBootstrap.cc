
#include <vector>
#include <cassert>
#include <fstream>
#include <iostream>
#include "TH1.h"
#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "IUAmpTools/Kinematics.h"
#include "DalitzDataIO/DalitzDataReaderBootstrap.h"
#include "TSystem.h"

using namespace std;

DalitzDataReaderBootstrap::DalitzDataReaderBootstrap( const vector< string >& args ) :
UserDataReader< DalitzDataReaderBootstrap >(args),
m_eventCounter( 0 ){

  assert(args.size() == 2);
  string inFileName(args[0]);
  string inTreeName("nt");

  TH1::AddDirectory( kFALSE );

  gSystem->Load( "libTree" );

  ifstream fileexists( inFileName.c_str() );
  if (fileexists) m_inFile = TFile::Open(inFileName.c_str() );
  else{
    cout << "DalitzDataReader WARNING:  Cannot find file... " << inFileName << endl;
    m_inFile = NULL;
    m_inTree = NULL;
  }

  int seed = stoi(args[1]);
  m_randGenerator = new TRandom2(seed);
  m_inTree = dynamic_cast<TTree*>( m_inFile->Get( inTreeName.c_str() ) );

  if (m_inTree){

    m_inTree->SetBranchAddress( "EnP1", &m_EnP1 );
    m_inTree->SetBranchAddress( "PxP1", &m_PxP1 );
    m_inTree->SetBranchAddress( "PyP1", &m_PyP1 );
    m_inTree->SetBranchAddress( "PzP1", &m_PzP1 );

    m_inTree->SetBranchAddress( "EnP2", &m_EnP2 );
    m_inTree->SetBranchAddress( "PxP2", &m_PxP2 );
    m_inTree->SetBranchAddress( "PyP2", &m_PyP2 );
    m_inTree->SetBranchAddress( "PzP2", &m_PzP2 );

    m_inTree->SetBranchAddress( "EnP3", &m_EnP3 );
    m_inTree->SetBranchAddress( "PxP3", &m_PxP3 );
    m_inTree->SetBranchAddress( "PyP3", &m_PyP3 );
    m_inTree->SetBranchAddress( "PzP3", &m_PzP3 );

    m_inTree->SetBranchAddress( "weight", &m_weight );
  }

  unsigned int nEvents = numEvents();
  // unsigned int fracNEvents = 0.75*nEvents;

  // cout << "Number of Events in the sample: " << fracNEvents << endl;

  for (unsigned int i=0;i<nEvents;i++) {
    int evtNumber = m_randGenerator->Rndm()*nEvents;
    // cout << evtNumber << endl;
    m_entryOrder.insert((unsigned int)floor(evtNumber));
  }

  m_nextEntry = m_entryOrder.begin();

}

DalitzDataReaderBootstrap::~DalitzDataReaderBootstrap(){
  // cout << "DalitzDataReaderBootstrap destructor begin" << endl;
  if (m_inFile!=NULL) m_inFile->Close();
  if (m_randGenerator) delete m_randGenerator;
  // cout << "DalitzDataReaderBootstrap destructor end" << endl;
}


void
DalitzDataReaderBootstrap::resetSource(){

  m_eventCounter = 0;
  m_nextEntry = m_entryOrder.begin();

}


Kinematics*
DalitzDataReaderBootstrap::getEvent(){

  if( m_eventCounter++ < numEvents() ){

    assert(m_nextEntry != m_entryOrder.end());

    m_inTree->GetEntry(*m_nextEntry++);

    vector<TLorentzVector> particleList;
    particleList.push_back( TLorentzVector( m_PxP1, m_PyP1, m_PzP1, m_EnP1 ) );
    particleList.push_back( TLorentzVector( m_PxP2, m_PyP2, m_PzP2, m_EnP2 ) );
    particleList.push_back( TLorentzVector( m_PxP3, m_PyP3, m_PzP3, m_EnP3 ) );

    return new Kinematics( particleList, m_weight );

  }

  else{
    return NULL;
  }

}


unsigned int
DalitzDataReaderBootstrap::numEvents() const{
  if (!m_inTree) return 0;
  return static_cast< unsigned int >( m_inTree->GetEntries() );
}