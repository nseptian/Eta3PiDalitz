// Description: Eta -> pi+ pi- pi0 reconstruction macro
// Author: Nizar Septian, Daniel Lersch, Sean Dobbs

// Standard includes
#include <iostream>
#include <vector>
#include <string>

// ROOT includes
#include "TChain.h"
#include "TFile.h"
#include "RooFit.h"
#include "TLorentzVector.h"
#include "TGraph.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "TDirectory.h"
#include "TString.h"
#include "TCanvas.h"
#include <ROOT/TThreadExecutor.hxx>

using namespace RooFit;
using namespace std;

TString dataDirMC = "/d/grid17/septian/Eta3PiDalitz/DSelectors";
TString outDir = "/d/home/septian/Eta3PiDalitzPlots/";
TString runDir = "/d/grid17/septian/Eta3PiDalitz/run/";
TString treeThreadDirName = Form("%sEta3PiSelectorParallelTree",runDir.Data());
TString treeThreadFileName = "Eta3PiSelectorParallelTree";

// const unsigned nThreads = 64;

//kfit_cut value represents a cut on the kinematic fit probability!
Double_t kfit_cut = 0.001;
Bool_t enableKfitCut = kTRUE;

//user config for pi_0 invariant mass range cut
const Bool_t enablePi0MassCut = kTRUE;
Double_t Pi0MassRange[2] = {0.115,0.155}; //GeV

//user config for theta photons cut
const Bool_t enablePhotonsThetaCut = kTRUE;

// Default photon beam energy range, will be replaced by call function if systmeatic variation is enabled
Double_t PhotonBeamEnergyRange[2] = {6.5,11.6};

//user config for sideband subtraction
const Bool_t enableSidebandSub = kTRUE;
const Bool_t fitOnly = kFALSE;
const Bool_t signalOnlyTree = kFALSE;
const Double_t width = 0.02;
// const Double_t signalRange[2] = {0.53,0.565}; //commented -> determine using sigma value from the fit
Double_t leftSidebandRange[2] = {0.48,0.5};
Double_t rightSidebandRange[2] = {0.60,0.62};

Double_t MissingZVertexRange[2] = {-320,-200};

//user config for MC
//set this two for every MC
// const Double_t signalRangeMC[2] = {0.52585909,0.56975488}; //2017_data_sbs_10092022
// const Double_t weightSidebandMC = -0.510745; //2017_data_sbs_10092022
// now is retrieved directly from the fitInformatin in the tree from data
Double_t signalRangeMC[2] = {0.0,0.0};
Double_t weightSidebandMC = 0.0;

const Bool_t enableResolutionAnalysis = kFALSE;

//Default Mandelstam t cut range, will be replaced by call function if systmeatic variation is enabled
Double_t MandelstamTCutRange[2] = {0.15,0.6};

/*
const Double_t signalRangeMC[2] = {0.547123-(2*0.0105783),0.547123+(2*0.0105783)}; //2018S_data_sbs_10092022
const Double_t weightSidebandMC = -0.452449; //2018S_data_sbs_10092022
*/

const Bool_t enableKinematicCut = kFALSE;
enum BkgModel {
  EXPPOLY=0,
  ARGUS,
  CHEBYCHEV,
};
// Default background model
BkgModel bkgModel = EXPPOLY;

void setHPipPimG1G2Axis(TH1F* h);
void setHG1G2Axis(TH1F* h);
Double_t AcceptanceProb(TGraph *gr, Double_t X, Double_t Y);
void ProgressBar(Long64_t i, Long64_t n);

void Initialize(){
  gROOT->SetBatch(kTRUE);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  gStyle->SetPalette(1);
}
/*
vector<string> getCutConfig(TString cutTag, Bool_t enablePhotonBeamEnergyCut, Int_t PhotonBeamEnergyRangeIdx) {
  vector<string> cutConfigs;

  cout << "Cut tag:" << cutTag << endl;
  cutConfigs.push_back(TString("Cut tag: "+cutTag).Data());

  // Header
  cout << "\n================ Cut Configuration ================\n";
  cout << left << setw(30) << "Cut" << setw(15) << "Status" << setw(20) << "Value/Range" << endl;
  cout << "--------------------------------------------------\n";

  // Kinematic fit probability cut
  string kfitCutStr = "Kinematic fit probability: " + string(enableKfitCut ? "Enabled" : "Disabled") + " " + to_string(kfit_cut);
  cutConfigs.push_back(kfitCutStr);
  cout << left << setw(30) << "Kinematic fit probability:" << setw(15) << (enableKfitCut ? "Enabled" : "Disabled") << setw(20) << kfit_cut << endl;

  // Pi0 mass cut
  if (enablePi0MassCut) {
    string pi0MassCutStr = "Pi0 mass range (GeV): Enabled " + to_string(Pi0MassRange[0]) + " - " + to_string(Pi0MassRange[1]);
    cutConfigs.push_back(pi0MassCutStr);
    cout << left << setw(30) << "Pi0 mass range (GeV):" << setw(15) << "Enabled" << setw(20) << Pi0MassRange[0] << " - " << Pi0MassRange[1] << endl;
  }

  // Photon theta cut
  string photonThetaCutStr = "Photon theta cut: " + string(enablePhotonsThetaCut ? "Enabled" : "Disabled");
  cutConfigs.push_back(photonThetaCutStr);
  cout << left << setw(30) << "Photon theta cut:" << setw(15) << (enablePhotonsThetaCut ? "Enabled" : "Disabled") << endl;

  // Photon beam energy cut
  if (enablePhotonBeamEnergyCut) {
    string photonBeamEnergyCutStr = "Photon beam energy range (GeV): Enabled " + to_string(PhotonBeamEnergyBin[PhotonBeamEnergyRangeIdx]) + " - " + to_string(PhotonBeamEnergyBin[PhotonBeamEnergyRangeIdx+1]);
    cutConfigs.push_back(photonBeamEnergyCutStr);
    cout << left << setw(30) << "Photon beam energy range (GeV):" << setw(15) << "Enabled" << setw(20) << PhotonBeamEnergyBin[PhotonBeamEnergyRangeIdx] << " - " << PhotonBeamEnergyBin[PhotonBeamEnergyRangeIdx+1] << endl;
  }

   // Sideband subtraction
  if (enableSidebandSub) {
    string sidebandSubtractionStr = "Sideband subtraction: Enabled, Left: " + to_string(leftSidebandRange[0]) + " - " + to_string(leftSidebandRange[1]) + ", Right: " + to_string(rightSidebandRange[0]) + " - " + to_string(rightSidebandRange[1]);
    cutConfigs.push_back(sidebandSubtractionStr);
    cout << left << setw(30) << "Sideband subtraction:" << setw(15) << "Enabled" << setw(20) << "See details" << endl;
  }

  // Resolution analysis
  string resolutionAnalysisStr = "Resolution analysis: " + string(enableResolutionAnalysis ? "Enabled" : "Disabled");
  cutConfigs.push_back(resolutionAnalysisStr);
  cout << left << setw(30) << "Resolution analysis:" << setw(15) << (enableResolutionAnalysis ? "Enabled" : "Disabled") << endl;

  // Kinematic cut
  string kinematicCutStr = "Kinematic cut: " + string(enableKinematicCut ? "Enabled" : "Disabled");
  cutConfigs.push_back(kinematicCutStr);
  cout << left << setw(30) << "Kinematic cut:" << setw(15) << (enableKinematicCut ? "Enabled" : "Disabled") << endl;

  // Mandelstam t cut
  if (enableMandelstamTCut) {
    string mandelstamTCutStr = "Mandelstam t cut range (GeV^2): Enabled " + to_string(MandelstamTCutRange[0]) + " - " + to_string(MandelstamTCutRange[1]);
    cutConfigs.push_back(mandelstamTCutStr);
    cout << left << setw(30) << "Mandelstam t cut range (GeV^2):" << setw(15) << "Enabled" << setw(20) << MandelstamTCutRange[0] << " - " << MandelstamTCutRange[1] << endl;
  }

  cout << "==================================================\n";
  return cutConfigs;
}
*/
void addDataToChain(TChain* dataChain, int data_set, bool is_mc) {
  // Define paths for real data and MC data
  const char* realDataPaths[4] = {
    "/d/grid17/dlersch/AmpTool_Data/data2017/PiPiGG_Tree_2017_data_sideband.root",
    "/d/grid17/dlersch/AmpTool_Data/data2018S/PiPiGG_Tree_2018S_data_sideband.root",
    "/d/grid17/dlersch/AmpTool_Data/data2018F/PiPiGG_Tree_05*_amptool_sideband.root",
    "/d/grid17/dlersch/AmpTool_Data/data2017/PiPiGG_Tree_2017_data_sideband.root;/d/grid17/dlersch/AmpTool_Data/data2018S/PiPiGG_Tree_2018S_data_sideband.root"
  };

  // ccdbFlux MC
  // const char* mcDataPaths[4] = {
  //   // Placeholder paths for MC data
  //   "/d/grid17/septian/Eta3PiDalitz/DSelectors/Eta3Pi_Tree_2017_ccdbFlux_genEtaRegge_flat.root",
  //   "/d/grid17/septian/Eta3PiDalitz/DSelectors/Tree_Eta3Pi_Tree_2018S_ccdbFlux_genEtaRegge_flat.root",
  //   "",
  //   "/d/grid17/septian/Eta3PiDalitz/DSelectors/Eta3Pi_Tree_2017_ccdbFlux_genEtaRegge_flat.root;/d/grid17/septian/Eta3PiDalitz/DSelectors/Eta3Pi_Tree_2018S_genEtaRegge_flat.root"
  // };

  // cobremsFlux MC
  // const char* mcDataPaths[4] = {
  //   // Placeholder paths for MC data
  //   "/d/grid17/septian/Eta3PiDalitz/DSelectors/Eta3Pi_Tree_2017_cobremsFlux_genEtaRegge_flat.root",
  //   "",
  //   "",
  //   ""
  // };

  // cobrems + scaled Flux MC
  // const char* mcDataPaths[4] = {
  //   // Placeholder paths for MC data
  //   "/d/home/septian/Eta3PiDalitz/DSelectors/Eta3Pi_Tree_2017_cobrems_ccdbFlux_genEtaRegge_flat.root",
  //   "",
  //   "",
  //   ""
  // };

  // Combined based on datasets
  const char* mcDataPaths[4] = {
    // Placeholder paths for MC data
    "/d/grid17/septian/Eta3PiDalitz/DSelectors/Eta3Pi_Tree_2017_cobrems_ccdbFlux_genEtaRegge_flat.root",
    "/d/grid17/septian/Eta3PiDalitz/DSelectors/Tree_Eta3Pi_Tree_2018S_ccdbFlux_genEtaRegge_flat.root",
    "/d/grid17/septian/Eta3PiDalitz/DSelectors/Tree_Eta3Pi_Tree_2018F_ccdbFlux_genEtaRegge_flat.root",
    "/d/grid17/septian/Eta3PiDalitz/DSelectors/Eta3Pi_Tree_2017_cobrems_ccdbFlux_genEtaRegge_flat.root;/d/grid17/septian/Eta3PiDalitz/DSelectors/Tree_Eta3Pi_Tree_2018S_ccdbFlux_genEtaRegge_flat.root;/d/grid17/septian/Eta3PiDalitz/DSelectors/Tree_Eta3Pi_Tree_2018F_ccdbFlux_genEtaRegge_flat.root"
  };

  // Select the appropriate path array based on is_mc
  const char* selectedPaths[4];
  if (is_mc) {
    for (int i = 0; i < 4; ++i) {
      selectedPaths[i] = mcDataPaths[i];
    }
  } else {
    for (int i = 0; i < 4; ++i) {
      selectedPaths[i] = realDataPaths[i];
    }
  }

  // Add data to the chain
  TString path(selectedPaths[data_set]);
  if (data_set == 3) { 
    TObjArray* tokens = path.Tokenize(";");
    for (Int_t i = 0; i < tokens->GetEntries(); i++) {
      TObjString* token = (TObjString*)tokens->At(i);
      if (token != nullptr) {
        dataChain->Add(token->GetString());
      }
    }
    delete tokens; // Clean up to avoid memory leak
  } else {
    if (path.Length() > 0) { // Check if the path is not empty
      dataChain->Add(path);
    }
  }
}

