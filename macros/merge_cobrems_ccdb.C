const TString outDirPlots = "/d/home/septian/Eta3PiDalitzPlots/";

const TString inFileNameCobrems = "/d/home/septian/Eta3PiDalitz/DSelectors/Eta3Pi_Thrown_2017_cobremsFlux_genEtaRegge_flat.root";
const TString inFileNameCCDB = "/d/home/septian/Eta3PiDalitz/DSelectors/Eta3Pi_Thrown_2017_ccdbFlux_genEtaRegge_flat.root";
const TString outFileNameCCDBNorm = "/d/home/septian/Eta3PiDalitz/DSelectors/Eta3Pi_Thrown_2017_ccdbFlux_genEtaRegge_flat_norm.root";
const TString outFileName = "/d/home/septian/Eta3PiDalitz/DSelectors/Eta3Pi_Thrown_2017_cobrems_ccdbFlux_genEtaRegge_flat.root";

const TString inFileNameReconCobrems = "/d/home/septian/Eta3PiDalitz/DSelectors/Eta3Pi_Tree_2017_cobremsFlux_genEtaRegge_flat.root";
const TString inFileNameReconCCDB = "/d/home/septian/Eta3PiDalitz/DSelectors/Eta3Pi_Tree_2017_ccdbFlux_genEtaRegge_flat.root";
const TString outFileNameReconCCDBNorm = "/d/home/septian/Eta3PiDalitz/DSelectors/Eta3Pi_Tree_2017_ccdbFlux_genEtaRegge_flat_norm.root";
const TString outFileNameRecon = "/d/home/septian/Eta3PiDalitz/DSelectors/Eta3Pi_Tree_2017_cobrems_ccdbFlux_genEtaRegge_flat.root";

const Double_t EnBeamBinSize = 0.5; 

