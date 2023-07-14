// Purpose: draw beam energy histogram

TString fileName = "/d/home/septian/Eta3PiDalitz/run/root4Amptools/eta_2017_data_BeamEnergy.root";
TString fileNameMC = "/d/home/septian/Eta3PiDalitz/run/root4Amptools/mc_rec_2017_data_BeamEnergy.root";

void drawHistE(){
    gSystem->Exec("mkdir -p plots");

	TFile* f=new TFile(fileName,"READ");
    TFile* fMC=new TFile(fileNameMC,"READ");
	TH1F* h1;
    TH1F* h1MC;

    TString histName = "h_BeamEnergy";

    h1=(TH1F*)f->Get(histName.Data());
    h1MC=(TH1F*)fMC->Get(histName.Data());

    //scale h1MC to h1
    h1MC->Scale(0.5*h1->Integral()/h1MC->Integral());

    h1->SetLineColor(kRed);
    h1MC->SetLineColor(kBlue);

    h1->SetLineWidth(2);
    h1MC->SetLineWidth(2);
    h1->SetStats(0);
    h1->SetTitle("");
    h1->GetXaxis()->SetTitle("Beam Energy [GeV]");
    h1->GetYaxis()->SetTitle("Events");

    //draw h1 and scaled h1 MC in the same canvas

    TCanvas* c1=new TCanvas("c1","c1",800,600);
    c1->cd();
    h1->Draw("HIST");
    h1MC->Draw("HIST SAME");

    TLegend* leg=new TLegend(0.7,0.8,0.9,0.9);
    leg->AddEntry(h1,"Data","l");
    leg->AddEntry(h1MC,"Scaled MC","l");
    leg->Draw();
    
    c1->SaveAs("plots/beamEnergy.pdf");

}