// Function to set mcThrown and outNameDataForMC
void setMcThrownAndOutName(TFile*& mcThrown, TString& outNameDataForMC, int data_set, bool enablePhotonBeamEnergyCut, const TString& cutTag, int PhotonBeamEnergyRangeIdx) {
    TString runDirectory = "/d/grid17/septian/Eta3PiDalitz/run/";
    const char* mcThrownBasePaths[4] = {
        "mc_thrown_2017","mc_thrown_2018S","mc_thrown_2018F","mc_thrown_phaseI"
    };

    outNameDataForMC = runDirectory;
    outNameDataForMC += "root4Amptools";
    TString mcThrownFileName(runDirectory);
    mcThrownFileName += "root4Amptools";
    if (enablePhotonBeamEnergyCut) {
        outNameDataForMC += "EnergyDep";
        mcThrownFileName += Form("EnergyDep/%s_%s_%d.root",mcThrownBasePaths[data_set],cutTag.Data(),PhotonBeamEnergyRangeIdx);
    }
    else {
        mcThrownFileName += Form("/%s_%s.root",mcThrownBasePaths[data_set],cutTag.Data());
    }
    mcThrown = TFile::Open(mcThrownFileName,"READ");
    outNameDataForMC += "/";

    const char* MCFileName[4] = {
        "eta_2017_data","eta_2018S_data","eta_2018F_data","eta_phaseI_data"
    };

    outNameDataForMC += MCFileName[data_set];
    outNameDataForMC += "_";
    outNameDataForMC += cutTag;

    if (enablePhotonBeamEnergyCut) {
        outNameDataForMC += Form("_%d",PhotonBeamEnergyRangeIdx);
    }
}

struct ThreadHistograms {
  
  TH1F* h1_EnergyBeam;
  TH1F* h1_MandelstamT;
  TH1F* h1_PiPlusPiMinusGamma1Gamma2InvMass;
  TH1F* h1_Gamma1Gamma2InvMass;
  TH1F* h1_Dalitz_X;
  TH1F* h1_Dalitz_Y;
  TH2F* h2_DalitzPlot;
  TH1F* h1_PhotonTheta1;
  TH1F* h1_PhotonTheta2;
  TH1F* h1_zVertex;
  TH1F* h1_KinFitProb;

  ThreadHistograms(){

    TString name1 = "h1_PiPlusPiMinusGamma1Gamma2InvMass";
    TString title1 = "#pi^{+}#pi^{-}#gamma_{1}#gamma_{2} Invariant";
    h1_PiPlusPiMinusGamma1Gamma2InvMass = new TH1F(name1, title1, 100, 0.45, 0.65);

    TString name2 = "h1_Gamma1Gamma2InvMass";
    TString title2 = "#gamma_{1}#gamma_{2} Invariant Mass";
    h1_Gamma1Gamma2InvMass = new TH1F(name2, title2, 100, 0.0, 0.2);

    TString name3 = "h1_EnergyBeam";
    TString title3 = "Beam Energy";
    h1_EnergyBeam = new TH1F(name3, title3, 121, -0.05, 12.05);

    TString name4 = "h1_MandelstamT";
    TString title4 = "Mandelstam t";
    h1_MandelstamT = new TH1F(name4, title4, 101, MandelstamTCutRange[0], MandelstamTCutRange[1]);

    TString name5 = "h1_Dalitz_X";
    TString title5 = "Dalitz X";
    h1_Dalitz_X = new TH1F(name5, title5, 101, -1.0, 1.0);
    
    TString name6 = "h1_Dalitz_Y";
    TString title6 = "Dalitz Y";
    h1_Dalitz_Y = new TH1F(name6, title6, 101, -1.0, 1.0);

    TString name7 = "h2_DalitzPlot";
    TString title7 = "Dalitz Plot";
    h2_DalitzPlot = new TH2F(name7, title7, 101, -1.0, 1.0, 101, -1.0, 1.0);

    TString name8 = "h1_PhotonTheta1";
    TString title8 = "#theta_{#gamma_{1}}";
    h1_PhotonTheta1 = new TH1F(name8, title8, 100, 0.0, 180.0);

    TString name9 = "h1_PhotonTheta2";
    TString title9 = "#theta_{#gamma_{2}}";
    h1_PhotonTheta2 = new TH1F(name9, title9, 100, 0.0, 180.0);

    TString name10 = "h1_zVertex";
    TString title10 = "z Vertex";
    h1_zVertex = new TH1F(name10, title10, 100, -500.0, -100.0);

    TString name11 = "h1_KinFit_Prob";
    TString title11 = "Kinematic Fit Probability";
    h1_KinFitProb = new TH1F(name11, title11, 1000, 0.0, 1.0);
  }