Double_t merge_thrown_cobrems_ccdb(){
    gStyle->SetOptStat(0);
    TFile *inFileCobrems = TFile::Open(inFileNameCobrems, "READ");
    TFile *inFileCCDB = TFile::Open(inFileNameCCDB, "READ");
    // TFile *outFile = new TFile(outFileName, "RECREATE");

    TTree *treeCobrems = (TTree*)inFileCobrems->Get("nt");
    TTree *treeCCDB = (TTree*)inFileCCDB->Get("nt");

    // TTree *tree = new TTree("tree", "tree");
    // tree->Branch("EnBeam", &Ebeam, "EnBeam/D");

    Int_t nBins = 12.0/EnBeamBinSize;
    TH1F *hEnBeamCobrems = new TH1F("hEnBeamCobrems", "hEnBeamCobrems", nBins, 0, 12);
    TH1F *hEnBeamCCDB = new TH1F("hEnBeamCCDB", "hEnBeamCCDB", nBins, 0, 12);
    Double_t EnBeam;

    treeCobrems->SetBranchAddress("EnBeam", &EnBeam);

    for (int i = 0; i < treeCobrems->GetEntries(); i++){
        treeCobrems->GetEntry(i);
        if (EnBeam < 3.0 || EnBeam > 6.5) continue;
        hEnBeamCobrems->Fill(EnBeam);
        // cout << EnBeam << endl;
        // treeCobremsCCDB->Fill();
    }

    treeCCDB->SetBranchAddress("EnBeam", &EnBeam);
    for (int i = 0; i < treeCCDB->GetEntries(); i++){
        treeCCDB->GetEntry(i);
        if (EnBeam < 6.5 || EnBeam > 11.6) continue;
        hEnBeamCCDB->Fill(EnBeam);
    }

    // tree->Write();
    // outFile->Close();

    TCanvas *c = new TCanvas("c", "c", 800, 600);
    hEnBeamCCDB->GetXaxis()->SetTitle("E_{#gamma} (GeV)");
    hEnBeamCCDB->GetYaxis()->SetTitle("Counts / 0.5 GeV");
    hEnBeamCCDB->SetTitle("E_{#gamma} distribution");
    hEnBeamCCDB->SetLineColor(kBlue);
    hEnBeamCCDB->SetFillColor(kBlue);
    hEnBeamCCDB->SetFillStyle(3001);
    hEnBeamCCDB->Draw();
    hEnBeamCobrems->SetLineColor(kRed);
    hEnBeamCobrems->SetFillColor(kRed);
    hEnBeamCobrems->SetFillStyle(3001);
    hEnBeamCobrems->Draw("same");

    TLegend *leg = new TLegend(0.75, 0.75, 0.9, 0.9);
    leg->AddEntry(hEnBeamCobrems, "Cobrems flux", "f");
    leg->AddEntry(hEnBeamCCDB, "CCDB flux", "f");
    leg->Draw();
    c->SaveAs(outDirPlots + "hEnBeam_cobrems_ccdb.pdf");

    // Get normalization factor by comparing the bin content of the intersection bin
    Int_t binCobrems = hEnBeamCobrems->FindBin(6.4);
    Int_t binContentCobrems = hEnBeamCobrems->GetBinContent(binCobrems);
    Int_t binCCDB = hEnBeamCCDB->FindBin(6.6);
    Int_t binContentCCDB = hEnBeamCCDB->GetBinContent(binCCDB);
    Double_t normFactor = (Double_t)binContentCobrems / (Double_t)binContentCCDB;
    
    cout << "Bin content Cobrems: " << binContentCobrems << endl;
    cout << "Bin content CCDB: " << binContentCCDB << endl;
    cout << "Normalization factor: " << normFactor << endl;

    // Normalize the CCDB flux to the cobrems flux using accept and reject method

    TH1F *hEnBeamCCDBNorm = new TH1F("hEnBeamCCDBNorm", "hEnBeamCCDBNorm", nBins, 0, 12);

    TFile *outFileCCDBNorm = new TFile(outFileNameCCDBNorm, "RECREATE");
    TTree *treeCCDBNorm = treeCCDB->CloneTree(0);

    for (int i = 0; i < treeCCDB->GetEntries(); i++){
        treeCCDB->GetEntry(i);
        if (EnBeam < 6.5 || EnBeam > 11.6) continue;
        if (gRandom->Uniform() < normFactor){
            hEnBeamCCDBNorm->Fill(EnBeam);
            treeCCDBNorm->Fill();
        }
    }

    treeCCDBNorm->Write();
    outFileCCDBNorm->Close();

    TCanvas *cNorm = new TCanvas("cNorm", "cNorm", 800, 600);
    hEnBeamCobrems->GetXaxis()->SetTitle("E_{#gamma} (GeV)");
    hEnBeamCobrems->GetYaxis()->SetTitle("Counts / 0.5 GeV");
    hEnBeamCobrems->SetTitle("Normalized E_{#gamma} distribution");
    hEnBeamCobrems->SetLineColor(kRed);
    hEnBeamCobrems->SetFillColor(kRed);
    hEnBeamCobrems->SetFillStyle(3001);
    hEnBeamCobrems->Draw();
    hEnBeamCCDBNorm->SetLineColor(kBlue);
    hEnBeamCCDBNorm->SetFillColor(kBlue);
    hEnBeamCCDBNorm->SetFillStyle(3001);
    hEnBeamCCDBNorm->Draw("same");

    leg->Clear();
    leg->SetX1(0.70);
    leg->AddEntry(hEnBeamCobrems, "Cobrems flux", "f");
    leg->AddEntry(hEnBeamCCDBNorm, "CCDB flux (normalized)", "f");
    leg->Draw();

    cNorm->SaveAs(outDirPlots + "hEnBeam_cobrems_ccdb_norm.pdf");
    inFileCobrems->Close();
    inFileCCDB->Close();

    // Merge the cobrems and normalized CCDB flux using hadd
    TString haddCommand = "hadd -f " + outFileName + " " + inFileNameCobrems + " " + outFileNameCCDBNorm;
    cout << "Merging files using command: " << haddCommand << endl;
    gSystem->Exec(haddCommand);

    // Draw histogram of the merged file
    TFile *outFile = TFile::Open(outFileName, "READ");
    TTree *tree = (TTree*)outFile->Get("nt");
    TH1F *hEnBeam = new TH1F("hEnBeam", "hEnBeam", nBins, 0, 12);

    tree->SetBranchAddress("EnBeam", &EnBeam);
    for (int i = 0; i < tree->GetEntries(); i++){
        tree->GetEntry(i);
        hEnBeam->Fill(EnBeam);
    }

    TCanvas *cMerged = new TCanvas("cMerged", "cMerged", 800, 600);
    hEnBeam->GetXaxis()->SetTitle("E_{#gamma} (GeV)");
    hEnBeam->GetYaxis()->SetTitle("Counts / 0.5 GeV");
    hEnBeam->SetTitle("Merged E_{#gamma} distribution");
    hEnBeam->SetLineColor(kBlack);
    hEnBeam->SetFillColor(kBlack);
    hEnBeam->SetFillStyle(3001);
    hEnBeam->Draw();
    cMerged->SaveAs(outDirPlots + "hEnBeam_cobrems_ccdb_merged.pdf");

    return normFactor;
}

