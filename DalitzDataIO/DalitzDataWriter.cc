
#include <vector>
#include <cassert>

#include "DalitzDataIO/DalitzDataWriter.h"

#include "TLorentzVector.h"
#include "TVector3.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TSystem.h"

DalitzDataWriter::DalitzDataWriter( const string& outFile ){

  TH1::AddDirectory( kFALSE );
  gSystem->Load( "libTree" );
  
  m_outFile = new TFile( outFile.c_str(), "recreate" );
  m_outTree = new TTree( "nt", "nt" );

  m_outTree->Branch( "EnP1", &m_EnP1, "EnP1/D" );
  m_outTree->Branch( "PxP1", &m_PxP1, "PxP1/D" );
  m_outTree->Branch( "PyP1", &m_PyP1, "PyP1/D" );
  m_outTree->Branch( "PzP1", &m_PzP1, "PzP1/D" );

  m_outTree->Branch( "EnP2", &m_EnP2, "EnP2/D" );
  m_outTree->Branch( "PxP2", &m_PxP2, "PxP2/D" );
  m_outTree->Branch( "PyP2", &m_PyP2, "PyP2/D" );
  m_outTree->Branch( "PzP2", &m_PzP2, "PzP2/D" );

  m_outTree->Branch( "EnP3", &m_EnP3, "EnP3/D" );
  m_outTree->Branch( "PxP3", &m_PxP3, "PxP3/D" );
  m_outTree->Branch( "PyP3", &m_PyP3, "PyP3/D" );
  m_outTree->Branch( "PzP3", &m_PzP3, "PzP3/D" );
  
  m_outTree->Branch( "s12", &m_s12, "s12/D" );
  m_outTree->Branch( "s23", &m_s23, "s23/D" );
    
  m_outTree->Branch( "X", &X, "X/D" );
  m_outTree->Branch( "Y", &Y, "Y/D" );

  m_outTree->Branch( "weight", &m_weight, "weight/D" );

  m_eventCounter = 0;

}


DalitzDataWriter::~DalitzDataWriter(){

  m_outFile->cd();
  m_outTree->Write();
  m_outFile->Close();

}


void
DalitzDataWriter::writeEvent( const Kinematics& kin ){

  vector< TLorentzVector > particleList = kin.particleList();

  m_EnP1 = particleList[0].E();
  m_PxP1 = particleList[0].Px();
  m_PyP1 = particleList[0].Py();
  m_PzP1 = particleList[0].Pz();

  m_EnP2 = particleList[1].E();
  m_PxP2 = particleList[1].Px();
  m_PyP2 = particleList[1].Py();
  m_PzP2 = particleList[1].Pz();

  m_EnP3 = particleList[2].E();
  m_PxP3 = particleList[2].Px();
  m_PyP3 = particleList[2].Py();
  m_PzP3 = particleList[2].Pz();

  m_s12 = (particleList[0]+particleList[1]).M2();
  m_s23 = (particleList[1]+particleList[2]).M2();
  
  TLorentzVector eta = particleList[0] + particleList[1] + particleList[2];
  TVector3 eta_boost_vec = eta.BoostVector();
  
  TLorentzVector boosted_p1 = particleList[0];
  TLorentzVector boosted_p2 = particleList[1];
  TLorentzVector boosted_p3 = particleList[2];
    
  boosted_p1.Boost(eta_boost_vec*(-1));
  boosted_p2.Boost(eta_boost_vec*(-1));
  boosted_p3.Boost(eta_boost_vec*(-1));
    
  double T_1 = boosted_p1.E() - boosted_p1.M();
  double T_2 = boosted_p2.E() - boosted_p2.M();
  double T_3 = boosted_p3.E() - boosted_p3.M();
  double T_all = T_1 + T_2 + T_3;
  
  X = TMath::Sqrt(3)*(T_1 - T_2)/T_all;
  Y = 3*T_3/T_all - 1.0;
    
  m_weight = kin.weight();

  m_outTree->Fill();
    
  m_eventCounter++;

}