  ~ThreadHistograms() {
    delete h1_PiPlusPiMinusGamma1Gamma2InvMass;
    delete h1_Gamma1Gamma2InvMass;
    delete h1_EnergyBeam;
    delete h1_MandelstamT;
    delete h1_Dalitz_X;
    delete h1_Dalitz_Y;
    delete h2_DalitzPlot;
  }
};

struct ThreadTree {
  TFile *outputFile;
  TTree* outputTree;
  int threadNum;
  TString fileName;
  TString treeName;
  TString outputDirectory;

  ThreadTree(TString fname, TString tname, TString oDir, int num) : fileName(fname), treeName(tname), outputDirectory(oDir), threadNum(num) {
    // Use the thread number to create unique tree names, set negative thread number for merged trees
    if (threadNum >= 0) {
      fileName += Form("_%d", threadNum);
      outputFile = new TFile(Form("%s%s.root", outputDirectory.Data(), fileName.Data()), "RECREATE"); 
    }
    else {
      outputFile = new TFile(Form("%smerged_%s.root", outputDirectory.Data(), fileName.Data()), "RECREATE");
    }
    outputTree = new TTree(treeName, treeName);
  }

  void WriteTree() {
    outputFile->cd();
    outputTree->Write();
  }

  // Merge TFile from another ThreadTree object using hadd
  void merge(Bool_t isDeleteIndividualFiles = kTRUE) {
    // Use hadd to merge the trees
    TString haddTarget = Form("%smerged_%s.root", outputDirectory.Data(), fileName.Data());
    TString haddSources = Form("%s%s*.root", outputDirectory.Data(), fileName.Data());
    TString haddCommand = Form("hadd -f %s %s", haddTarget.Data(), haddSources.Data());
    cout << "Merging trees using hadd: " << haddCommand << endl;
    gSystem->Exec(haddCommand);

    if (isDeleteIndividualFiles) {
      // Delete individual files
      TString deleteCommand = Form("rm %s%s*.root", outputDirectory.Data(), fileName.Data());
      cout << "Deleting individual files: " << deleteCommand << endl;
      gSystem->Exec(deleteCommand);
    }
  }

  ~ThreadTree() {
    delete outputTree;
    outputFile->Close();
    delete outputFile;
  }

};

struct CutConfig {
  Bool_t enablePhotonBeamEnergyCut;
  Double_t PhotonBeamEnergyRange[2];
  Bool_t enableKfitCut;
  Double_t kfit_cut;
  Bool_t enablePi0MassCut;
  Double_t Pi0MassRange[2];
  Bool_t enablePhotonsThetaCut;
  Bool_t enableMandelstamTCut;
  Double_t MandelstamTCutRange[2];
  Double_t MissingZVertexRange[2];

  CutConfig() {
    enablePhotonBeamEnergyCut = kFALSE;
    PhotonBeamEnergyRange[0] = 0;
    PhotonBeamEnergyRange[1] = 12.0;
    enableKfitCut = kFALSE;
    kfit_cut = 0.001;
    enablePi0MassCut = kFALSE;
    Pi0MassRange[0] = 0.0;
    Pi0MassRange[1] = 0.2;
    enablePhotonsThetaCut = kFALSE;
    enableMandelstamTCut = kFALSE;
    MandelstamTCutRange[0] = 0.0;
    MandelstamTCutRange[1] = 1.0;
    MissingZVertexRange[0] = -320;
    MissingZVertexRange[1] = -200;
  }

  void Print() {
    cout << "Cut configuration:\n";
    cout << "Photon beam energy cut: " << (enablePhotonBeamEnergyCut ? "Enabled" : "Disabled") << " " << PhotonBeamEnergyRange[0] << " - " << PhotonBeamEnergyRange[1] << endl;
    cout << "Kinematic fit cut: " << (enableKfitCut ? "Enabled" : "Disabled") << " " << kfit_cut << endl;
    cout << "Pi0 mass cut: " << (enablePi0MassCut ? "Enabled" : "Disabled") << " " << Pi0MassRange[0] << " - " << Pi0MassRange[1] << endl;
    cout << "Photons theta cut: " << (enablePhotonsThetaCut ? "Enabled" : "Disabled") << endl;
  }
};

struct SidebandSubtractionParameters {
  Double_t GaussianMean;
  Double_t GaussianSigma;
  Double_t LeftSidebandRange[2];
  Double_t RightSidebandRange[2];
  Double_t SignalRange[2];
  Double_t WeightSidebands=0.0;

  void Print() {
    cout << "Sideband subtraction parameters:\n";
    cout << "Gaussian mean: " << GaussianMean << endl;
    cout << "Gaussian sigma: " << GaussianSigma << endl;
    cout << "Left sideband range: " << LeftSidebandRange[0] << " - " << LeftSidebandRange[1] << endl;
    cout << "Right sideband range: " << RightSidebandRange[0] << " - " << RightSidebandRange[1] << endl;
    cout << "Signal range: " << SignalRange[0] << " - " << SignalRange[1] << endl;
    cout << "Weight sidebands: " << WeightSidebands << endl;
  }
};