void merge_recon_cobrems_ccdb(Double_t normFactor){
    gStyle->SetOptStat(0);
    TFile *inFileReconCCDB = TFile::Open(inFileNameReconCCDB, "READ");
    TTree *treeReconCCDB = (TTree*)inFileReconCCDB->Get("myTree");

    Int_t nBins = 12.0/EnBeamBinSize;
    Double_t EnBeam;
    TLorentzVector *P4_PhotonBeam = 0;

    treeReconCCDB->SetBranchAddress("beam_p4_kin", &P4_PhotonBeam);

    TFile *outFileReconCCDBNorm = new TFile(outFileNameReconCCDBNorm, "RECREATE");
    TTree *treeReconCCDBNorm = treeReconCCDB->CloneTree(0);

    cout << " Entries CCDB tree: " << treeReconCCDB->GetEntries() << endl;
    cout << " Normalizing CCDB flux..." << endl;
    for (int i = 0; i < treeReconCCDB->GetEntries(); i++){
        treeReconCCDB->GetEntry(i);
        EnBeam = P4_PhotonBeam->E();
        if (EnBeam < 6.5 || EnBeam > 11.6) continue;
        if (gRandom->Uniform() < normFactor){
            treeReconCCDBNorm->Fill();
        }
    }
    cout << " Entries normalized CCDB tree: " << treeReconCCDBNorm->GetEntries() << endl;
    treeReconCCDBNorm->Write();
    outFileReconCCDBNorm->Close();

    // Merge the cobrems and normalized CCDB flux using hadd
    TString haddCommand = "hadd -f " + outFileNameRecon + " " + inFileNameReconCobrems + " " + outFileNameReconCCDBNorm;
    cout << "Merging files using command: " << haddCommand << endl;
    gSystem->Exec(haddCommand);

    // Draw histogram of the merged file
    TFile *outFile = TFile::Open(outFileNameRecon, "READ");
    TTree *tree = (TTree*)outFile->Get("myTree");
    TH1F *hEnBeam = new TH1F("hEnBeam", "hEnBeam", nBins, 0, 12);

    tree->SetBranchAddress("beam_p4_kin", &P4_PhotonBeam);

    cout << " Entries merged tree: " << tree->GetEntries() << endl;
    cout << " Drawing merged tree beam energy histogram..." << endl;
    for (int i = 0; i < tree->GetEntries(); i++){
        tree->GetEntry(i);
        EnBeam = P4_PhotonBeam->E();
        hEnBeam->Fill(EnBeam);
    }

    TCanvas *cMerged = new TCanvas("cMerged", "cMerged", 800, 600);
    hEnBeam->GetXaxis()->SetTitle("E_{#gamma} (GeV)");
    hEnBeam->GetYaxis()->SetTitle("Counts / 0.5 GeV");
    hEnBeam->SetTitle("Merged E_{#gamma} distribution");
    hEnBeam->SetLineColor(kBlack);
    hEnBeam->SetFillColor(kBlack);
    hEnBeam->SetFillStyle(3001);
    hEnBeam->Draw();
    cMerged->SaveAs(outDirPlots + "hEnBeam_cobrems_ccdb_merged_recon.pdf");

    inFileReconCCDB->Close();
}

void merge_cobrems_ccdb(){
    // Double_t normFactor = merge_thrown_cobrems_ccdb();
    Double_t normFactor = 0.179107;
    cout << "Normalization factor: " << normFactor << endl;
    merge_recon_cobrems_ccdb(normFactor);
}