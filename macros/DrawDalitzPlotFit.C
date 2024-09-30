const TString inputDir = "/d/home/septian/Eta3PiDalitzPlots/";
const TString pdfOutDir = "/d/home/septian/Eta3PiDalitzPlots/";
const TString pdfOut_base = "2018F_expPolyBkg_PhotonBeamEnergy";

// const Int_t NPhotonBeamEnergyBin = 8;
// const Double_t PhotonBeamEnergyBin[NPhotonBeamEnergyBin+1] = {3.0,4.0,5.0,6.5,7.5,8.0,9.0,10.0,11.6};

const Int_t NPhotonBeamEnergyBin = 5;
const Double_t PhotonBeamEnergyBin[NPhotonBeamEnergyBin+1] = {6.5,7.5,8.0,9.0,10.0,11.6};

void SetPlotStyle();
void DrawDalitzPlotFit(){
    SetPlotStyle();

    TCanvas *c1 = new TCanvas("c1","c1",1200,600);
    c1->Divide(4,2);
    TLegend *leg1 = new TLegend(0.45,0.2,0.65,0.35);

    TCanvas *c2 = new TCanvas("c2","c2",1200,600);
    c2->Divide(4,2);
    TLegend *leg2 = new TLegend(0.45,0.2,0.65,0.35);

    for (Int_t i=0;i<NPhotonBeamEnergyBin;i++) {
        TString fluxName = "";
        Int_t iPhotonBeamEnergyBin = i+3;
        // if (i<3) fluxName = "cobremsFlux";
        // else {
        //     fluxName = "ccdbFlux";
        //     iPhotonBeamEnergyBin = i-3;
        // }
        // TString fileInputName = Form("dalitzPlot_2018F_t01506_%s_EgBin_genEtaRegge_%d.root",fluxName.Data(),iPhotonBeamEnergyBin);
        TString fileInputName = Form("dalitzPlot_2018F_expPolyBkg_PhotonBeamEnergy_%d.root",iPhotonBeamEnergyBin);
        //open input file
        TFile *fileInput = TFile::Open(inputDir+fileInputName, "READ");

        // get hX and hY histograms for data
        TString hnameX = "hXdat";
        TString hnameY = "hYdat";
        TH1F *h1_X_data = (TH1F*)fileInput->Get(hnameX.Data());
        TH1F *h1_Y_data = (TH1F*)fileInput->Get(hnameY.Data());

        // get hX and hY histograms for the fits
        hnameX = "hXacc";
        hnameY = "hYacc";
        TH1F *h1_X_fit = (TH1F*)fileInput->Get(hnameX.Data());
        TH1F *h1_Y_fit = (TH1F*)fileInput->Get(hnameY.Data());
        
        h1_X_data->SetTitle(Form("%0.1f GeV < E_{#gamma} < %0.1f GeV",PhotonBeamEnergyBin[i],PhotonBeamEnergyBin[i+1]));
        h1_X_data->GetXaxis()->SetTitle("X");
        h1_X_data->GetXaxis()->SetTitleSize(0.05);
        h1_X_data->GetXaxis()->SetTitleOffset(1.0);
        h1_X_data->GetYaxis()->SetTitle("Events");
        h1_X_data->GetYaxis()->SetTitleSize(0.05);
        h1_X_data->GetYaxis()->SetTitleOffset(1.2);

        h1_X_data->SetStats(0);
        h1_X_data->SetLineColor(kBlack);
        h1_X_data->SetLineWidth(2);
        h1_X_fit->SetLineColor(kRed);
        h1_X_fit->SetLineWidth(2);

        c1->cd(i+1);
        
        h1_X_data->Draw();
        h1_X_fit->Draw("SAME");

        leg1->Clear();
        leg1->AddEntry(h1_X_data,"Data","l");
        leg1->AddEntry(h1_X_fit,"Fit","l");
        leg1->Draw();

        // draw Y
        h1_Y_data->SetTitle(Form("%0.1f GeV < E_{#gamma} < %0.1f GeV",PhotonBeamEnergyBin[i],PhotonBeamEnergyBin[i+1]));
        h1_Y_data->GetXaxis()->SetTitle("Y");
        h1_Y_data->GetXaxis()->SetTitleSize(0.05);
        h1_Y_data->GetXaxis()->SetTitleOffset(1.0);
        h1_Y_data->GetYaxis()->SetTitle("Events");
        h1_Y_data->GetYaxis()->SetTitleSize(0.05);
        h1_Y_data->GetYaxis()->SetTitleOffset(1.2);

        h1_Y_data->SetStats(0);
        h1_Y_data->SetLineColor(kBlack);
        h1_Y_data->SetLineWidth(2);
        h1_Y_fit->SetLineColor(kRed);
        h1_Y_fit->SetLineWidth(2);

        c2->cd(i+1);

        h1_Y_data->Draw();
        h1_Y_fit->Draw("SAME");

        leg2->Clear();
        leg2->AddEntry(h1_Y_data,"Data","l");
        leg2->AddEntry(h1_Y_fit,"Fit","l");
        leg2->Draw();

    }

    c1->SaveAs((pdfOutDir+pdfOut_base).Data()+TString("_dalitzXPlotFit.pdf"));
    c2->SaveAs((pdfOutDir+pdfOut_base).Data()+TString("_dalitzYPlotFit.pdf"));

}

void SetPlotStyle() {
    gStyle->SetOptStat(0);
    gStyle->SetPadLeftMargin(0.15);
    gStyle->SetPadRightMargin(0.05);
    gStyle->SetPadTopMargin(0.1);
    gStyle->SetPadBottomMargin(0.15);
}