void processEntryRange(TChain* dataChain, Bool_t is_mc, CutConfig cutConfig, SidebandSubtractionParameters sidebandParameters, Long64_t startEntry, Long64_t endEntry, ThreadHistograms*& histograms, ThreadTree*& outputThreadTree) {
  
  // cout << "Processing entries from " << startEntry << " to " << endEntry << endl;
  TChain* dataChainProcess = new TChain("myTree");
  dataChainProcess->Add(dataChain);
  //Define output tree
  gSystem->mkdir(treeThreadDirName,kTRUE);
  Double_t EnBeam,PxBeam,PyBeam,PzBeam;
  Double_t EnProton,PxProton,PyProton,PzProton;
  Double_t EnP1,PxP1,PyP1,PzP1;
  Double_t EnP2,PxP2,PyP2,PzP2;
  Double_t EnP3,PxP3,PyP3,PzP3;
  Double_t X,Y,weight=1.0;

  // sidebandParameters.Print();

  if (sidebandParameters.WeightSidebands == 0.0) outputThreadTree = new ThreadTree(treeThreadFileName, "nt", treeThreadDirName, startEntry);
  else {
    outputThreadTree = new ThreadTree(Form("%s_SidebandSubtracted",treeThreadFileName.Data()), "nt", treeThreadDirName, startEntry);
  }
  
  //photon beam
  outputThreadTree->outputTree->Branch("EnBeam",&EnBeam,"EnBeam/D");
  outputThreadTree->outputTree->Branch("PxBeam",&PxBeam,"PxBeam/D");
  outputThreadTree->outputTree->Branch("PyBeam",&PyBeam,"PyBeam/D");
  outputThreadTree->outputTree->Branch("PzBeam",&PzBeam,"PzBeam/D");

  // Recoil proton
  outputThreadTree->outputTree->Branch("EnProton",&EnProton,"EnProton/D");
  outputThreadTree->outputTree->Branch("PxProton",&PxProton,"PxProton/D");
  outputThreadTree->outputTree->Branch("PyProton",&PyProton,"PyProton/D");
  outputThreadTree->outputTree->Branch("PzProton",&PzProton,"PzProton/D");

  //Pi+
  outputThreadTree->outputTree->Branch("EnP1",&EnP1,"EnP1/D");
  outputThreadTree->outputTree->Branch("PxP1",&PxP1,"PxP1/D");
  outputThreadTree->outputTree->Branch("PyP1",&PyP1,"PyP1/D");
  outputThreadTree->outputTree->Branch("PzP1",&PzP1,"PzP1/D");
  
  //Pi-
  outputThreadTree->outputTree->Branch("EnP2",&EnP2,"EnP2/D");
  outputThreadTree->outputTree->Branch("PxP2",&PxP2,"PxP2/D");
  outputThreadTree->outputTree->Branch("PyP2",&PyP2,"PyP2/D");
  outputThreadTree->outputTree->Branch("PzP2",&PzP2,"PzP2/D");
  
  //Pi0
  outputThreadTree->outputTree->Branch("EnP3",&EnP3,"EnP3/D");
  outputThreadTree->outputTree->Branch("PxP3",&PxP3,"PxP3/D");
  outputThreadTree->outputTree->Branch("PyP3",&PyP3,"PyP3/D");
  outputThreadTree->outputTree->Branch("PzP3",&PzP3,"PzP3/D");

  //weight
  outputThreadTree->outputTree->Branch("weight",&weight,"weight/D");

  // // Initialize variable on the tree
  Double_t kfit_prob, dBRT, zVertex;
  TLorentzVector *pip_p4_kin = nullptr;
  TLorentzVector *pim_p4_kin = nullptr;
  TLorentzVector *g1_p4_kin = nullptr;
  TLorentzVector *g2_p4_kin = nullptr;
  TLorentzVector *beam_p4_kin = nullptr;
  TLorentzVector *p_p4_kin = nullptr;

  // True values for Monte Carlo data
  TLorentzVector *pip_p4_true = nullptr;
  TLorentzVector *pim_p4_true = nullptr;
  TLorentzVector *g1_p4_true = nullptr;
  TLorentzVector *g2_p4_true = nullptr;
  TLorentzVector *beam_p4_true = nullptr;

  dataChainProcess->SetBranchAddress("kfit_prob", &kfit_prob);
  dataChainProcess->SetBranchAddress("dBRT", &dBRT);
  dataChainProcess->SetBranchAddress("zVertex", &zVertex);
  dataChainProcess->SetBranchAddress("pip_p4_kin", &pip_p4_kin);
  dataChainProcess->SetBranchAddress("pim_p4_kin", &pim_p4_kin);
  dataChainProcess->SetBranchAddress("g1_p4_kin", &g1_p4_kin);
  dataChainProcess->SetBranchAddress("g2_p4_kin", &g2_p4_kin);
  dataChainProcess->SetBranchAddress("beam_p4_kin", &beam_p4_kin);
  dataChainProcess->SetBranchAddress("p_p4_kin", &p_p4_kin);

  TLorentzVector p4_TargetProton(0.0,0.0,0.0,0.938272);

  if (is_mc) {
    dataChainProcess->SetBranchAddress("pip_p4_true", &pip_p4_true);
    dataChainProcess->SetBranchAddress("pim_p4_true", &pim_p4_true);
    dataChainProcess->SetBranchAddress("g1_p4_true", &g1_p4_true);
    dataChainProcess->SetBranchAddress("g2_p4_true", &g2_p4_true);
    dataChainProcess->SetBranchAddress("beam_p4_true", &beam_p4_true);
  }

  // Initialize histograms
  TDirectory *dir = outputThreadTree->outputFile->mkdir("Histograms");
  dir->cd();
  histograms = new ThreadHistograms();

  for (Long64_t entry = startEntry; entry < endEntry; ++entry) {
    dataChainProcess->GetEntry(entry);

    if (cutConfig.enableKfitCut && kfit_prob < cutConfig.kfit_cut) continue;
    if (zVertex < cutConfig.MissingZVertexRange[0] || zVertex > cutConfig.MissingZVertexRange[1]) continue;

    if (cutConfig.enablePhotonsThetaCut) {
      Bool_t isPhoton1InTransitionRegion = g1_p4_kin->Theta()*TMath::RadToDeg() > 10.3 && g1_p4_kin->Theta()*TMath::RadToDeg() < 11.9;
      Bool_t isPhoton1InLowThetaAngle = g1_p4_kin->Theta()*TMath::RadToDeg() < 2.0;
      Bool_t isPhoton2InTransitionRegion = g2_p4_kin->Theta()*TMath::RadToDeg() > 10.3 && g2_p4_kin->Theta()*TMath::RadToDeg() < 11.9;
      Bool_t isPhoton2InLowThetaAngle = g2_p4_kin->Theta()*TMath::RadToDeg() < 2.0;
      Bool_t isRejectPhotons = isPhoton1InTransitionRegion || isPhoton1InLowThetaAngle || isPhoton2InTransitionRegion || isPhoton2InLowThetaAngle;
      if (isRejectPhotons) continue;
    }
  
    Double_t MandelstamT = -1.0*(p4_TargetProton - *p_p4_kin).M2();
    if (cutConfig.enableMandelstamTCut) {
      Bool_t isMandelstamTInRange = MandelstamT > cutConfig.MandelstamTCutRange[0] && MandelstamT < cutConfig.MandelstamTCutRange[1];
      if (!isMandelstamTInRange) continue;
    }

    TLorentzVector p4_Gamma1Gamma2 = (*g1_p4_kin + *g2_p4_kin);
    Double_t m_Gamma1Gamma2 = p4_Gamma1Gamma2.M();
    if (cutConfig.enablePi0MassCut) {
      Bool_t isPi0MassInRange = m_Gamma1Gamma2 > cutConfig.Pi0MassRange[0] && m_Gamma1Gamma2 < cutConfig.Pi0MassRange[1];
      if (!isPi0MassInRange) continue;
    }
    TLorentzVector p4_PiPlusPiMinusGamma1Gamma2 = (*pip_p4_kin + *pim_p4_kin + *g1_p4_kin + *g2_p4_kin);
    Double_t m_PiPlusPiMinusGamma1Gamma2 = p4_PiPlusPiMinusGamma1Gamma2.M();

    EnBeam = beam_p4_kin->E();    
    if (cutConfig.enablePhotonBeamEnergyCut) {
      Bool_t isPhotonBeamEnergyInRange = EnBeam > cutConfig.PhotonBeamEnergyRange[0] && EnBeam < cutConfig.PhotonBeamEnergyRange[1];
      if (!isPhotonBeamEnergyInRange) continue;
    }

    if(dBRT >= -2.0 && dBRT <= 2.0){
      weight = 1.0;
    }
    else weight = -1.0/8.0;

    PxBeam = beam_p4_kin->Px();
    PyBeam = beam_p4_kin->Py();
    PzBeam = beam_p4_kin->Pz();

    EnProton = p_p4_kin->E();
    PxProton = p_p4_kin->Px();
    PyProton = p_p4_kin->Py();
    PzProton = p_p4_kin->Pz();

    EnP1 = pip_p4_kin->E();
    PxP1 = pip_p4_kin->Px();
    PyP1 = pip_p4_kin->Py();
    PzP1 = pip_p4_kin->Pz();

    EnP2 = pim_p4_kin->E();
    PxP2 = pim_p4_kin->Px();
    PyP2 = pim_p4_kin->Py();
    PzP2 = pim_p4_kin->Pz();

    TLorentzVector pi0_p4_kin = *g1_p4_kin + *g2_p4_kin;
    EnP3 = pi0_p4_kin.E();
    PxP3 = pi0_p4_kin.Px();
    PyP3 = pi0_p4_kin.Py();
    PzP3 = pi0_p4_kin.Pz();

    if (sidebandParameters.WeightSidebands < 0.0) {
      Bool_t isInSidebandRange = (m_PiPlusPiMinusGamma1Gamma2 >= sidebandParameters.LeftSidebandRange[0] && m_PiPlusPiMinusGamma1Gamma2 <= sidebandParameters.LeftSidebandRange[1]) || (m_PiPlusPiMinusGamma1Gamma2 >= sidebandParameters.RightSidebandRange[0] && m_PiPlusPiMinusGamma1Gamma2 <= sidebandParameters.RightSidebandRange[1]);
      Bool_t isInSignalRange = (m_PiPlusPiMinusGamma1Gamma2 >= sidebandParameters.SignalRange[0] && m_PiPlusPiMinusGamma1Gamma2 <= sidebandParameters.SignalRange[1]);
      if (isInSidebandRange) {
        weight *= sidebandParameters.WeightSidebands;
      }
      else if (!isInSignalRange) {
        continue;
      }
    }

    // Calculate X and Y
    TLorentzVector eta = (*pip_p4_kin + *pim_p4_kin + *g1_p4_kin + *g2_p4_kin);
    TVector3 eta_boost = eta.BoostVector();
    TLorentzVector boosted_p1 = *pip_p4_kin;
    TLorentzVector boosted_p2 = *pim_p4_kin;
    TLorentzVector boosted_p3 = *g1_p4_kin + *g2_p4_kin;
    boosted_p1.Boost(eta_boost*(-1));
    boosted_p2.Boost(eta_boost*(-1));
    boosted_p3.Boost(eta_boost*(-1));
    Double_t T_plus = boosted_p1.E() - boosted_p1.M();
    Double_t T_minus = boosted_p2.E() - boosted_p2.M();
    Double_t T_zero = boosted_p3.E() - boosted_p3.M();
    Double_t T_all = T_plus + T_minus + T_zero;
    Double_t X_c = TMath::Sqrt(3.0)*(T_plus - T_minus)/T_all;
    Double_t Y_c = 3.0*T_zero/T_all - 1.0;

    outputThreadTree->outputTree->Fill();
    histograms->h1_EnergyBeam->Fill(EnBeam, weight);
    histograms->h1_MandelstamT->Fill(MandelstamT, weight);
    histograms->h1_PiPlusPiMinusGamma1Gamma2InvMass->Fill(m_PiPlusPiMinusGamma1Gamma2, weight);
    histograms->h1_Gamma1Gamma2InvMass->Fill(m_Gamma1Gamma2, weight);
    histograms->h1_Dalitz_X->Fill(X_c, weight);
    histograms->h1_Dalitz_Y->Fill(Y_c, weight);
    histograms->h2_DalitzPlot->Fill(X_c, Y_c, weight);
    histograms->h1_PhotonTheta1->Fill(g1_p4_kin->Theta()*TMath::RadToDeg(), weight);
    histograms->h1_PhotonTheta2->Fill(g2_p4_kin->Theta()*TMath::RadToDeg(), weight);
    histograms->h1_zVertex->Fill(zVertex, weight);
    histograms->h1_KinFitProb->Fill(kfit_prob, weight);

  }
  outputThreadTree->outputFile->cd();
  outputThreadTree->outputTree->Write();
  outputThreadTree->outputFile->Write();

  // delete outputThreadTree;
  delete dataChainProcess;
}

