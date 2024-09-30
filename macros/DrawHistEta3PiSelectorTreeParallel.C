
// const TString inputDir = "/d/home/septian/Eta3PiDalitz/run/Eta3PiSelectorParallelTree_t01506_ccdbcobremsFlux_genEtaRegge/";
// const TString inputFile = "data_2017_t01506_ccdbcobremsFlux_genEtaRegge.root";
// const TString inputFileSidebandSubtracted = "data_2017_data_t01506_ccdbcobremsFlux_genEtaRegge_0.root";
// const TString inputFileMC = "mc_rec_2017_data_t01506_ccdbcobremsFlux_genEtaRegge_0.root";
// const TString outputDir = "/d/home/septian/Eta3PiDalitzPlots/";

const TString inputDir = "/d/home/septian/Eta3PiDalitz/run/Eta3PiSelectorParallelTree_t01506_ccdbFlux_genEtaRegge/";
const TString inputFile = "WOSBS_data_2018S_t01506_ccdbFlux_genEtaRegge_0.root";
const TString inputFileSidebandSubtracted = "data_2018S_t01506_ccdbFlux_genEtaRegge_0.root";
const TString inputFileMC = "mc_rec_2018S_t01506_ccdbFlux_genEtaRegge_0.root";
const TString outputDir = "/d/home/septian/Eta3PiDalitzPlots/";

void DrawHistEta3PiSelectorTreeParallel(){
    
    TFile *fileInput = TFile::Open(inputDir+inputFile, "READ");
    TDirectory *dir = (TDirectory*)fileInput->Get("Histograms");

    TString histName = "h1_PiPlusPiMinusGamma1Gamma2InvMass";
    TH1F *h1 = (TH1F*)dir->Get(histName.Data());

    // Double_t mu1 = 0.54742;
    // Double_t sigma1 = 0.00613;
    // Double_t mu2 = 0.54844;
    // Double_t sigma2 = 0.0120;

    // // calculate weighted average of two gaussians
    // Double_t mu = (mu1/(sigma1*sigma1) + mu2/(sigma2*sigma2))/(1/(sigma1*sigma1) + 1/(sigma2*sigma2));
    // Double_t sigma = TMath::Sqrt(1/(1/(sigma1*sigma1) + 1/(sigma2*sigma2)));

    // // Draw histogram with signal and background bands
    TCanvas *c1 = new TCanvas("c1","c1",1200,600);
    // h1->GetXaxis()->SetTitle("M_{#pi^{+}#pi^{-}#gamma_{1}#gamma_{2}} (GeV)");
    // h1->GetYaxis()->SetTitle("Events");
    // h1->SetLineColor(kBlack);
    // h1->Draw("HIST");
    // TBox *box1 = new TBox(mu-2*sigma,0,mu+2*sigma,h1->GetMaximum());
    // box1->SetFillColor(kGreen);
    // box1->SetFillStyle(3002);
    // box1->Draw();
    // TBox *box2 = new TBox(0.455,0,0.48,h1->GetMaximum());
    // box2->SetFillColor(kRed);
    // box2->SetFillStyle(3002);
    // box2->Draw();
    // TBox *box3 = new TBox(0.61,0,0.635,h1->GetMaximum());
    // box3->SetFillColor(kRed);
    // box3->SetFillStyle(3002);
    // box3->Draw();
    // c1->SaveAs(outputDir+"PiPlusPiMinusGamma1Gamma2InvMass_bands.pdf");

    histName = "h1_EnergyBeam";

    TFile *fileInputSidebandSubtracted = TFile::Open(inputDir+inputFileSidebandSubtracted, "READ");
    TDirectory *dirSidebandSubtracted = (TDirectory*)fileInputSidebandSubtracted->Get("Histograms");

    TH1F *h1_EnergyBeamSidebandSubtracted = (TH1F*)dirSidebandSubtracted->Get(histName.Data());
    
    TFile *fileInputMC = TFile::Open(inputDir+inputFileMC, "READ");
    TDirectory *dirMC = (TDirectory*)fileInputMC->Get("Histograms");

    TH1F *h1_EnergyBeamMC = (TH1F*)dirMC->Get(histName.Data());

    //scale h1MC to h1
    h1_EnergyBeamMC->Scale(h1_EnergyBeamSidebandSubtracted->Integral()/h1_EnergyBeamMC->Integral());

    h1_EnergyBeamSidebandSubtracted->SetLineColor(kRed);
    h1_EnergyBeamMC->SetLineColor(kBlue);

    h1_EnergyBeamSidebandSubtracted->SetLineWidth(2);
    h1_EnergyBeamMC->SetLineWidth(2);
    h1_EnergyBeamMC->SetStats(0);
    h1_EnergyBeamMC->SetTitle("");
    h1_EnergyBeamMC->GetXaxis()->SetTitle("E_{#gamma} (GeV)");
    h1_EnergyBeamMC->GetYaxis()->SetTitle(Form("Events/%0.2f GeV",h1_EnergyBeamSidebandSubtracted->GetBinWidth(1)));
    h1_EnergyBeamMC->GetYaxis()->SetTitleOffset(1.6);
    h1_EnergyBeamMC->GetXaxis()->SetRangeUser(2.5,12.0);
    h1_EnergyBeamMC->GetYaxis()->SetRangeUser(0.0,1.2*h1_EnergyBeamMC->GetMaximum());

    //draw h1 and scaled h1 MC in the same canvas

    TCanvas* c2=new TCanvas("c2","c2",1200,600);
    c2->cd();

    h1_EnergyBeamMC->GetXaxis()->SetRangeUser(6.0,12.0);
    h1_EnergyBeamMC->Draw("HIST");
    h1_EnergyBeamSidebandSubtracted->Draw("HIST SAME");

    TLegend* leg=new TLegend(0.55,0.7,0.9,0.9);
    leg->AddEntry(h1_EnergyBeamSidebandSubtracted,"Data (2018S)","l");
    leg->AddEntry(h1_EnergyBeamMC,"Scaled signal MC (2018S)","l");
    leg->Draw();

    c2->SetLeftMargin(0.15);
    c2->SaveAs(outputDir+"2018S_beamEnergy_ccdbFlux.pdf");

}