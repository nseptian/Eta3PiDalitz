const Double_t PhotonBeamEnergyBin[6] = {0.65,7.0,8.0,9.0,10.0,99.0};
const Double_t MandelstamtTRange[2] = {0.1,0.6};
TString fileInputName = "/d/home/septian/Eta3PiDalitz/run/root4Amptools/Eta3Pi_Thrown_2017_genEtaRegge_flat.root";
TString fileOutputBaseName = "mc_thrown_2017_t0106_";

void selectMCThrown(){ 
    TChain *dataChain = new TChain("nt");
    dataChain->Add(fileInputName.Data());
    Int_t nEntries = dataChain->GetEntries();
    cout << "Filename: " << fileInputName << endl; 
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

    //loop over energy bins
    for(Int_t i=0;i<5;i++){
        Double_t EnBeamMin = PhotonBeamEnergyBin[i];
        Double_t EnBeamMax = PhotonBeamEnergyBin[i+1];
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
        Double_t Weight = 1;
        treeOutput->Branch("weight",&Weight,"weight/D");            

        //loop over entries

        for(Int_t i=0;i<nEntries;i++){
            dataChain->GetEntry(i);
            if(EnBeam<EnBeamMin || EnBeam>EnBeamMax) continue;
            if(TMath::Abs(t)<MandelstamtTRange[0] || TMath::Abs(t)>MandelstamtTRange[1]) continue;

            // fill output tree
            treeOutput->Fill();
        }
        //write output file
        TString fileOutputName = Form("%s%d.root",fileOutputBaseName.Data(),i);
        cout << "Output file: " << fileOutputName << endl;
        //create output file
        TFile *fileOutput = new TFile(fileOutputName.Data(),"RECREATE");
        treeOutput->Write();
        treeOutput->Delete();
        fileOutput->Close();
        fileOutput->Delete();
    }
}