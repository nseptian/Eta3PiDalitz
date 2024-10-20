

Double_t IntensityFunction(Double_t X, Double_t Y);

Int_t ReweightPhSpMCDP(const TString inFileName="/d/home/septian/Eta3PiDalitz/run/Eta3PiSelectorParallelTree_IO_test/mc_rec_2017_IO_test_None_0.root", 
                        const TString outFileName="/d/home/septian/Eta3PiDalitz/run/Eta3PiSelectorParallelTree_IO_test/DP_mc_rec_2017_IO_test_None_0.root"){
    const TString outDirPlot = "/d/home/septian/Eta3PiDalitzPlots/";
    TFile *inFile = TFile::Open(inFileName);
    TTree *inTree = (TTree*)inFile->Get("nt");
    Int_t nEntries = inTree->GetEntries();
    cout << "Filename: " << inFileName << endl;
    cout << "Number of entries: " << nEntries << endl;

    Double_t EnP1,PxP1,PyP1,PzP1;
    Double_t EnP2,PxP2,PyP2,PzP2;
    Double_t EnP3,PxP3,PyP3,PzP3;
    Double_t weight;

    inTree->SetBranchAddress("EnP1",&EnP1);
    inTree->SetBranchAddress("PxP1",&PxP1);
    inTree->SetBranchAddress("PyP1",&PyP1);
    inTree->SetBranchAddress("PzP1",&PzP1);

    inTree->SetBranchAddress("EnP2",&EnP2);
    inTree->SetBranchAddress("PxP2",&PxP2);
    inTree->SetBranchAddress("PyP2",&PyP2);
    inTree->SetBranchAddress("PzP2",&PzP2);

    inTree->SetBranchAddress("EnP3",&EnP3);
    inTree->SetBranchAddress("PxP3",&PxP3);
    inTree->SetBranchAddress("PyP3",&PyP3);
    inTree->SetBranchAddress("PzP3",&PzP3);

    inTree->SetBranchAddress("weight",&weight);

    TH2F *h2DalitzPlotAccPhSp = new TH2F("h2_DalitzPlotAccPhSp","",101,-1.0,1.0,101,-1.0,1.0);
    TH2F *h2DalitzPlotAccPhSpWeighted = new TH2F("h2_DalitzPlotAccPhSpWeighted","",101,-1.0,1.0,101,-1.0,1.0); 
    TH2F *h2DalitzPlotIntensity = new TH2F("h2_DalitzPlotIntensity","",101,-1.0,1.0,101,-1.0,1.0);

    // Sample a flat space and draw intensity dalitz plot
    // vector<Double_t> vX,vY,vIntensity;
    // for (Int_t iEvent=0; iEvent<100000; iEvent++) {
    //     Double_t X = -1.0 + 2.0*gRandom->Rndm();
    //     Double_t Y = -1.0 + 2.0*gRandom->Rndm();
    //     vX.push_back(X);
    //     vY.push_back(Y);
    //     vIntensity.push_back(IntensityFunction(X,Y));
    //     // h2DalitzPlotAccPhSp->Fill(X,Y);
    //     // h2DalitzPlotIntensity->Fill(X,Y,IntensityFunction(X,Y));
    // }
    
    // cout << "Intensity min: " << *min_element(vIntensity.begin(),vIntensity.end()) << endl;
    // cout << "Intensity max: " << *max_element(vIntensity.begin(),vIntensity.end()) << endl;

    // Double_t intensity_max = *max_element(vIntensity.begin(),vIntensity.end());
    // Double_t intensity_min = *min_element(vIntensity.begin(),vIntensity.end());

    TFile *outFile = new TFile(outFileName,"RECREATE");
    outFile->cd();

    // create a new tree with the same structure as the input tree
    TTree *outTree = inTree->CloneTree(0);

    // loop over entries and accept/reject based on intensity function
    for(Int_t i=0;i<nEntries;i++){
        inTree->GetEntry(i);

        TLorentzVector pip_p4_kin = TLorentzVector(PxP1,PyP1,PzP1,EnP1);
        TLorentzVector pim_p4_kin = TLorentzVector(PxP2,PyP2,PzP2,EnP2);
        TLorentzVector pi0_p4_kin = TLorentzVector(PxP3,PyP3,PzP3,EnP3);

        TLorentzVector eta = pip_p4_kin + pim_p4_kin + pi0_p4_kin;
        TVector3 eta_boost = eta.BoostVector();

        TLorentzVector boosted_p1 = pip_p4_kin;
        TLorentzVector boosted_p2 = pim_p4_kin;
        TLorentzVector boosted_p3 = pi0_p4_kin;
        
        boosted_p1.Boost(eta_boost*(-1));
        boosted_p2.Boost(eta_boost*(-1));
        boosted_p3.Boost(eta_boost*(-1));

        Double_t T_plus = boosted_p1.E() - boosted_p1.M();
        Double_t T_minus = boosted_p2.E() - boosted_p2.M();
        Double_t T_zero = boosted_p3.E() - boosted_p3.M();
        Double_t T_all = T_plus + T_minus + T_zero;
        Double_t X_c = TMath::Sqrt(3.0)*(T_plus - T_minus)/T_all;
        Double_t Y_c = 3.0*T_zero/T_all - 1.0;

        h2DalitzPlotAccPhSp->Fill(X_c,Y_c,weight);
        h2DalitzPlotIntensity->Fill(X_c,Y_c,weight*IntensityFunction(X_c,Y_c));
        
        if (gRandom->Rndm()*35.0 < IntensityFunction(X_c,Y_c)) {
            h2DalitzPlotAccPhSpWeighted->Fill(X_c,Y_c,weight);
            outTree->Fill();
        }
    }
    outTree->Write();
    outFile->Close();
    
    // gStyle->SetOptStat(0);
    TCanvas *c = new TCanvas("c","Dalitz plot",1200,1200);
    h2DalitzPlotAccPhSp->GetXaxis()->SetTitle("X");
    h2DalitzPlotAccPhSp->GetYaxis()->SetTitle("Y");
    h2DalitzPlotAccPhSp->Draw("colz");
    c->SaveAs(outDirPlot+"Dalitz_Plot_Acc_PhSp.pdf");
    h2DalitzPlotIntensity->GetXaxis()->SetTitle("X");
    h2DalitzPlotIntensity->GetYaxis()->SetTitle("Y");
    h2DalitzPlotIntensity->Draw("colz");
    c->SaveAs(outDirPlot+"Dalitz_Plot_Intensity.pdf");
    h2DalitzPlotAccPhSpWeighted->GetXaxis()->SetTitle("X");
    h2DalitzPlotAccPhSpWeighted->GetYaxis()->SetTitle("Y");
    h2DalitzPlotAccPhSpWeighted->Draw("colz");
    c->SaveAs(outDirPlot+"Dalitz_Plot_Acc_PhSp_Weighted.pdf");
    return 0;
}

Double_t IntensityFunction(Double_t X, Double_t Y){
    const Double_t pars[9] = {-1.095,0.145,0.0,0.081,0.0,0.141,-0.044,0.0,0.0};
    return (1.0 + pars[0]*Y + pars[1]*Y*Y + pars[2]*X + pars[3]*X*X + pars[4]*X*Y + pars[5]*Y*Y*Y + pars[6]*X*X*Y + pars[7]*X*Y*Y + pars[8]*X*X*X);
    // return TMath::Sqrt(1.0 + par_A*Y + par_B*Y*Y + par_C*X + par_D*X*X + par_E*X*Y + par_F*Y*Y*Y + par_G*X*X*Y + par_H*X*Y*Y + par_L*X*X*X)
}