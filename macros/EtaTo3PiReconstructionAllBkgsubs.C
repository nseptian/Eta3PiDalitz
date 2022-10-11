using namespace RooFit;

//kfit_cut value represents a cut on the kinematic fit probability!
const Double_t kfit_cut = 0.01;

//user config for pi_0 invariant mass range cut
const Bool_t enablePi0MassCut = kFALSE;
const Double_t Pi0MassRange[2] = {0.11,0.165}; //GeV

//user config for theta photons cut
const Bool_t enablePhotonsThetaCut = kTRUE;

//user config for sideband subtraction
const Bool_t enableSidebandSubs = kTRUE;
const Bool_t fitOnly = kFALSE;
const Bool_t signalOnlyTree = kTRUE;
const Double_t width = 0.025;
// const Double_t signalRange[2] = {0.53,0.565}; //commented -> determine using sigma value from the fit
const Double_t leftSidebandRange[2] = {0.48-width,0.48};
const Double_t rightSidebandRange[2] = {0.61,0.61+width};

const Double_t signalRangeMC[2] = {0.52585909,0.56975488}; //2017_data_sbs_10092022

void setHPipPimG1G2Axis(TH1F* h);
void setHG1G2Axis(TH1F* h);

void EtaTo3PiReconstructionAllBkgsubs(int data_set,TString outName,bool is_mc, TString cutTag){
  
  //define input tree chain
  TChain *dataChain = new TChain("myTree");
  if(!is_mc){
    if (data_set == 0){
      //GlueX 2017 datasets
      cout << "Dataset used is GlueX 2017." << endl;
      if (enableSidebandSubs){
        dataChain->Add("/d/grid17/dlersch/AmpTool_Data/data2017/PiPiGG_Tree_2017_data_sideband.root");
      }
      else{
        dataChain->Add("/d/grid17/dlersch/AmpTool_Data/data2017/PiPiGG_Tree_2017_data.root");
      }
    }
    if (data_set == 1){
      //GlueX 2018S datasets
      cout << "Dataset used is GlueX 2018S." << endl;
      if (enableSidebandSubs){
        dataChain->Add("/d/grid17/dlersch/AmpTool_Data/data2018S/PiPiGG_Tree_2018S_data_sideband.root");
      }
      else{
        dataChain->Add("/d/grid17/dlersch/AmpTool_Data/data2018S/PiPiGG_Tree_2018S_data.root");
      }
    }
    if (data_set == 2){
      //GlueX 2018F datasets
      cout << "Dataset used is GlueX 2018F." << endl;
      if (enableSidebandSubs){
        cout << "GlueX 2018F datasets with sidebands has not been available..." << endl << "Please choose another datasets. Currently available datasets with sideband are 2017 and 2018S." << endl;
        exit(1);
      }
      else{
        dataChain->Add("/d/grid17/dlersch/AmpTool_Data/data2018F/PiPiGG_Tree_2018F_data.root");
      }
    }
    if (data_set == 3){
      //GlueX-Phase I datasets
      cout << "Dataset used is GlueX Phase-I." << endl;
      if (enableSidebandSubs){
        dataChain->Add("/d/grid17/dlersch/AmpTool_Data/data2017/PiPiGG_Tree_2017_data_sideband.root");
        dataChain->Add("/d/grid17/dlersch/AmpTool_Data/data2018S/PiPiGG_Tree_2018S_data_sideband.root");
        //add 2018F datasets if available
      }
      else{
        dataChain->Add("/d/grid17/dlersch/AmpTool_Data/data2017/PiPiGG_Tree_2017_data.root");
        dataChain->Add("/d/grid17/dlersch/AmpTool_Data/data2018S/PiPiGG_Tree_2018S_data.root");
        dataChain->Add("/d/grid17/dlersch/AmpTool_Data/data2018F/PiPiGG_Tree_2018F_data.root");
      }
    }
  }
  else{
    if (data_set == 0) {
      //MC 2017
      dataChain->Add("/d/grid16/dlersch/eta3pi_mc_data_2017/geant4/ds_trees/PiPiGG_Tree_runSim_Geant4_MC2017_all.root");
    }
    if (data_set == 1) {
      //MC 2018S
      dataChain->Add("/d/grid16/dlersch/eta3pi_mc_data_2018S/geant4/ds_trees/PiPiGG_Tree_runSim_Geant4_MC2018S_all.root");
    }
    if (data_set == 2) {
      //MC 2018F
      dataChain->Add("/d/grid16/dlersch/eta3pi_mc_data_2018F/geant4/ds_trees/PiPiGG_Tree_runSim_Geant4_MC2018F_all.root");
    }
    if (data_set == 3){
      dataChain->Add("/d/grid16/dlersch/eta3pi_mc_data_2017/geant4/ds_trees/PiPiGG_Tree_runSim_Geant4_MC2017_all.root");
      dataChain->Add("/d/grid16/dlersch/eta3pi_mc_data_2018S/geant4/ds_trees/PiPiGG_Tree_runSim_Geant4_MC2018S_all.root");
      // dataChain->Add("/d/grid16/dlersch/eta3pi_mc_data_2018F/geant4/ds_trees/PiPiGG_Tree_runSim_Geant4_MC2018F_all.root");
    }
  }
  Int_t nEntries = dataChain->GetEntries();
  cout << "Number of entries: " << nEntries << endl;

  //initialize variables on the tree
  Double_t kfit_prob = 0.0;
  Double_t imassGG_kfit = 0.0;
  Double_t DP_X = 0.0;
  Double_t DP_Y = 0.0;
  Double_t dBRT = 0.0;
  Double_t zVertex = 0.0;
  TLorentzVector *pip_p4_kin = 0;
  TLorentzVector *pim_p4_kin = 0;
  TLorentzVector *g1_p4_kin = 0;
  TLorentzVector *g2_p4_kin = 0;
  Bool_t is_truecombo;

  dataChain->SetBranchStatus("*",0);
  dataChain->SetBranchStatus("kfit_prob",1);
  dataChain->SetBranchStatus("dBRT",1);
  dataChain->SetBranchStatus("pim_p4_kin",1);
  dataChain->SetBranchStatus("pip_p4_kin",1);
  dataChain->SetBranchStatus("g1_p4_kin",1);
  dataChain->SetBranchStatus("g2_p4_kin",1);
  
  dataChain->SetBranchAddress("kfit_prob",&kfit_prob);
  dataChain->SetBranchAddress("dBRT",&dBRT);
  dataChain->SetBranchAddress("pip_p4_kin",&pip_p4_kin); //4-vector of pi+
  dataChain->SetBranchAddress("pim_p4_kin",&pim_p4_kin); //4-vector of pi- 
  dataChain->SetBranchAddress("g1_p4_kin",&g1_p4_kin); //4-vector of photon1 (gamma 1)
  dataChain->SetBranchAddress("g2_p4_kin",&g2_p4_kin); //4-vector of photon2 (gamma 2)
  
  if(is_mc){
      dataChain->SetBranchStatus("is_truecombo",1);
      dataChain->SetBranchAddress("is_truecombo",&is_truecombo);
  }else{
      is_truecombo = true;
  }

  if (enableSidebandSubs) outName += "_sbs";
  if (signalOnlyTree && !is_mc) outName += "_signalOnlyTree";
  outName += "_";
  outName += cutTag;
  outName += ".root";

  TFile *outfile;
  if (!fitOnly) outfile = new TFile(outName,"RECREATE");

  //Declare histogram before the fits here
  TH1F* hpippimg1g2mass;

  //Define output tree
  Double_t EnP1,PxP1,PyP1,PzP1;
  Double_t EnP2,PxP2,PyP2,PzP2;
  Double_t EnP3,PxP3,PyP3,PzP3;
  Double_t X,Y,weight=1.0;
  TTree *out_tree = new TTree("nt","nt");
  //Pi+
  out_tree->Branch("EnP1",&EnP1,"EnP1/D");
  out_tree->Branch("PxP1",&PxP1,"PxP1/D");
  out_tree->Branch("PyP1",&PyP1,"PyP1/D");
  out_tree->Branch("PzP1",&PzP1,"PzP1/D");
  
  //Pi-
  out_tree->Branch("EnP2",&EnP2,"EnP2/D");
  out_tree->Branch("PxP2",&PxP2,"PxP2/D");
  out_tree->Branch("PyP2",&PyP2,"PyP2/D");
  out_tree->Branch("PzP2",&PzP2,"PzP2/D");
  
  //Pi0
  out_tree->Branch("EnP3",&EnP3,"EnP3/D");
  out_tree->Branch("PxP3",&PxP3,"PxP3/D");
  out_tree->Branch("PyP3",&PyP3,"PyP3/D");
  out_tree->Branch("PzP3",&PzP3,"PzP3/D");
  
  // out_tree->Branch("amp",&amp,"amp/D");
  // out_tree->Branch("amp_2",&amp_2,"amp_2/D");
  out_tree->Branch("weight",&weight,"weight/D");

  //vector to save ranges information
  vector<Double_t> ranges;
  
  if (fitOnly) {
    TFile *rootFile = TFile::Open(outName);
    if (!rootFile || rootFile->IsZombie()) {
      std::cerr << "Error opening file, please generate the tree before using fitOnly option." << endl;
      exit(-1);
    }
    cout << "Root file opened for fitOnly: " << outName << endl << endl;
    hpippimg1g2mass = (TH1F*)rootFile->Get("h_pippimg1g2mass");
  }
  else{

    //generate invariant mass histogram and tree without sideband subtraction
    hpippimg1g2mass = new TH1F("h_pippimg1g2mass","#pi^{+} #pi^{-} #gamma #gamma invariant mass spectrum",101,0.45,0.65);
    TH1F *hpippimg1g2massSignalOnly = new TH1F("h_pippimg1g2massSignalOnly","#pi^{+} #pi^{-} #gamma #gamma invariant mass spectrum",101,0.45,0.65);
    TH1F *hg1g2mass = new TH1F("h_g1g2mass","#gamma_1 #gamma_2 invariant mass spectrum",101,0,0.2);
    TH2F *h2DalitzPlotEta3Pi = new TH2F("h2_DalitzPlotEta3Pi","",101,-1.0,1.0,101,-1.0,1.0);
    // const Double_t pi0MassPDG = 0.1349768;
    //++++++++++++++++++++++++++++++++++++++

    cout << "Selecting events..." << endl;
    for(Int_t i=0;i<nEntries;i++){
        dataChain->GetEntry(i);
        
        //Apply cuts:
        //is_truecombo is for MC data (i.e. simulated data) --> Just look at simulated eta->pi+pi-pi0 and nothing else...
  
        if(is_truecombo && kfit_prob > kfit_cut){
          if (!enableSidebandSubs || is_mc){
            EnP1 = pip_p4_kin->E();
            PxP1 = pip_p4_kin->Px();
            PyP1 = pip_p4_kin->Py();
            PzP1 = pip_p4_kin->Pz();

            EnP2 = pim_p4_kin->E();
            PxP2 = pim_p4_kin->Px();
            PyP2 = pim_p4_kin->Py();
            PzP2 = pim_p4_kin->Pz();
            
            EnP3 = (*g1_p4_kin + *g2_p4_kin).E();
            PxP3 = (*g1_p4_kin + *g2_p4_kin).Px();
            PyP3 = (*g1_p4_kin + *g2_p4_kin).Py();
            PzP3 = (*g1_p4_kin + *g2_p4_kin).Pz();
          }

          if(dBRT >= -2.0 && dBRT <= 2.0){
              weight = 1.0;
          }
          else weight = -1.0/8.0;
          
          Double_t m_g1g2 = (*g1_p4_kin + *g2_p4_kin).M();
          Double_t m_pippimg1g2mass = (*g1_p4_kin + *g2_p4_kin + *pip_p4_kin + *pim_p4_kin).M();
          // Double_t diffmassg1g2 = (*g1_p4_kin).M() - (*g2_p4_kin).M();

          // Double_t anglePi0TwoGammas = (*g1_p4_kin).Angle((*g2_p4_kin).Vect());
          Double_t theta_photon1 = (*g1_p4_kin).Theta();
          Double_t theta_photon2 = (*g2_p4_kin).Theta();
          // cout << diffmassg1g2 << endl;

          Bool_t Pi0MassCut = kFALSE;
          if (enablePi0MassCut) Pi0MassCut = (m_g1g2 > Pi0MassRange[0]) && (m_g1g2 < Pi0MassRange[1]);
          else Pi0MassCut = kTRUE; //always true
          
          Bool_t PhotonThetaCut = kFALSE;
          if (enablePhotonsThetaCut) PhotonThetaCut = (theta_photon1*TMath::RadToDeg() < 10.3 || theta_photon1*TMath::RadToDeg() > 11.9) && (theta_photon2*TMath::RadToDeg() < 10.3 || theta_photon2*TMath::RadToDeg() > 11.9);
          else PhotonThetaCut = kTRUE; //always true

          //calculate X and Y
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
          
          if (Pi0MassCut && PhotonThetaCut) {
            hpippimg1g2mass->Fill(m_pippimg1g2mass,weight);
            hg1g2mass->Fill(m_g1g2,weight);
            h2DalitzPlotEta3Pi->Fill(X_c,Y_c,weight);
            if (!enableSidebandSubs || is_mc) out_tree->Fill();
            // h2g1g2massdiff->Fill(m_pi0,diffmassg1g2,weight);
            // h2anglePi0TwoGammas->Fill(m_pi0,anglePi0TwoGammas,weight);
          }
        }
    }
    //++++++++++++++++++++++++++++++++++++++
    
    // cout <<"sum: "<<sum_amp<< endl;
    // cout <<"sum2: "<<sum_amp_2<< endl;

    // gStyle->SetOptStat(0);
    h2DalitzPlotEta3Pi->GetXaxis()->SetTitle("X");
    h2DalitzPlotEta3Pi->GetYaxis()->SetTitle("Y");
    h2DalitzPlotEta3Pi->Write();
    setHPipPimG1G2Axis(hpippimg1g2mass);
    // hpippimg1g2mass->SetStats(0);
    hpippimg1g2mass->Write();
    setHG1G2Axis(hg1g2mass);
    hg1g2mass->GetYaxis()->SetLabelSize(0.025);
    // hg1g2mass->SetStats(0);
    hg1g2mass->Write();

    if (!enableSidebandSubs) out_tree->Write();
  }

  if (enableSidebandSubs && !is_mc) {
    //generate background substracted tree
    
    // cout << "entries = " << hpippimg1g2mass->GetEntries() << endl;
    RooRealVar x("h_pippimg1g2massRooFit","Eta->3Pi invariant mass (GeV)",0.45,0.65);
    
    RooDataHist data("data","data",RooArgList(x),hpippimg1g2mass);
   
    TCanvas *canvas = new TCanvas("c","c",800,500);
    RooPlot *plotMass = x.frame();
    plotMass->SetTitle(hpippimg1g2mass->GetTitle());
    plotMass->SetXTitle(hpippimg1g2mass->GetXaxis()->GetTitle());
    plotMass->SetTitleOffset(1.1,"Y");
    
    const Double_t eta_mass = 0.547862;
    RooRealVar meanSgn1("meanSgn1","meanSgn1",eta_mass,eta_mass-0.002,eta_mass+0.002);
    RooRealVar sigmaSgn1("sigmaSgn1","sigmaSgn1",0.01,0.0001,0.03);
    RooGaussian gauss1("gauss1","gauss1",x,meanSgn1,sigmaSgn1);
    RooRealVar meanSgn2("meanSgn2","meanSgn2",eta_mass,eta_mass-0.002,eta_mass+0.002);
    RooRealVar sigmaSgn2("sigmaSgn2","sigmaSgn2",0.01,0.0001,0.03);
    RooGaussian gauss2("gauss2","gauss2",x,meanSgn2,sigmaSgn2);
    RooRealVar cGauss2("cGauss2","cGauss2",0.01,0.01,0.3);
    RooRealVar a("a","a",1.0,0.,2.0);
    RooRealVar b("b","b",0.3,0.,2.0);
    RooRealVar c("c","c",0.1,0.,2.0);
    RooRealVar d("d","d",0.2,0.,2.0);
    RooRealVar e("e","e",0.,0.,2.0);
    // RooPolynomial polyBkg("polyBkg","polyomial background",x,RooArgList(a));
    RooChebychev chebyBkg("chebyBkg","chebychev polyomial background",x,RooArgList(a,b,c,d,e));
    // RooRealVar meanBkg1("meanBkg1","meanBkg1",mass_bkg1,0.07,0.09);
    // RooRealVar sigmaBkg1("sigmaBkg1","sigmaBkg1",0.005,0.001,0.02);
    // RooGaussian gauss2("gauss2","gauss2",x,meanBkg1,sigmaBkg1);
    RooRealVar cBkg1("cPolyBkg","cPolyBkg",0.2,0.1,1.0);
    // RooRealVar meanBkg2("meanBkg2","meanBkg2",mass_bkg2,0.095,0.13);
    // RooRealVar sigmaBkg2("sigmaBkg2","sigmaBkg2",0.005,0.001,0.03);
    // RooGaussian gauss3("gauss3","gauss3",x,meanBkg2,sigmaBkg2);
    // RooRealVar cBkg2("cBkg2","cBkg2",0.002,0.001,0.3);
    RooAddPdf fitFunction("gauspoly","gauspoly",RooArgList(chebyBkg,gauss2,gauss1),RooArgList(cBkg1,cGauss2));
    fitFunction.fitTo(data,RooFit::PrintLevel(1));
    data.plotOn(plotMass,MarkerSize(0.5));
    fitFunction.plotOn(plotMass,LineColor(kRed),LineWidth(2));
    fitFunction.plotOn(plotMass,Components("gauss1"),LineColor(kBlack),LineStyle(kDashed),LineWidth(1));
    fitFunction.plotOn(plotMass,Components("gauss2"),LineColor(kBlue),LineStyle(kDashed),LineWidth(1));
    fitFunction.plotOn(plotMass,Components("chebyBkg"),LineColor(kGreen),LineStyle(kDashed),LineWidth(1));
    fitFunction.paramOn(plotMass,Layout(0.1,0.4,0.9),Format("NU",AutoPrecision(0)));
    // plotMass->GetYaxis()->SetRangeUser(0,180000);
    plotMass->Draw();
    if (!fitOnly) plotMass->Write();

    if (!is_mc) {
      TString outMassFitPdfName = "pippimg1g2massfit_";
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
      outMassFitPdfName += ".pdf";
      canvas->Print(outMassFitPdfName);
    }

    if ((!fitOnly) && (!is_mc)){

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
      cout << "meanSgnVal = " << meanSgnVal << endl;
      cout << "sigmaSgnVal = " << sigmaSgnVal << endl;

      // Double_t TotalEvents = fitFunction.expectedEvents(RooArgSet(x));
      //calculate weight of sidebands using integral (area) of each band from background model
      Double_t signalRange[2] = {meanSgnVal-2*sigmaSgnVal,meanSgnVal+2*sigmaSgnVal};
      ranges.push_back(signalRange[0]);
      ranges.push_back(signalRange[1]);
      ranges.push_back(leftSidebandRange[0]);
      ranges.push_back(leftSidebandRange[1]);
      ranges.push_back(rightSidebandRange[0]);
      ranges.push_back(rightSidebandRange[1]);
      outfile->WriteObject(&ranges,"ranges");
      x.setRange("signalRange",signalRange[0],signalRange[1]);
      Double_t bkgSignalRangeArea = chebyBkg.createIntegral(x,"signalRange")->getVal();
      x.setRange("leftSidebandRange",leftSidebandRange[0],leftSidebandRange[1]);
      Double_t LSBArea = chebyBkg.createIntegral(x,"leftSidebandRange")->getVal();
      x.setRange("rightSidebandRange",rightSidebandRange[0],rightSidebandRange[1]);
      Double_t RSBArea = chebyBkg.createIntegral(x,"rightSidebandRange")->getVal();

      // cout << "Total Events (integrated) = " << TotalEvents << endl;
      cout << "bkgSignalRangeArea = " << bkgSignalRangeArea << endl;
      cout << "LSBArea = " << LSBArea << endl;
      cout << "RSBArea = " << RSBArea << endl;

      Double_t weightSideband = -1.0*bkgSignalRangeArea/(LSBArea+RSBArea);

      cout << "weightSideband = " << weightSideband << endl;
      cout << "bkgSignal + sideband = " << bkgSignalRangeArea + (weightSideband*LSBArea) + (weightSideband*RSBArea) << endl;

      TH1F *hg1g2mass_sbsAll = new TH1F("h_g1g2mass_sbsAll","#gamma #gamma invariant mass spectrum",101,0,0.2);
      TH1F *hg1g2mass_sbsSignalOnly = new TH1F("h_g1g2mass_sbsSignalOnly","#gamma #gamma invariant mass spectrum",101,0,0.2);
      TH1F *hg1g2mass_sbsSidebandOnlyUnweighted = new TH1F("h_g1g2mass_sbsSidebandOnlyUnweighted","#gamma #gamma invariant mass spectrum",101,0,0.2);
      TH1F *hg1g2mass_sbsSidebandOnlyWeighted = new TH1F("h_g1g2mass_sbsSidebandOnlyWeighted","#gamma #gamma invariant mass spectrum",101,0,0.2);
      TH1F *hpippimg1g2mass_sbsAll = new TH1F("h_pippimg1g2mass_sbsAll","#pi^{+} #pi^{-} #gamma #gamma invariant mass spectrum sidaband substracted",101,0.45,0.65);
      TH1F *hpippimg1g2mass_sbsSidebandOnly = new TH1F("h_pippimg1g2mass_sbsSidebandOnly","#pi^{+} #pi^{-} #gamma #gamma invariant mass spectrum sidaband substracted (sideband only)",101,0.45,0.65);
      TH1F *hpippimg1g2mass_sbsSignalOnly = new TH1F("h_pippimg1g2mass_sbsSignalOnly","#pi^{+} #pi^{-} #gamma #gamma invariant mass spectrum sidaband substracted (signal only)",101,0.45,0.65);
      TH2F *h2DalitzPlotEta3Pi_sbsAll = new TH2F("h2_DalitzPlotEta3Pi_sbsAll","",101,-1.0,1.0,101,-1.0,1.0);
      TH2F *h2DalitzPlotEta3Pi_sbsSignalOnly = new TH2F("h2_DalitzPlotEta3Pi_sbsSignalOnly","",101,-1.0,1.0,101,-1.0,1.0);
      TH2F *h2DalitzPlotEta3Pi_sbsSidebandOnlyUnweighted = new TH2F("h2_DalitzPlotEta3Pi_sbsSidebandOnlyUnweighted","",101,-1.0,1.0,101,-1.0,1.0);
      // TH2F *h2g1g2massdiffbkgsubs = new TH2F("h2_g2g2MassDiffvsInvMassBkgSubs","Two-photon energy difference vs invariant mass (GeV)",1000,0.,0.2,1000,-0.0000001,0.0000001);	
      // TH2F *h2anglePi0TwoGammasbkgsubs = new TH2F("h2_anglePi0TwoGammasBkgSubs","Two-photon angle vs invariant mass",100,0.07,0.2,1000,-2*TMath::Pi(),2*TMath::Pi());

      cout << "Generating background substracted tree..." << endl;
      cout << "Output: " << outName << endl;
      for(Int_t i=0;i<nEntries;i++){
          dataChain->GetEntry(i);

          //Apply cuts:
          //is_truecombo is for MC data (i.e. simulated data) --> Just look at simulated eta->pi+pi-pi0 and nothing else...

          //One thing to test: (g1_p4_kin + g2_p4_kin) == pi0 --> (g1_p4_kin + g2_p4_kin).M() (should be 0.135 = m(pi0)) --> 

          //Plot m_pi0 or perform a cut: m_pi0 >= 0.12 && m_pi0 < 0.15

          if(is_truecombo && kfit_prob > kfit_cut){
              EnP1 = pip_p4_kin->E();
              PxP1 = pip_p4_kin->Px();
              PyP1 = pip_p4_kin->Py();
              PzP1 = pip_p4_kin->Pz();

              EnP2 = pim_p4_kin->E();
              PxP2 = pim_p4_kin->Px();
              PyP2 = pim_p4_kin->Py();
              PzP2 = pim_p4_kin->Pz();

              EnP3 = (*g1_p4_kin + *g2_p4_kin).E();
              PxP3 = (*g1_p4_kin + *g2_p4_kin).Px();
              PyP3 = (*g1_p4_kin + *g2_p4_kin).Py();
              PzP3 = (*g1_p4_kin + *g2_p4_kin).Pz();

              Double_t m_g1g2 = (*g1_p4_kin + *g2_p4_kin).M();
              // Double_t diffmassg1g2 = (*g1_p4_kin).M() - (*g2_p4_kin).M();

              // Double_t anglePi0TwoGammas = (*g1_p4_kin).Angle((*g2_p4_kin).Vect());
              Double_t m_pippimg1g2mass = (*g1_p4_kin + *g2_p4_kin + *pip_p4_kin + *pim_p4_kin).M();
              // cout << diffmassg1g2 << endl;

              if(dBRT >= -2.0 && dBRT <= 2.0){
                  weight = 1.0;
              }else weight = -1.0/8.0;

              // x.setVal(m_pippimg1g2mass);
              // Double_t sgn_m_pippimg1g2mass = (1-cBkg1.getVal())*gauss1.getVal();
              // Double_t total_m_pippimg1g2mass = (1-cBkg1.getVal())*gauss1.getVal() + cBkg1.getVal()*chebyBkg.getVal();
              // Double_t w2 = sgn_m_pippimg1g2mass/total_m_pippimg1g2mass;
              Double_t theta_photon1 = (*g1_p4_kin).Theta();
              Double_t theta_photon2 = (*g2_p4_kin).Theta();

              Bool_t Pi0MassCut = kFALSE;
              if (enablePi0MassCut) Pi0MassCut = (m_g1g2 > Pi0MassRange[0]) && (m_g1g2 < Pi0MassRange[1]);
              else Pi0MassCut = kTRUE; //always true

              Bool_t PhotonThetaCut = kFALSE;
              if (enablePhotonsThetaCut) PhotonThetaCut = (theta_photon1*TMath::RadToDeg() < 10.3 || theta_photon1*TMath::RadToDeg() > 11.9) && (theta_photon2*TMath::RadToDeg() < 10.3 || theta_photon2*TMath::RadToDeg() > 11.9);
              else PhotonThetaCut = kTRUE; //always true

                  
              //calculate X and Y
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

              if (Pi0MassCut && PhotonThetaCut) {
                  if ((m_pippimg1g2mass>=leftSidebandRange[0]) && (m_pippimg1g2mass<=leftSidebandRange[1])){
                    hg1g2mass_sbsSidebandOnlyUnweighted->Fill(m_g1g2,weight);
                    h2DalitzPlotEta3Pi_sbsSidebandOnlyUnweighted->Fill(X_c,Y_c,weight);
                    weight *= weightSideband;
                    hpippimg1g2mass_sbsSidebandOnly->Fill(m_pippimg1g2mass,weight);
                    hpippimg1g2mass_sbsAll->Fill(m_pippimg1g2mass,weight);
                    hg1g2mass_sbsAll->Fill(m_g1g2,weight);
                    h2DalitzPlotEta3Pi_sbsAll->Fill(X_c,Y_c,weight);
                    hg1g2mass_sbsSidebandOnlyWeighted->Fill(m_g1g2,weight);
                    if (!signalOnlyTree) out_tree->Fill();
                  }
                  if((m_pippimg1g2mass>=rightSidebandRange[0]) && (m_pippimg1g2mass<=rightSidebandRange[1])){
                    hg1g2mass_sbsSidebandOnlyUnweighted->Fill(m_g1g2,weight);
                    h2DalitzPlotEta3Pi_sbsSidebandOnlyUnweighted->Fill(X_c,Y_c,weight);
                    weight *= weightSideband;
                    hpippimg1g2mass_sbsSidebandOnly->Fill(m_pippimg1g2mass,weight);
                    hpippimg1g2mass_sbsAll->Fill(m_pippimg1g2mass,weight);
                    hg1g2mass_sbsAll->Fill(m_g1g2,weight);
                    h2DalitzPlotEta3Pi_sbsAll->Fill(X_c,Y_c,weight);
                    hg1g2mass_sbsSidebandOnlyWeighted->Fill(m_g1g2,weight);
                    if (!signalOnlyTree) out_tree->Fill();
                  }
                  if((m_pippimg1g2mass>=signalRange[0]) && (m_pippimg1g2mass<=signalRange[1])) {
                    hpippimg1g2mass_sbsSignalOnly->Fill(m_pippimg1g2mass,weight);
                    hpippimg1g2mass_sbsAll->Fill(m_pippimg1g2mass,weight);
                    hg1g2mass_sbsAll->Fill(m_g1g2,weight);
                    h2DalitzPlotEta3Pi_sbsAll->Fill(X_c,Y_c,weight);
                    hg1g2mass_sbsSignalOnly->Fill(m_g1g2,weight);
                    h2DalitzPlotEta3Pi_sbsSignalOnly->Fill(X_c,Y_c,weight);
                    out_tree->Fill();
                  }
              }

                // hg1g2massbkgsubs->Fill(m_pi0,weight);
                // h2g1g2massdiffbkgsubs->Fill(m_pi0,diffmassg1g2,weight);
                // h2anglePi0TwoGammasbkgsubs->Fill(m_pi0,anglePi0TwoGammas,weight);
                // hpippimg1g2massbkgsubs->Fill(m_pippimg1g2mass,weight);
                // out_tree->Fill();
                // h2anglePi0TwoGammas->Fill(m_pi0,anglePi0TwoGammas,weight);
              

              //This is just testing....
              //---------------------------
              // Double_t amp_raw = 1.0 + test_pars[0]*Y + test_pars[1]*Y*Y + test_pars[2]*X + test_pars[3]*X*X + test_pars[4]*X*Y + test_pars[5]*Y*Y*Y + test_pars[6]*X*X*Y + test_pars[7]*X*Y*Y + test_pars[8]*X*X*X;
              // Double_t amp_raw_2 = 1.0 + test_pars_2[0]*Y + test_pars_2[1]*Y*Y + test_pars_2[2]*X + test_pars_2[3]*X*X + test_pars_2[4]*X*Y + test_pars_2[5]*Y*Y*Y + test_pars_2[6]*X*X*Y + test_pars_2[7]*X*Y*Y + test_pars_2[8]*X*X*X;

              // amp = TMath::Sqrt(amp_raw);
              // amp_2 = TMath::Sqrt(1.037*amp_raw_2);

              // sum_amp += (amp);
              // sum_amp_2 += (amp_2);
              //--------------------------- 
          }
      }
     
      setHG1G2Axis(hg1g2mass_sbsAll);
      hg1g2mass_sbsAll->Write();
      setHG1G2Axis(hg1g2mass_sbsSignalOnly);
      hg1g2mass_sbsSignalOnly->Write();
      setHG1G2Axis(hg1g2mass_sbsSidebandOnlyUnweighted);
      hg1g2mass_sbsSidebandOnlyUnweighted->Write();
      setHG1G2Axis(hg1g2mass_sbsSidebandOnlyWeighted);
      hg1g2mass_sbsSidebandOnlyWeighted->Write();
      setHPipPimG1G2Axis(hpippimg1g2mass_sbsAll);
      hpippimg1g2mass_sbsAll->Write();
      setHPipPimG1G2Axis(hpippimg1g2mass_sbsSidebandOnly);
      hpippimg1g2mass_sbsSidebandOnly->Write();
      setHPipPimG1G2Axis(hpippimg1g2mass_sbsSignalOnly);
      hpippimg1g2mass_sbsSignalOnly->Write();
      h2DalitzPlotEta3Pi_sbsAll->Write();
      h2DalitzPlotEta3Pi_sbsSignalOnly->Write();
      h2DalitzPlotEta3Pi_sbsSidebandOnlyUnweighted->Write();
      out_tree->Write();
    }
  }
  if (!fitOnly) outfile->Write();
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