void parallelProcess(TChain* dataChain, Bool_t is_mc, CutConfig cutConfig, SidebandSubtractionParameters sidebandParameters, Int_t nThreads) {

    const Long64_t nEntries = dataChain->GetEntries();
    // const unsigned nThreads = std::thread::hardware_concurrency();
    cout << "Number of threads: " << nThreads << endl;
    ROOT::TThreadExecutor executor(nThreads);

    // Calculate the number of ranges based on the desired number of threads or executor's default pool size
    
    const Long64_t entriesPerThread = nEntries / nThreads;

    std::vector<std::pair<Int_t, Int_t>> entryRanges;
    for (unsigned i = 0; i < nThreads; ++i) {
        Long64_t startEntry = i * entriesPerThread;
        Long64_t endEntry = (i + 1) * entriesPerThread - 1;
        if (i == nThreads - 1) {
            endEntry = nEntries - 1;
        }
        entryRanges.push_back({startEntry, endEntry});
    }

    ThreadHistograms* histograms[nThreads];
    ThreadTree* outputThreadTree[nThreads];

    // Foreach requires a function that can be applied to each element of a collection
    auto processRange = [dataChain, is_mc, cutConfig, sidebandParameters, &histograms, &outputThreadTree, entriesPerThread](const std::pair<Int_t, Int_t>& range) {
        Long64_t startEntry = range.first;
        Long64_t endEntry = range.second;
        unsigned idx = startEntry/entriesPerThread;
        processEntryRange(dataChain, is_mc, cutConfig, sidebandParameters, startEntry, endEntry, histograms[idx], outputThreadTree[idx]);
    };

    // cout << "Processing " << nEntries << " entries with " << nThreads << " threads" << endl;

    // Use Foreach to process each range
    executor.Foreach(processRange, entryRanges);

    // delete MergedHistograms;
    for (unsigned i = 0; i < nThreads; ++i) {
        delete outputThreadTree[i];
    }

    // Merge trees
    if (sidebandParameters.WeightSidebands == 0.0) {
        ThreadTree* MergedOutputTree = new ThreadTree(treeThreadFileName, "nt", treeThreadDirName, -1);
        MergedOutputTree->merge(kTRUE);
    }
    else {
        ThreadTree* MergedOutputTree = new ThreadTree(Form("%s_SidebandSubtracted",treeThreadFileName.Data()), "nt", treeThreadDirName, -1);
        MergedOutputTree->merge(kTRUE);
    }
}


class SystematicVariation {
  public:
    SystematicVariation(TString SystematicVariation, Int_t SystematicVariationIdx) {
      // Initialize
      SetSystematicVariation(SystematicVariation, SystematicVariationIdx);
    }
    ~SystematicVariation() {
      // Destructor
    }
    void SetSystematicVariation(TString SystematicVariation, Int_t SystematicVariationIdx) {
      if (SystematicVariation == "MandelstamT") {
        values[0] = MandelstamTCutRange[SystematicVariationIdx];
        values[1] = MandelstamTCutRange[SystematicVariationIdx+1];
      }
      else if (SystematicVariation == "PhotonBeamEnergy") {
        values[0] = PhotonBeamEnergyRange[SystematicVariationIdx];
        values[1] = PhotonBeamEnergyRange[SystematicVariationIdx+1];
      }
      else if (SystematicVariation == "BkgModel") {
        bkgModelValue = bkgModel[SystematicVariationIdx];
      }
      else if (SystematicVariation == "Pi0Mass") {
        if (SystematicVariationIdx) {
          Pi0MassRange[0] += 0.01;
          Pi0MassRange[1] -= 0.01;
        }
        else {
          Pi0MassRange[0] -= 0.01;
          Pi0MassRange[1] += 0.01;
        }
      }
      else if (SystematicVariation == "ConfidenceLevel") {
        values[0] = ConfidenceLevelCut[SystematicVariationIdx];
      }
      else if (SystematicVariation == "SidebandPosition") {
        values[0] = 0.0;
        values[1] = 0.0;
        leftSidebandRange[0] -= 0.01*SystematicVariationIdx;
        leftSidebandRange[1] -= 0.01*SystematicVariationIdx;
        rightSidebandRange[0] += 0.01*SystematicVariationIdx;
        rightSidebandRange[1] += 0.01*SystematicVariationIdx;
      }
      else if (SystematicVariation == "SidebandWidth") {
        values[0] = 0.0;
        values[1] = 0.0;
        leftSidebandRange[0] -= 0.01*SystematicVariationIdx;
        rightSidebandRange[1] += 0.01*SystematicVariationIdx;
      }
      else if (SystematicVariation == "ZVertex") {
        MissingZVertexRange[0] += zVertexShift[SystematicVariationIdx];
        MissingZVertexRange[1] -= zVertexShift[SystematicVariationIdx];
      }
    }

    Double_t GetSystematicVariationValue(Int_t idx=0) {
      return values[idx];
    }

    BkgModel GetSystematicBkgVariation() {
      return bkgModelValue;
    }

