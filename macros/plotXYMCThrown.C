void set_axis(TAxis *axis,TString title,Double_t offset,Double_t size,Double_t label_size){
    axis->SetTitle(title);
    axis->SetTitleOffset(offset);
    axis->SetTitleSize(size);
    axis->SetLabelSize(label_size);
}

void set_palette(TH2F *my2dhisto){
    gPad->Update();
    TPaletteAxis *palette = (TPaletteAxis*)my2dhisto->GetListOfFunctions()->FindObject("palette");
    palette->SetX1NDC(0.9);
    palette->SetX2NDC(0.92);
    palette->SetY1NDC(0.15);
    palette->SetY2NDC(0.9);
    gPad->Modified();
    gPad->Update();
}

void plotXYMCThrown(TString fileName, TString outName){
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(0);
    gStyle->SetPadBottomMargin(0.15);
    gStyle->SetPadLeftMargin(0.15);
    gStyle->SetLegendTextSize(0.05);
    gStyle->SetLegendBorderSize(1);
    
    Double_t offset = 1.35;
    Double_t size = 0.06;
    Double_t label_size = 0.04;

    TChain *dataChain = new TChain("nt");
    dataChain->Add(fileName.Data());
    Int_t nEntries = dataChain->GetEntries();
    cout << "Filename: " << fileName << endl; 
    cout << "Number of entries: " << nEntries << endl;

    Double_t EnP1,PxP1,PyP1,PzP1;
    Double_t EnP2,PxP2,PyP2,PzP2;
    Double_t EnP3,PxP3,PyP3,PzP3;
    Double_t weight;

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

    TH2F *h2DalitzPlotEta3Pi = new TH2F("h2_DalitzPlotEta3Pi_thrown","",101,-1.0,1.0,101,-1.0,1.0);

    for(Int_t i=0;i<nEntries;i++){
        dataChain->GetEntry(i);

        TLorentzVector pip_p4_kin = TLorentzVector(PxP1,PyP1,PzP1,EnP1);
        TLorentzVector pim_p4_kin = TLorentzVector(PxP2,PyP2,PzP2,EnP2);
        TLorentzVector pi0_p4_kin = TLorentzVector(PxP3,PyP3,PzP3,EnP3);;

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

        h2DalitzPlotEta3Pi->Fill(X_c,Y_c,weight);
    }

    TCanvas *c =new TCanvas("c","Dalitz plot",1200,1200);
    h2DalitzPlotEta3Pi->GetXaxis()->SetTitle("X");
    h2DalitzPlotEta3Pi->GetYaxis()->SetTitle("Y");
    h2DalitzPlotEta3Pi->Draw("COLZ");
    h2DalitzPlotEta3Pi->SetMinimum(0);
    set_axis(h2DalitzPlotEta3Pi->GetXaxis(),"Dalitz Variable X",offset-0.4,size,label_size);
    set_axis(h2DalitzPlotEta3Pi->GetYaxis(),"Dalitz Variable Y",offset,size,label_size);
    set_palette(h2DalitzPlotEta3Pi);
    c->Print("plotXYMCThrown.pdf");
    TFile *out = new TFile(outName,"RECREATE");
    h2DalitzPlotEta3Pi->Write();
    out->Close();

}