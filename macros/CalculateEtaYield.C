TString workDir = "/d/grid17/septian/Eta3PiDalitz/run/Eta3PiSelectorParallelTree_nominal/";
TString inFileName = "data_2017_nominal_ccdbFlux_0.root";
TString inFileNameMC = "reweighted_mc_rec_2017_nominal_ccdbFlux_0.root";

// TString inFileName = "data_2017_nominal_cobremsFlux_0.root";
// TString inFileNameMC = "reweighted_mc_rec_2017_nominal_cobremsFlux_0.root";

// TString inFileName = "data_2018S_nominal_None_0.root";
// TString inFileNameMC = "reweighted_mc_rec_2018S_nominal_None_0.root";

// TString inFileName = "data_2018F_nominal_None_0.root";
// TString inFileNameMC = "reweighted_mc_rec_2018F_nominal_None_0.root";

void CalculateEtaYield(){
    // Load the input file
    TFile* inFile = TFile::Open(workDir+inFileName);
    TFile* inFileMC = TFile::Open(workDir+inFileNameMC);
    if (!inFile || !inFileMC) {
        cout << "Error: Unable to open input file" << endl;
        return;
    }
    
    // Load the input tree
    TTree* inTree = (TTree*)inFile->Get("nt");
    TTree* inTreeMC = (TTree*)inFileMC->Get("nt");

    // Set active branches
    Double_t weight=0;
    inTree->SetBranchAddress("weight", &weight);

    Double_t SumWeight=0;

    cout << "Entries in data tree: " << inTree->GetEntries() << endl;
    // loop over the data tree
    for (Long64_t i = 0; i < inTree->GetEntries(); i++) {
        inTree->GetEntry(i);
        SumWeight += weight;
    }

    cout << "SumWeight = " << SumWeight << endl;

    Double_t SumWeightMC=0;

    // loop over the MC tree
    inTreeMC->SetBranchAddress("weight", &weight);
    for (Long64_t i = 0; i < inTreeMC->GetEntries(); i++) {
        inTreeMC->GetEntry(i);
        SumWeightMC += weight;
    }

    cout << "SumWeightMC = " << SumWeightMC << endl;

    // Calculate ratio of MC to data
    Double_t Ratio = SumWeightMC/SumWeight;
    cout << "Ratio MC to data = " << Ratio << endl;
}