  private:
    Double_t values[2];
    BkgModel bkgModelValue;
    // const Double_t PhotonBeamEnergyRange[9] = {3.0,4.0,5.0,6.5,7.5,8.0,9.0,10.0,11.6};
    const Double_t PhotonBeamEnergyRange[6] = {3.0,4.5,6.5,8.5,9.5,11.6};
    // add more systematic variations here
    const Double_t MandelstamTCutRange[2] = {0.15,0.6};
    const BkgModel bkgModel[3] = {ARGUS,CHEBYCHEV,EXPPOLY};
    const Double_t ConfidenceLevelCut[4] = {0.0001,0.0005,0.005,0.01};
    const Double_t zVertexShift[2] = {8,20};
};

SidebandSubtractionParameters GetSidebandParameters(TString pathToFile, TString fitFileNameTag, BkgModel bkgModel) {
  // check if the merged data file exists
  TFile *f = TFile::Open(pathToFile.Data(),"READ");
  if (!f) {
    cout << "Merged data file does not exist." << endl;
    exit(1);
  }
  // TTree *t = (TTree*)f->Get("nt");
  // cout << "nEntries = " << t->GetEntries() << endl;
  TDirectory *dir = f->GetDirectory("Histograms");
  TH1F *h1_PiPlusPiMinusGamma1Gamma2InvMass = (TH1F*)dir->Get("h1_PiPlusPiMinusGamma1Gamma2InvMass");

  // Fit the histogram with a Gaussian and polynomial
  RooRealVar x("h1_PiPlusPiMinusGamma1Gamma2InvMass_RooFit","Eta->3Pi invariant mass (GeV)",0.45,0.65);
  RooDataHist data("data","data",RooArgList(x),h1_PiPlusPiMinusGamma1Gamma2InvMass);
   
  TCanvas *canvas = new TCanvas("c","c",800,500);
  RooPlot *plotMass = x.frame();
  plotMass->SetTitle(h1_PiPlusPiMinusGamma1Gamma2InvMass->GetTitle());
  plotMass->SetXTitle(h1_PiPlusPiMinusGamma1Gamma2InvMass->GetXaxis()->GetTitle());
  plotMass->SetTitleOffset(1.1,"Y");
    
  const Double_t eta_mass = 0.547862;
  RooRealVar meanSgn1("meanSgn1","meanSgn1",eta_mass,eta_mass-0.002,eta_mass+0.002);
  RooRealVar sigmaSgn1("sigmaSgn1","sigmaSgn1",0.002,0.001,0.02);
  RooGaussian gauss1("gauss1","gauss1",x,meanSgn1,sigmaSgn1);
  RooRealVar meanSgn2("meanSgn2","meanSgn2",eta_mass,eta_mass-0.002,eta_mass+0.002);
  RooRealVar sigmaSgn2("sigmaSgn2","sigmaSgn2",0.002,0.005,0.02);
  RooGaussian gauss2("gauss2","gauss2",x,meanSgn2,sigmaSgn2);
  RooRealVar cGauss2("cGauss2","cGauss2",0.3,0.0,0.5);
  RooRealVar cBkg("cBkg","cBkg",0.1,0.0,1.0);
  RooAddPdf *fitFunction = nullptr;
  RooAbsPdf *bkgFunction = nullptr;

  RooRealVar bkgParam[5];

  switch (bkgModel) {
    case CHEBYCHEV: {
      // construct Chebychev polynomial background function
      RooRealVar a("a","a",1.5,0.01,3.0);
      RooRealVar b("b","b",0.5,0.01,1.5);
      RooRealVar c("c","c",0.3,0.01,1.0);
      RooRealVar d("d","d",0.2,0.01,1.0);
      RooRealVar e("e","e",0.05,0.001,1.0);
      bkgParam[0] = a;
      bkgParam[1] = b;
      bkgParam[2] = c;
      bkgParam[3] = d;
      bkgParam[4] = e;
      bkgFunction = new RooChebychev("chebyBkg","chebychev polyomial background",x,RooArgList(bkgParam[0],bkgParam[1],bkgParam[2],bkgParam[3],bkgParam[4]));
      cout << "Background function: Chebychev polynomial" << endl;
      break;
    }
    case ARGUS: {
      // construct ARGUS background function
      RooRealVar m0("m0","m0",0.782);
      RooRealVar c("c","c",-10.0,-100.0,0.0);
      bkgParam[0] = m0;
      bkgParam[1] = c;
      bkgFunction = new RooArgusBG("argus","argus",x,bkgParam[0],bkgParam[1]);
      cout << "Background function: ARGUS" << endl;
      break;
    }
    case EXPPOLY: {
      // construct exponential(polynomial) background function
      RooRealVar p0("p0","p0",10.0,0.00,20.0);
      RooRealVar p1("p1","p1",6.0,0.00,10.0);
      // RooRealVar p2("p2","p2",0.5,0.00,1.0);
      bkgParam[0] = p0;
      bkgParam[1] = p1;
      // bkgParam[2] = p2;
      bkgFunction = new RooGenericPdf("exppoly","exponential of polynomial","TMath::Exp(@1*@0+@2*@0*@0)",RooArgList(x,bkgParam[0],bkgParam[1]));
      cout << "Background function: Exponential of Polynomial" << endl;
      break;
    }
  }

  fitFunction = new RooAddPdf("gauspoly","gauspoly",RooArgList(*bkgFunction,gauss2,gauss1),RooArgList(cBkg,cGauss2));

  // construct Chebychev polynomial background function
  
  // RooRealVar a("a","a",1.5,0.01,3.0);
  // RooRealVar b("b","b",0.5,0.01,1.5);
  // RooRealVar c("c","c",0.3,0.01,1.0);
  // RooRealVar d("d","d",0.2,0.01,1.0);
  // RooRealVar e("e","e",0.05,0.001,1.0);
  // RooChebychev chebyBkg("chebyBkg","chebychev polyomial background",x,RooArgList(a,b,c,d,e));
  // RooRealVar cBkg1("cPolyBkg","cPolyBkg",0.1,0.0,1.0);
  // RooAddPdf fitFunction("gauspoly","gauspoly",RooArgList(chebyBkg,gauss2,gauss1),RooArgList(cBkg1,cGauss2));

  // construct ARGUS background function
  // RooRealVar m0("m0","m0",0.782);
  // RooRealVar c("c","c",-10.0,-100.0,0.0);
  // RooArgusBG argusBkg("argus","argus",x,m0,c);
  // RooRealVar cBkg1("cBkg1","cBkg1",0.1,0.0,1.0);
  // RooAddPdf fitFunction("gauspoly","gauspoly",RooArgList(argusBkg,gauss2,gauss1),RooArgList(cBkg1,cGauss2));
  
  fitFunction->fitTo(data,RooFit::PrintLevel(1),RooFit::SumW2Error(kTRUE));

  data.plotOn(plotMass,MarkerSize(0.5));
  fitFunction->plotOn(plotMass,LineColor(kRed),LineWidth(2));
  fitFunction->plotOn(plotMass,Components("gauss1"),LineColor(kBlack),LineStyle(kDashed),LineWidth(1));
  fitFunction->plotOn(plotMass,Components("gauss2"),LineColor(kBlue),LineStyle(kDashed),LineWidth(1));
  switch (bkgModel) {
    case CHEBYCHEV: {
      fitFunction->plotOn(plotMass,Components("chebyBkg"),LineColor(kGreen),LineStyle(kDashed),LineWidth(1));
      break;
    }
    case ARGUS: {
      fitFunction->plotOn(plotMass,Components("argus"),LineColor(kGreen),LineStyle(kDashed),LineWidth(1));
      break;
    }
    case EXPPOLY: {
      fitFunction->plotOn(plotMass,Components("exppoly"),LineColor(kGreen),LineStyle(kDashed),LineWidth(1));
      break;
    }
  }
  fitFunction->paramOn(plotMass,Layout(0.1,0.4,0.9),Format("NU",AutoPrecision(0)));
  // plotMass->GetYaxis()->SetRangeUser(0,180000);
  plotMass->Draw();
  canvas->SaveAs(Form("%sfit_M3Pi_%s.pdf",outDir.Data(),fitFileNameTag.Data()));
  /*
    if (!fitOnly) plotMass->Write();

    if (!is_mc) {
      TString outMassFitPdfName = outDir+"pippimg1g2massfit_";
      switch(data_set){
        case 0: {
          outMassFitPdfName += "2017_";
          break;
          }
        case 1: {
          outMassFitPdfName += "2018S_";
          break;
        }
        case 2: {
          outMassFitPdfName += "2018F_";
          break;
        }
        case 3: {
          outMassFitPdfName += "all_";
          break;
        }
      }
      outMassFitPdfName += cutTag;
      if (enablePhotonBeamEnergyCut) {
        outMassFitPdfName += "_";
        outMassFitPdfName += PhotonBeamEnergyRangeIdx;
      }

      outMassFitPdfName += ".pdf";
      canvas->Print(outMassFitPdfName);
    }

    */

      //calculate weight sidebands using trapezoid formulas
      // x.setVal(leftSidebandRange[0]);
      // Double_t lowerLSBCount = cBkg1.getVal()*chebyBkg.getVal();
      // x.setVal(leftSidebandRange[1]);
      // Double_t upperLSBCount = cBkg1.getVal()*chebyBkg.getVal();
      // Double_t LSBarea = (lowerLSBCount+upperLSBCount)*(leftSidebandRange[1]-leftSidebandRange[0])/2;
      // x.setVal(rightSidebandRange[0]);
      // Double_t lowerRSBCount = cBkg1.getVal()*chebyBkg.getVal();
      // x.setVal(rightSidebandRange[1]);
      // Double_t upperRSBCount = cBkg1.getVal()*chebyBkg.getVal();
      // Double_t RSBarea = (lowerLSBCount+upperLSBCount)*(leftSidebandRange[1]-leftSidebandRange[0])/2;
      // Double_t LSBWeight = -LSBarea/(LSBarea+RSBarea);
      // Double_t RSBWeight = -RSBarea/(LSBarea+RSBarea);

      //calculate weighted average of mean and sigma of the two gaussian signal region
      x.setRange("signalRange1",0.5,0.6);
      Double_t gauss1Yield = gauss1.createIntegral(x,"signalRange1")->getVal();
      x.setRange("signalRange2",0.5,0.6);
      Double_t gauss2Yield = gauss2.createIntegral(x,"signalRange2")->getVal();
      Double_t meanSgnVal = (gauss1Yield*meanSgn1.getVal() + gauss2Yield*meanSgn2.getVal())/(gauss1Yield + gauss2Yield);
      Double_t sigmaSgnVal = (gauss1Yield*sigmaSgn1.getVal() + gauss2Yield*sigmaSgn2.getVal())/(gauss1Yield + gauss2Yield);
      // cout << "meanSgnVal = " << meanSgnVal << endl;
      // fitInformation.push_back(meanSgnVal);
      // fitInformationLabel.push_back("meanSgnVal");
      // cout << "sigmaSgnVal = " << sigmaSgnVal << endl;
      // fitInformation.push_back(sigmaSgnVal);
      // fitInformationLabel.push_back("sigmaSgnVal");

      // Double_t TotalEvents = fitFunction.expectedEvents(RooArgSet(x));
      //calculate weight of sidebands using integral (area) of each band from background model
      Double_t signalRange[2] = {meanSgnVal-2*sigmaSgnVal,meanSgnVal+2*sigmaSgnVal};
      // ranges.push_back(signalRange[0]);
      // rangesLabel.push_back("Signal lower limit");
      // ranges.push_back(signalRange[1]);
      // rangesLabel.push_back("Signal upper limit");
      // ranges.push_back(leftSidebandRange[0]);
      // rangesLabel.push_back("Left sideband lower limit");
      // ranges.push_back(leftSidebandRange[1]);
      // rangesLabel.push_back("Left sideband upper limit");
      // ranges.push_back(rightSidebandRange[0]);
      // rangesLabel.push_back("Right sideband lower limit");
      // ranges.push_back(rightSidebandRange[1]);
      // rangesLabel.push_back("Right sideband upper limit");
      // outfile->WriteObject(&ranges,"ranges");
      // outfile->WriteObject(&rangesLabel,"rangesLabel");
      x.setRange("signalRange",signalRange[0],signalRange[1]);
      Double_t bkgSignalRangeArea = bkgFunction->createIntegral(x,"signalRange")->getVal();
      x.setRange("leftSidebandRange",leftSidebandRange[0],leftSidebandRange[1]);
      Double_t LSBArea = bkgFunction->createIntegral(x,"leftSidebandRange")->getVal();
      x.setRange("rightSidebandRange",rightSidebandRange[0],rightSidebandRange[1]);
      Double_t RSBArea = bkgFunction->createIntegral(x,"rightSidebandRange")->getVal();

      // cout << "Total Events (integrated) = " << TotalEvents << endl;
      cout << "bkgSignalRangeArea = " << bkgSignalRangeArea << endl;
      // fitInformation.push_back(bkgSignalRangeArea);
      // fitInformationLabel.push_back("bkgSignalRangeArea");
      cout << "LSBArea = " << LSBArea << endl;
      // fitInformation.push_back(LSBArea);
      // fitInformationLabel.push_back("LSBArea");
      cout << "RSBArea = " << RSBArea << endl;
      // fitInformation.push_back(RSBArea);
      // fitInformationLabel.push_back("RSBArea");

      Double_t weightSideband = -1.0*bkgSignalRangeArea/(LSBArea+RSBArea); 

  cout << "weightSideband = " << weightSideband << endl;
  // fitInformation.push_back(weightSideband);
  // fitInformationLabel.push_back("weightSideband");
  // outfile->WriteObject(&fitInformation,"fitInformation");
  // outfile->WriteObject(&fitInformationLabel,"fitInformationLabel");
  cout << "bkgSignal + sideband = " << bkgSignalRangeArea + (weightSideband*LSBArea) + (weightSideband*RSBArea) << endl;

  SidebandSubtractionParameters sidebandParameters;
  sidebandParameters.GaussianMean = meanSgnVal;
  sidebandParameters.GaussianSigma = sigmaSgnVal;
  sidebandParameters.LeftSidebandRange[0] = leftSidebandRange[0];
  sidebandParameters.LeftSidebandRange[1] = leftSidebandRange[1];
  sidebandParameters.RightSidebandRange[0] = rightSidebandRange[0];
  sidebandParameters.RightSidebandRange[1] = rightSidebandRange[1];
  sidebandParameters.SignalRange[0] = signalRange[0];
  sidebandParameters.SignalRange[1] = signalRange[1];
  sidebandParameters.WeightSidebands = weightSideband;

  return sidebandParameters;
}

