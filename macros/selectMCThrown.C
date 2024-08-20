// const Double_t PhotonBeamEnergyBin[6] = {6.5,7.5,8.0,9.0,10.0,11.6};
// const Double_t PhotonBeamEnergyBin[2] = {6.5,11.6};
const Double_t MandelstamtTRange[2] = {0.15,0.6};
// const Int_t NEnergyBin=3;
const Double_t PhotonBeamEnergyBin[4] = {3.0,4.0,5.0,6.5};
// const Double_t MandelstamtTRange[2] = {0.1,0.6};
// TString fileInputName = "/d/home/septian/Eta3PiDalitz/run/root4Amptools/Eta3Pi_Thrown_2017_genEtaRegge_flat.root";
// TString fileInputName = "/d/grid17/septian/Eta3PiDalitz/DSelectors/Eta3Pi_Thrown_2017_genEtaRegge_flat.root";
// TString tag = "t0106_ccdbFlux_oldMC";

// const Double_t PhotonBeamEnergyBin[NEnergyBin+1] = {3.0,11.6};
TString outDir = "/d/home/septian/Eta3PiDalitzPlots/";

void selectMCThrown(Int_t data_set, TString tag, Int_t PhotonBeamEnergyIdx){ 
    gStyle->SetOptStat(0);
    TString runPeriod[3] = {"2017","2018S","2018F"};
    TString workingDir = Form("/d/home/septian/Eta3PiDalitz/run/Eta3PiSelectorParallelTree_t01506_ccdbFlux_genEtaRegge/");
    TString DSelectorDir = "/d/home/septian/Eta3PiDalitz/DSelectors/";
    // TString fileInputName[3] = {Form("%sEta3Pi_Thrown_2017_ccdbFlux_genEtaRegge_flat.root",DSelectorDir.Data()),
    //                             Form("%sTree_Eta3Pi_Thrown_2018S_ccdbFlux_genEtaRegge_flat.root",DSelectorDir.Data()),
    //                             ""}; 
    TString fileInputName[3] = {Form("%sEta3Pi_Thrown_2017_cobremsFlux_genEtaRegge_flat.root",DSelectorDir.Data()),
                                "",
                                ""}; 
    TString fileOutputBaseName = Form("Eta3PiSelectorParallelTree_%s/mc_thrown_%s_%s_%d",tag.Data(),runPeriod[data_set].Data(),tag.Data(),PhotonBeamEnergyIdx);
    
    TChain *dataChain = new TChain("nt");
    dataChain->Add(fileInputName[data_set].Data());
    Int_t nEntries = dataChain->GetEntries();
    cout << "Filename: " << fileInputName[data_set] << endl; 
    cout << "Number of entries: " << nEntries << endl;

    Double_t EnP1,PxP1,PyP1,PzP1;
    Double_t EnP2,PxP2,PyP2,PzP2;
    Double_t EnP3,PxP3,PyP3,PzP3;
    Float_t weight;

    Double_t EnBeam,t;

    dataChain->SetBranchAddress("EnP1",&EnP1);
    dataChain->SetBranchAddress("PxP1",&PxP1);
    dataChain->SetBranchAddress("PyP1",&PyP1);
    dataChain->SetBranchAddress("PzP1",&PzP1);

    dataChain->SetBranchAddress("EnP2",&EnP2);
    dataChain->SetBranchAddress("PxP2",&PxP2);
    dataChain->SetBranchAddress("PyP2",&PyP2);
    dataChain->SetBranchAddress("PzP2",&PzP2);

    dataChain->SetBranchAddress("EnP3",&EnP3);
    dataChain->SetBranchAddress("PxP3",&PxP3);
    dataChain->SetBranchAddress("PyP3",&PyP3);
    dataChain->SetBranchAddress("PzP3",&PzP3);

    dataChain->SetBranchAddress("weight",&weight);

    dataChain->SetBranchAddress("t",&t);
    dataChain->SetBranchAddress("EnBeam",&EnBeam);

    TCanvas *c = new TCanvas("c","c",800,600);
    TH1F *h1 = new TH1F("h1","h1",50,MandelstamtTRange[0],MandelstamtTRange[1]);
    TH1F *h1_pippimg1g2mass = new TH1F("h1_pippimg1g2mass","h1_pippimg1g2mass",100,0.45,0.65);
    TH2F *h2_DalitzPlot = new TH2F("h2_DalitzPlotEta3Pi_thrown","h2_DalitzPlotEta3Pi_thrown",101,-1.0,1.0,101,-1.0,1.0);
    
    Double_t EnBeamMin = PhotonBeamEnergyBin[PhotonBeamEnergyIdx];
    Double_t EnBeamMax = PhotonBeamEnergyBin[PhotonBeamEnergyIdx+1];
    cout << "EnBeamMin: " << EnBeamMin << endl;
    cout << "EnBeamMax: " << EnBeamMax << endl;

    TTree *treeOutput = new TTree("nt","nt");
    
    //create branches
    treeOutput->Branch("EnP1",&EnP1,"EnP1/D");
    treeOutput->Branch("PxP1",&PxP1,"PxP1/D");
    treeOutput->Branch("PyP1",&PyP1,"PyP1/D");
    treeOutput->Branch("PzP1",&PzP1,"PzP1/D");
    treeOutput->Branch("EnP2",&EnP2,"EnP2/D");
    treeOutput->Branch("PxP2",&PxP2,"PxP2/D");
    treeOutput->Branch("PyP2",&PyP2,"PyP2/D");
    treeOutput->Branch("PzP2",&PzP2,"PzP2/D");
    treeOutput->Branch("EnP3",&EnP3,"EnP3/D");
    treeOutput->Branch("PxP3",&PxP3,"PxP3/D");
    treeOutput->Branch("PyP3",&PyP3,"PyP3/D");
    treeOutput->Branch("PzP3",&PzP3,"PzP3/D");
    treeOutput->Branch("EnBeam",&EnBeam,"EnBeam/D");
    Double_t Weight = 1;
    treeOutput->Branch("weight",&Weight,"weight/D");            
    //loop over entries
    for(Int_t i=0;i<nEntries;i++){
        dataChain->GetEntry(i);
        if(EnBeam<EnBeamMin || EnBeam>EnBeamMax) continue;
        if(TMath::Abs(t)<MandelstamtTRange[0] || TMath::Abs(t)>MandelstamtTRange[1]) continue;
        // calculate X and Y
        TLorentzVector P4_PiPlus,P4_PiMinus,P4_PiZero;
        P4_PiPlus.SetPxPyPzE(PxP1,PyP1,PzP1,EnP1);
        P4_PiMinus.SetPxPyPzE(PxP2,PyP2,PzP2,EnP2);
        P4_PiZero.SetPxPyPzE(PxP3,PyP3,PzP3,EnP3);
        Double_t etaMass = (P4_PiPlus + P4_PiMinus + P4_PiZero).M();
        TLorentzVector P4_Eta = P4_PiPlus + P4_PiMinus + P4_PiZero;
    
        P4_PiPlus.Boost(-P4_Eta.BoostVector());
        P4_PiMinus.Boost(-P4_Eta.BoostVector());
        P4_PiZero.Boost(-P4_Eta.BoostVector());
        Double_t T_PiPlus = P4_PiPlus.E() - P4_PiPlus.M();
        Double_t T_PiMinus = P4_PiMinus.E() - P4_PiMinus.M();
        Double_t T_PiZero = P4_PiZero.E() - P4_PiZero.M();
        Double_t Dalitz_X = TMath::Sqrt(3.0)*(T_PiPlus - T_PiMinus)/(T_PiPlus + T_PiMinus + T_PiZero);
        Double_t Dalitz_Y = 3.0*T_PiZero/(T_PiPlus + T_PiMinus + T_PiZero) - 1.0;
        // fill output tree
        treeOutput->Fill();
        // fill histogram
        h1->Fill(TMath::Abs(t),weight);
        h1_pippimg1g2mass->Fill(etaMass,weight);
        h2_DalitzPlot->Fill(Dalitz_X,Dalitz_Y,Weight);
    }
    //write output file
    TString fileOutputName = Form("%s.root",fileOutputBaseName.Data());
    cout << "Output file: " << fileOutputName << endl;
    //create output file
    TFile *fileOutput = new TFile(fileOutputName.Data(),"RECREATE");
    treeOutput->Write();
    treeOutput->Delete();
    h1->GetXaxis()->SetTitle("|t| (GeV^{2})");
    h1->GetYaxis()->SetTitle("Events");
    h1->SetTitle(Form("Mandelstam-t distribution for %0.2f < E_{#gamma} < %0.2f GeV",EnBeamMin,EnBeamMax));
    // fit h1 with exponential model
    TF1 *f1 = new TF1("f1","[0]*exp([1]*x)",MandelstamtTRange[0],MandelstamtTRange[1]);
    h1->Fit("f1","R");
    // show fit parameters on the plot
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    h1->Draw("E");
    Tl.DrawLatexNDC(0.15,0.30,Form("#chi^{2}/ndf = %.2f",f1->GetChisquare()/f1->GetNDF()));
    Tl.DrawLatexNDC(0.15,0.25,Form("p0 = %.2f #pm %.2f",f1->GetParameter(0),f1->GetParError(0)));
    Tl.DrawLatexNDC(0.15,0.20,Form("p1 = %.2f #pm %.2f",f1->GetParameter(1),f1->GetParError(1)));
    c->SaveAs(Form("%sh_mandelstam_t_%s_%0.2f_%0.2f.pdf",outDir.Data(),tag.Data(),EnBeamMin,EnBeamMax));
    h1->Write();
    h2_DalitzPlot->GetXaxis()->SetTitle("Dalitz X");
    h2_DalitzPlot->GetYaxis()->SetTitle("Dalitz Y");
    h2_DalitzPlot->Draw("COLZ");
    c->SaveAs(Form("%sh_DalitzPlotEta3Pi_thrown_%s_%0.2f_%0.2f.pdf",outDir.Data(),tag.Data(),EnBeamMin,EnBeamMax));
    h2_DalitzPlot->Write();
    h1_pippimg1g2mass->GetXaxis()->SetTitle("M_{#pi^{+}#pi^{-}#pi^{0}} (GeV)");
    h1_pippimg1g2mass->GetYaxis()->SetTitle("Events");
    h1_pippimg1g2mass->SetTitle(Form("Thrown M_{#pi^{+}#pi^{-}#pi^{0}} distribution for %0.2f < E_{#gamma} < %0.2f GeV",EnBeamMin,EnBeamMax));
    h1_pippimg1g2mass->Draw("hist");
    c->SaveAs(Form("%sh_pippimg1g2mass_%s_%0.2f_%0.2f.pdf",outDir.Data(),tag.Data(),EnBeamMin,EnBeamMax));
    fileOutput->Close();
}