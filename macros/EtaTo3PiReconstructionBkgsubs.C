using namespace RooFit;

const Double_t signalRange[2] = {min,max};
const Double_t leftSidebandRange[2] = {};
const Double_t rightSidebandRange[2] = {};

void EtaTo3PiReconstruction(int data_set,TString outName,bool is_mc){

  TString dataDir = "";
  TString dataName = "";

  //Directories where the different data sets are stored:
  if(!is_mc){

     if(data_set == 0){
        dataDir = "/d/grid17/dlersch/AmpTool_Data/data2017";
        dataName = "PiPiGG_Tree_2017_data";
     }else if(data_set == 1){
       dataDir = "/d/grid17/dlersch/AmpTool_Data/data2018S";
       dataName = "PiPiGG_Tree_2018S_data";
     }else if(data_set == 2){
       dataDir = "/d/grid17/dlersch/AmpTool_Data/data2018F";
       dataName = "PiPiGG_Tree_2018F_data";
    }
  
  }else{
    if(data_set == 0){
        dataDir = "/d/grid16/dlersch/eta3pi_mc_data_2017/geant4/ds_trees";
        dataName = "PiPiGG_Tree_runSim_Geant4_MC2017_all";
    }else if(data_set == 1){
      dataDir = "/d/grid16/dlersch/eta3pi_mc_data_2018S/geant4/ds_trees";
      dataName = "PiPiGG_Tree_runSim_Geant4_MC2018S_all";
    }else if(data_set == 2){
      dataDir = "/d/grid16/dlersch/eta3pi_mc_data_2018F/geant4/ds_trees";
      dataName = "PiPiGG_Tree_runSim_Geant4_MC2018F_all";
    }
  };

  //kfit_cut1 = 0.01;
  //kfit_cut2 = 0.1;

    Double_t kfit_cut = 0.01; //This values represents a cut on the kinematic fit probability!


    //Input tree with data we want to analyze and transformt to the AmpTool data:
    TString treeName = "myTree";
    
    TFile *in = TFile::Open(dataDir + "/" + dataName + ".root");
    TTree *in_tree = (TTree*)in->Get(treeName);
    
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
    
    in_tree->SetBranchStatus("*",0);
    in_tree->SetBranchStatus("kfit_prob",1);
    in_tree->SetBranchStatus("dBRT",1);
    in_tree->SetBranchStatus("pim_p4_kin",1);
    in_tree->SetBranchStatus("pip_p4_kin",1);
    in_tree->SetBranchStatus("g1_p4_kin",1);
    in_tree->SetBranchStatus("g2_p4_kin",1);
    
    in_tree->SetBranchAddress("kfit_prob",&kfit_prob);
    in_tree->SetBranchAddress("dBRT",&dBRT);
    in_tree->SetBranchAddress("pip_p4_kin",&pip_p4_kin); //4-vector of pi+
    in_tree->SetBranchAddress("pim_p4_kin",&pim_p4_kin); //4-vector of pi- 
    in_tree->SetBranchAddress("g1_p4_kin",&g1_p4_kin); //4-vector of photon1 (gamma 1)
    in_tree->SetBranchAddress("g2_p4_kin",&g2_p4_kin); //4-vector of photon2 (gamma 2)

    if(is_mc){
        in_tree->SetBranchStatus("is_truecombo",1);
        in_tree->SetBranchAddress("is_truecombo",&is_truecombo);
    }else{
        is_truecombo = true;
    }
    
    //Define the output tree:
    
    Int_t nEntries = in_tree->GetEntries();
    /*
    Double_t EnP1,PxP1,PyP1,PzP1;
    Double_t EnP2,PxP2,PyP2,PzP2;
    Double_t EnP3,PxP3,PyP3,PzP3;
    Double_t X,Y,weight;
    */

    // Double_t test_pars[9] = {-1.095,0.145,0.0,0.081,0.0,0.141,-0.044,0.0,0.0};
    // Double_t test_pars_2[9] = {-0.532,-0.044,0.01,0.050,-0.002,0.058,-0.0033,-0.025,-0.0084};
    
    TFile *outfile = new TFile(outName + ".root","RECREATE");
    
    // Double_t sum_amp = 0.0;
    // Double_t sum_amp_2 = 0.0;
    
    // Double_t amp = 0.0;
    // Double_t amp_2 = 0.0;
    
    //Output data for AmpTools -> Please do not change
    /*
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
    out_tree->Branch("PzP3",&PzP3,"PzP3/D");
    
    out_tree->Branch("weight",&weight,"weight/D");
    */
    
    Double_t weight = 1.0;
    
    // Double_t test_norm = TMath::Sqrt(TMath::Power(nEntries,-1));

    Double_t mass_Pi0 = 0.1349768;

    TH1F *hg1g2mass = new TH1F("h_g1g2Mass","Di-photon invariant mass spectra (GeV)",1000,0,0.2);
    TH2F *h2g1g2massdiff = new TH2F("h2_g2g2MassDiffvsInvMass","Di-photon mass difference vs invariant mass (GeV)",1000,0.,0.2,1000,-0.0000001,0.0000001);	
    TH2F *h2anglePi0TwoGammas = new TH2F("h2_anglePi0TwoGammas","Di-photon angle vs invariant mass",100,0.07,0.2,1000,-2*TMath::Pi(),2*TMath::Pi());
    //++++++++++++++++++++++++++++++++++++++
    for(Int_t i=0;i<nEntries;i++){
        in_tree->GetEntry(i);
        
        //Apply cuts:
        //is_truecombo is for MC data (i.e. simulated data) --> Just look at simulated eta->pi+pi-pi0 and nothing else...
  
        //One thing to test: (g1_p4_kin + g2_p4_kin) == pi0 --> (g1_p4_kin + g2_p4_kin).M() (should be 0.135 = m(pi0)) --> 
        
        //Plot m_pi0 or perform a cut: m_pi0 >= 0.12 && m_pi0 < 0.15

        if(is_truecombo && kfit_prob > kfit_cut){
            // EnP1 = pip_p4_kin->E();
            // PxP1 = pip_p4_kin->Px();
            // PyP1 = pip_p4_kin->Py();
            // PzP1 = pip_p4_kin->Pz();
            
            // EnP2 = pim_p4_kin->E();
            // PxP2 = pim_p4_kin->Px();
            // PyP2 = pim_p4_kin->Py();
            // PzP2 = pim_p4_kin->Pz();
            
            // EnP3 = (*g1_p4_kin + *g2_p4_kin).E();
            // PxP3 = (*g1_p4_kin + *g2_p4_kin).Px();
            // PyP3 = (*g1_p4_kin + *g2_p4_kin).Py();
            // PzP3 = (*g1_p4_kin + *g2_p4_kin).Pz();

            Double_t m_pi0 = (*g1_p4_kin + *g2_p4_kin).M();
            Double_t diffmassg1g2 = (*g1_p4_kin).M() - (*g2_p4_kin).M();

            Double_t anglePi0TwoGammas = (*g1_p4_kin).Angle((*g2_p4_kin).Vect());
            // cout << diffmassg1g2 << endl;

            if(dBRT >= -2.0 && dBRT <= 2.0){
                weight = 1.0;
            }else weight = -1.0/8.0;

            hg1g2mass->Fill(m_pi0,weight);
            h2g1g2massdiff->Fill(m_pi0,diffmassg1g2,weight);
            h2anglePi0TwoGammas->Fill(m_pi0,anglePi0TwoGammas,weight);           
            
            //This is just testing....
            //---------------------------
            // Double_t amp_raw = 1.0 + test_pars[0]*Y + test_pars[1]*Y*Y + test_pars[2]*X + test_pars[3]*X*X + test_pars[4]*X*Y + test_pars[5]*Y*Y*Y + test_pars[6]*X*X*Y + test_pars[7]*X*Y*Y + test_pars[8]*X*X*X;
            // Double_t amp_raw_2 = 1.0 + test_pars_2[0]*Y + test_pars_2[1]*Y*Y + test_pars_2[2]*X + test_pars_2[3]*X*X + test_pars_2[4]*X*Y + test_pars_2[5]*Y*Y*Y + test_pars_2[6]*X*X*Y + test_pars_2[7]*X*Y*Y + test_pars_2[8]*X*X*X;
            
            // amp = TMath::Sqrt(amp_raw);
            // amp_2 = TMath::Sqrt(1.037*amp_raw_2);
            
            // sum_amp += (amp);
            // sum_amp_2 += (amp_2);
            //--------------------------- 
            // if (m_pi0 > 0.11 && m_pi0 < 0.165) out_tree->Fill();
        }
    }
    //++++++++++++++++++++++++++++++++++++++
    
    // cout <<"sum: "<<sum_amp<< endl;
    // cout <<"sum2: "<<sum_amp_2<< endl;

    hg1g2mass->Write();
    h2g1g2massdiff->Write();

    //generate background substracted tree
    RooRealVar x("gamma gamma invariant mass","gamma gamma invariant mass (GeV)",0.,0.2);
    RooDataHist data("data","data",RooArgList(x),hg1g2mass);

    TCanvas *canvas = new TCanvas("c","c",800,500);
    RooPlot *plotMass = x.frame();
    plotMass->SetTitle(hg1g2mass->GetTitle());
    plotMass->SetXTitle(hg1g2mass->GetXaxis()->GetTitle());
    plotMass->SetTitleOffset(1.1,"Y");
    
    Double_t mass_bkg1 = 0.08;
    Double_t mass_bkg2 = 0.1;
    RooRealVar meanSgn("meanSgn","meanSgn",mass_Pi0,mass_Pi0-0.02,mass_Pi0+0.02);
    RooRealVar sigmaSgn("sigmaSgn","sigmaSgn",0.005,0.001,0.006);
    RooGaussian gauss1("gauss1","gauss1",x,meanSgn,sigmaSgn);
    RooRealVar meanBkg1("meanBkg1","meanBkg1",mass_bkg1,0.07,0.09);
    RooRealVar sigmaBkg1("sigmaBkg1","sigmaBkg1",0.005,0.001,0.02);
    RooGaussian gauss2("gauss2","gauss2",x,meanBkg1,sigmaBkg1);
    RooRealVar cBkg1("cBkg1","cBkg1",0.002,0.001,0.2);
    RooRealVar meanBkg2("meanBkg2","meanBkg2",mass_bkg2,0.095,0.13);
    RooRealVar sigmaBkg2("sigmaBkg2","sigmaBkg2",0.005,0.001,0.03);
    RooGaussian gauss3("gauss3","gauss3",x,meanBkg2,sigmaBkg2);
    RooRealVar cBkg2("cBkg2","cBkg2",0.002,0.001,0.3);
    RooAddPdf fitFunction("tripleGauss","tripleGauss",RooArgList(gauss2,gauss3,gauss1),RooArgList(cBkg1,cBkg2));
    fitFunction.fitTo(data,RooFit::PrintLevel(-1),RooFit::Minos(1));
    data.plotOn(plotMass,MarkerSize(0.5));
    fitFunction.plotOn(plotMass,LineColor(kRed),LineWidth(2));
    fitFunction.plotOn(plotMass,Components("gauss1"),LineColor(kBlue),LineStyle(kDashed),LineWidth(1));
    fitFunction.plotOn(plotMass,Components("gauss2"),LineColor(kYellow),LineStyle(kDashed),LineWidth(1));
    fitFunction.plotOn(plotMass,Components("gauss3"),LineColor(kGreen),LineStyle(kDashed),LineWidth(1));
    fitFunction.paramOn(plotMass,Layout(0.1,0.4,0.9),Format("NU",AutoPrecision(0)));
    plotMass->Draw();
    
    if (!is_mc) {
      TString outMassFitPdfName = outName;
      outMassFitPdfName += "pi0toggMassFit";
      outMassFitPdfName += ".pdf";
      canvas->Print(outMassFitPdfName);
    }
    // TF1 *gaus1F = (TF1*)plotMass->FindObject("gauss1");
    // Double_t i = 0.12;
    // while(i<0.15){
    //     x.setVal(i);
    //     Double_t w2 = (1-cBkg1.getVal()-cBkg2.getVal())*gauss1.getVal() + cBkg1.getVal()*gauss2.getVal() + cBkg2.getVal()*gauss3.getVal();
    //     cout << i << " " << w2 << endl;
    //     i+=0.001;
    // }
    
    Double_t EnP1,PxP1,PyP1,PzP1;
    Double_t EnP2,PxP2,PyP2,PzP2;
    Double_t EnP3,PxP3,PyP3,PzP3;
    Double_t X,Y;

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
    out_tree->Branch("PzP3",&PzP3,"PzP3/D");
    
    out_tree->Branch("weight",&weight,"weight/D");
    
    weight = 1.0;

    TH1F *hg1g2massbkgsubs = new TH1F("h_g1g2MassBkgSubs","Di-photon invariant mass spectra (GeV)",1000,0,0.2);
    TH2F *h2g1g2massdiffbkgsubs = new TH2F("h2_g2g2MassDiffvsInvMassBkgSubs","Di-photon mass difference vs invariant mass (GeV)",1000,0.,0.2,1000,-0.0000001,0.0000001);	
    TH2F *h2anglePi0TwoGammasbkgsubs = new TH2F("h2_anglePi0TwoGammasBkgSubs","Di-photon angle vs invariant mass",100,0.07,0.2,1000,-2*TMath::Pi(),2*TMath::Pi());

    for(Int_t i=0;i<nEntries;i++){
        in_tree->GetEntry(i);
        
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

            Double_t m_pi0 = (*g1_p4_kin + *g2_p4_kin).M();
            Double_t diffmassg1g2 = (*g1_p4_kin).M() - (*g2_p4_kin).M();

            Double_t anglePi0TwoGammas = (*g1_p4_kin).Angle((*g2_p4_kin).Vect());
            // cout << diffmassg1g2 << endl;

            if(dBRT >= -2.0 && dBRT <= 2.0){
                weight = 1.0;
            }else weight = -1.0/8.0;

            x.setVal(m_pi0);
            Double_t sgn_m_pi0 = (1-cBkg1.getVal()-cBkg2.getVal())*gauss1.getVal();
            Double_t total_m_pi0 = (1-cBkg1.getVal()-cBkg2.getVal())*gauss1.getVal() + cBkg1.getVal()*gauss2.getVal() + cBkg2.getVal()*gauss3.getVal();
            Double_t w2 = sgn_m_pi0/total_m_pi0;

            if (!is_mc) {
                weight *= w2
            };

            hg1g2massbkgsubs->Fill(m_pi0,weight);
            h2g1g2massdiffbkgsubs->Fill(m_pi0,diffmassg1g2,weight);
            h2anglePi0TwoGammasbkgsubs->Fill(m_pi0,anglePi0TwoGammas,weight);
            
            if (m_pi0 > 0.115 && m_pi0 < 0.16) out_tree->Fill();


            
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
    hg1g2massbkgsubs->Write();
    h2g1g2massdiffbkgsubs->Write();
    h2anglePi0TwoGammasbkgsubs->Write();
    out_tree->Write();
    
    // out_tree->Write();
    outfile->Write();
}