void Eta3PiSelectorParallel(int data_set,TString outName,bool is_mc, TString cutTag, TString SystematicVariationStr, Int_t SystematicVariationIdx, Int_t nThreads) {
  Initialize();
  TStopwatch timer;
  timer.Start();
  if (data_set != 0 && SystematicVariationStr == "PhotonBeamEnergy") {
    SystematicVariationIdx += 2; // start from 6.5 GeV
  }

  treeThreadDirName += "_";
  treeThreadDirName += cutTag;
  treeThreadDirName += "/";

  treeThreadFileName += "_";
  treeThreadFileName += outName;
  treeThreadFileName += "_";
  treeThreadFileName += cutTag;
  if (SystematicVariationStr != "") {
    treeThreadFileName += "_";
    treeThreadFileName += SystematicVariationStr;
    treeThreadFileName += "_";
    treeThreadFileName += SystematicVariationIdx;
  }

  SystematicVariation systematicVariation(SystematicVariationStr, SystematicVariationIdx);

  Double_t PhotonBeamEnergyRange[2] = {6.5,11.6}; // default range for CCDB flux
  if ((SystematicVariationStr!="PhotonBeamEnergy") && (data_set==0)) {
    // default range for 2017 data
    PhotonBeamEnergyRange[0] = 3.0;
    PhotonBeamEnergyRange[1] = 11.6;
  }

  if (SystematicVariationStr == "PhotonBeamEnergy") {
    PhotonBeamEnergyRange[0] = systematicVariation.GetSystematicVariationValue(0);
    PhotonBeamEnergyRange[1] = systematicVariation.GetSystematicVariationValue(1);
  }
  if (SystematicVariationStr == "MandelstamT") {
    MandelstamTCutRange[0] = systematicVariation.GetSystematicVariationValue(0);
    MandelstamTCutRange[1] = systematicVariation.GetSystematicVariationValue(1);
  }
  if (SystematicVariationStr == "BkgModel") {
    bkgModel = systematicVariation.GetSystematicBkgVariation();
  }
  if (SystematicVariationStr == "ConfidenceLevel") {
    kfit_cut = systematicVariation.GetSystematicVariationValue(0);
  }

  //vector to save cut configuration
  // vector<string> cutConfigStr = getCutConfig(cutTag, enablePhotonBeamEnergyCut, PhotonBeamEnergyRangeIdx);
  // vector<bool> cutConfigBool;
  // cutConfigBool.push_back(is_mc);
  // cutConfigBool.push_back(enableKfitCut);
  // cutConfigBool.push_back(enablePi0MassCut);
  // cutConfigBool.push_back(enablePhotonsThetaCut);
  // cutConfigBool.push_back(enablePhotonBeamEnergyCut);
  // cutConfigBool.push_back(enableSidebandSub);
  // cutConfigBool.push_back(enableResolutionAnalysis);
  // cutConfigBool.push_back(enableKinematicCut);
  // vector<string> cutConfigBoolLabel;
  // cutConfigBoolLabel.push_back("is_mc");
  // cutConfigBoolLabel.push_back("enableKfitCut");
  // cutConfigBoolLabel.push_back("enablePi0MassCut");
  // cutConfigBoolLabel.push_back("enablePhotonsThetaCut");
  // cutConfigBoolLabel.push_back("enablePhotonBeamEnergyCut");
  // cutConfigBoolLabel.push_back("enableSidebandSub");
  // cutConfigBoolLabel.push_back("enableResolutionAnalysis");
  // cutConfigBoolLabel.push_back("enableKinematicCut");


  //define input tree chain
  TChain *dataChain = new TChain("myTree");
  TFile *mcThrown;
  TString outNameDataForMC;

  addDataToChain(dataChain, data_set, is_mc);
  // if (is_mc) setMcThrownAndOutName(mcThrown, outNameDataForMC, data_set, enablePhotonBeamEnergyCut, cutTag, PhotonBeamEnergyRangeIdx);

  // TFile *fKinematicAcc;
  // TGraph *DP_kinAccR;
  // if (enableKinematicCut){
  //   fKinematicAcc = TFile::Open("/d/home/dlersch/gluex_analysis/Eta_3Pi_Dalitz/kinematic_acceptance_ratio13.root","read");
  //   DP_kinAccR = (TGraph*)fKinematicAcc->Get("DP_kinAccR");
  // }

  // TFile *outfile;
  // if (!fitOnly) {
  //   outfile = new TFile(outName,"RECREATE");
  //   outfile->WriteObject(&cutConfigStr,"cutConfigStr");
  //   outfile->WriteObject(&cutConfigBool,"cutConfigBool");
  //   outfile->WriteObject(&cutConfigBoolLabel,"cutConfigBoolLabel");
  // }

  TTree *out_tree = new TTree("nt","nt");

  CutConfig cutConfig;
  cutConfig.enablePhotonBeamEnergyCut = kTRUE;
  cutConfig.PhotonBeamEnergyRange[0] = PhotonBeamEnergyRange[0];
  cutConfig.PhotonBeamEnergyRange[1] = PhotonBeamEnergyRange[1];
  cutConfig.enableKfitCut = enableKfitCut;
  cutConfig.kfit_cut = kfit_cut;
  cutConfig.enablePi0MassCut = enablePi0MassCut;
  cutConfig.Pi0MassRange[0] = Pi0MassRange[0];
  cutConfig.Pi0MassRange[1] = Pi0MassRange[1];
  cutConfig.enablePhotonsThetaCut = enablePhotonsThetaCut;
  cutConfig.enableMandelstamTCut = kTRUE;
  cutConfig.MandelstamTCutRange[0] = MandelstamTCutRange[0];
  cutConfig.MandelstamTCutRange[1] = MandelstamTCutRange[1];
  cutConfig.MissingZVertexRange[0] = MissingZVertexRange[0];
  cutConfig.MissingZVertexRange[1] = MissingZVertexRange[1];

  SidebandSubtractionParameters sidebandParameters;
  sidebandParameters.WeightSidebands = 0.0; // default value
  if (!is_mc) parallelProcess(dataChain, is_mc, cutConfig, sidebandParameters, nThreads);
  
  TString RunPeriodName;
  if (data_set == 0) RunPeriodName = "2017";
  else if (data_set == 1) RunPeriodName = "2018S";
  else if (data_set == 2) RunPeriodName = "2018F";
  else if (data_set == 3) RunPeriodName = "all";

  outName += "_";
  outName += cutTag;
  outName += "_";
  outName += SystematicVariationStr;
  outName += "_";
  outName += SystematicVariationIdx;
  outName += ".root";

  TString outFileName = Form("%s%s",treeThreadDirName.Data(),outName.Data());
  TString fitFileNameTag = Form("%s_%s_%s_%d",RunPeriodName.Data(),cutTag.Data(),SystematicVariationStr.Data(),SystematicVariationIdx);
  if (is_mc) {
    TString outFileNameData = Form("%sWOSBS_data_%s_%s_%s_%d.root",treeThreadDirName.Data(),RunPeriodName.Data(),cutTag.Data(),SystematicVariationStr.Data(),SystematicVariationIdx);
    sidebandParameters = GetSidebandParameters(outFileNameData,fitFileNameTag,bkgModel);
  }
  else {
    TString mergedFileName = Form("%smerged_%s.root",treeThreadDirName.Data(),treeThreadFileName.Data());
    sidebandParameters = GetSidebandParameters(mergedFileName,fitFileNameTag,bkgModel);
    TString outFileNameWOSBS = Form("%sWOSBS_%s",treeThreadDirName.Data(),outName.Data());
    gSystem->Rename(mergedFileName,outFileNameWOSBS);
  }
  sidebandParameters.Print();
  parallelProcess(dataChain, is_mc, cutConfig, sidebandParameters, nThreads);

  // rename merged file to outName
  if (enableSidebandSub) {
    TString mergedFileName = Form("%smerged_%s_SidebandSubtracted.root",treeThreadDirName.Data(),treeThreadFileName.Data());
    cout << "Renaming " << mergedFileName << " to " << outFileName << endl;
    gSystem->Rename(mergedFileName,outFileName);
  }

  // outfile->cd();
  // out_tree->Write();
  // outfile->Close();

  //vector to save ranges information
  vector<Double_t> ranges;
  vector<string> rangesLabel;

  //vector to save fit information
  vector<Double_t> fitInformation;
  vector<string> fitInformationLabel;

  timer.Stop();
  timer.Print();
}

