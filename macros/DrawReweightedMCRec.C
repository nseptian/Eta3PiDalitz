const TString inputDir = "/d/home/septian/Eta3PiDalitz/run/root4AmptoolsEnergyDep/";
const TString pdfOutDir = "/d/home/septian/Eta3PiDalitzPlots/";
const TString pdfOut_base = "2017_data_t01506_cobremsFlux_EgBin_genEtaRegge";

const Int_t NPhotonBeamEnergyBin = 8;
const Double_t PhotonBeamEnergyBin[NPhotonBeamEnergyBin+1] = {3.0,4.0,5.0,6.5,7.5,8.0,9.0,10.0,11.6};

void DrawReweightedMCRec(){

    TCanvas *c1 = new TCanvas("c1","c1",1200,600);
    TLegend *leg1 = new TLegend(0.5,0.7,0.85,0.85);

    c1->Divide(4,2);
    for (Int_t i=0; i<NPhotonBeamEnergyBin; i++) {
        TString fluxName = "";
        Int_t iPhotonBeamEnergyBin = i;
        if (i<3) fluxName = "cobremsFlux";
        else {
            fluxName = "ccdbFlux";
            iPhotonBeamEnergyBin = i-3;    
        }
        TString fileInputName = Form("eta_2017_data_t01506_%s_EgBin_genEtaRegge_%d.root",fluxName.Data(),iPhotonBeamEnergyBin);
        TString fileInputNameMC = Form("mc_rec_2017_data_t01506_%s_EgBin_genEtaRegge_%d.root",fluxName.Data(),iPhotonBeamEnergyBin);
        TString fileInputNameReweightedMC = Form("reweighted_mc_rec_2017_data_t01506_%s_EgBin_genEtaRegge_%d.root",fluxName.Data(),iPhotonBeamEnergyBin);
        
        //open input file
        TFile *fileInput = TFile::Open(inputDir+fileInputName, "READ");
        TFile *fileInputMC = TFile::Open(inputDir+fileInputNameMC, "READ");
        
        TDirectory *dir = (TDirectory*)fileInput->Get("hist_SidebandSub");
        TDirectory *dirMC = (TDirectory*)fileInputMC->Get("hist_SidebandSub");
        
        //get mandelstam_t histograms
        TString hname = "h_mandelstam_t_sbsAll";
        TH1F *h1_mandelstam_t_data = (TH1F*)dir->Get(hname.Data());
        TH1F *h1_mandelstam_t_MC = (TH1F*)dirMC->Get(hname.Data());

        //open reweighted MC file
        TFile *fileInputReweightedMC = TFile::Open(inputDir+fileInputNameReweightedMC, "READ");
        TH1F *h1_mandelstam_t_reweightedMC = (TH1F*)fileInputReweightedMC->Get("h1_mandelstam_t_weighted");
        
        //scale h1_mandelstam_t_MC to match h1_mandelstam_t_data
        h1_mandelstam_t_reweightedMC->Scale(h1_mandelstam_t_data->Integral()/h1_mandelstam_t_reweightedMC->Integral());
        
        //set histogram properties
        h1_mandelstam_t_data->SetTitle(Form("%0.1f GeV < E_{#gamma} < %0.1f GeV",PhotonBeamEnergyBin[i],PhotonBeamEnergyBin[i+1]));
        h1_mandelstam_t_data->GetXaxis()->SetTitle("|t| [GeV^{2}]");
        h1_mandelstam_t_data->GetYaxis()->SetTitle("Events")
        h1_mandelstam_t_data->SetStats(0);
        h1_mandelstam_t_data->SetLineColor(kRed);
        h1_mandelstam_t_data->SetLineWidth(2);
        h1_mandelstam_t_reweightedMC->SetLineColor(kBlack);
        h1_mandelstam_t_reweightedMC->SetLineWidth(2);

        c1->cd(i+1);
        h1_mandelstam_t_data->Draw();
        h1_mandelstam_t_reweightedMC->Draw("SAME");

        leg1->Clear();
        leg1->AddEntry(h1_mandelstam_t_data,"Data","l");
        leg1->AddEntry(h1_mandelstam_t_reweightedMC,"Reweighted MC","l");
        leg1->Draw();

    }

    c1->SaveAs((pdfOutDir+pdfOut_base).Data()+TString("_mandelstam_t.pdf"));

}