//user config for energy beam cut
const Int_t NBinPhotonEnergyBeam = 5;
const Double_t PhotonBeamEnergyBin[NBinPhotonEnergyBeam+1] = {6.5,7.0,8.0,9.0,10.0,99.0};

//user config for mandelstamT cut
Bool_t enableMandelstamTCut = kFALSE;
const Int_t NMandelstamTBin = 9;
const Double_t t_range[NMandelstamTBin+1] = {0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0};

Int_t NOutFiles = NBinPhotonEnergyBeam;

void makeMCThrownTreeFidCuts(TString fileName, TString outNameBase){

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
    }
}