void setHPipPimG1G2Axis(TH1F* h){
  h->GetXaxis()->SetTitle("M_{#pi^{+}#pi^{-}#gamma#gamma} (GeV)");
  h->GetXaxis()->SetLabelSize(0.025);
  h->GetYaxis()->SetTitle("count");
  h->GetYaxis()->SetTitleOffset(1.6);
  h->GetYaxis()->SetLabelSize(0.025);
}

void setHG1G2Axis(TH1F* h){
  h->GetXaxis()->SetTitle("M_{#gamma#gamma} (GeV)");
  h->GetXaxis()->SetLabelSize(0.025);
  h->GetYaxis()->SetTitle("count");
  h->GetYaxis()->SetTitleOffset(1.6);
  h->GetYaxis()->SetLabelSize(0.025);
}

Double_t AcceptanceProb(TGraph *gr,Double_t X, Double_t Y){
  const Int_t N = 13;
  const Double_t delta = 2.2/N;
  Double_t global_bin_data = TMath::FloorNint((X+1.1)/delta) + N*TMath::FloorNint((Y+1.1)/delta);
  return gr->Eval(global_bin_data,nullptr,"S"); //using cubic spline interpolation
}

void ProgressBar(Long64_t i, Long64_t n) {
    const int barWidth = 70;
    float progress = (i+1) * 1.0 / n;
    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
}