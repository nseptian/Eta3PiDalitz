// Purpose: draw beam energy histogram

TString fileName = "/d/home/septian/Eta3PiDalitz/run/root4AmptoolsEnergyDep/eta_2017_data_t01506_ccdbFlux_genrEtaRegge_0.root";
TString fileNameMC = "/d/home/septian/Eta3PiDalitz/run/root4AmptoolsEnergyDep/mc_rec_2017_data_t01506_ccdbFlux_genEtaRegge_0.root";
TString dataTag = "2017_t01506_ccdbFlux_genEtaRegge";
TString outDir = "/d/home/septian/Eta3PiDalitzPlots/";

void gluex_style();
void drawHistE(){
    // gluex_style();
    // gSystem->Exec("mkdir -p plots");

	TFile* f=new TFile(fileName,"READ");
    TFile* fMC=new TFile(fileNameMC,"READ");
	TH1F* h1;
    TH1F* h1MC;

    TString histName = "h_BeamEnergy";

    h1=(TH1F*)f->Get(histName.Data());
    h1MC=(TH1F*)fMC->Get(histName.Data());

    //scale h1MC to h1
    h1MC->Scale(h1->Integral()/h1MC->Integral());

    h1->SetLineColor(kRed);
    h1MC->SetLineColor(kBlue);

    h1->SetLineWidth(2);
    h1MC->SetLineWidth(2);
    h1->SetStats(0);
    h1->SetTitle("");
    h1->GetXaxis()->SetTitle("E_{#gamma} (GeV)");
    h1->GetYaxis()->SetTitle(Form("Events/%0.2f GeV",h1->GetBinWidth(1)));
    h1->GetYaxis()->SetTitleOffset(1.6);
    h1->GetXaxis()->SetRangeUser(6.0,12.0);
    h1->GetYaxis()->SetRangeUser(0.0,1.4*h1->GetMaximum());

    //draw h1 and scaled h1 MC in the same canvas

    TCanvas* c1=new TCanvas("c1","c1",800,600);
    c1->cd();
    h1->Draw("HIST");
    h1MC->Draw("HIST SAME");

    TLegend* leg=new TLegend(0.55,0.8,0.9,0.9);
    leg->AddEntry(h1,TString("Data ")+dataTag.Data(),"l");
    leg->AddEntry(h1MC,TString("Normalized signal MC ")+dataTag.Data(),"l");
    leg->Draw();
    c1->SetLeftMargin(0.15);
    c1->SaveAs(Form("%sbeamEnergy_%s.pdf",outDir.Data(),dataTag.Data()));

}

void gluex_style() {
	///////// 
	// Make the plots prettier with bigger fonts
	// Original code is https://halldweb.jlab.org/wiki-private/index.php/Guidance_for_Presentations 
	// 	slightly modified
	/////////
	auto gluex_style = new TStyle("GlueX","Default GlueX Style");

        gluex_style->SetCanvasBorderMode(0);
        gluex_style->SetPadBorderMode(0);
        gluex_style->SetPadColor(0);
        gluex_style->SetCanvasColor(0);
        gluex_style->SetStatColor(0);

	// some default window sizes
	gluex_style->SetCanvasDefW(800);
	gluex_style->SetCanvasDefH(600);

	// let's change the default margins
	gluex_style->SetPadBottomMargin(0.2);
	gluex_style->SetPadLeftMargin(1.0);
	gluex_style->SetPadTopMargin(0.08);
	gluex_style->SetPadRightMargin(0.15);

	// axis labels and settings
    gluex_style->SetStripDecimals(0);
 	gluex_style->SetLabelSize(0.045,"xyz"); // size of axis value font
 	gluex_style->SetTitleSize(0.06,"xyz"); // size of axis title font
 	gluex_style->SetTitleFont(42,"xyz"); // font option
 	gluex_style->SetLabelFont(42,"xyz"); 
 	gluex_style->SetTitleOffset(0.0,"y"); 
 	gluex_style->SetLabelOffset(0.01,"xyz");   // stop collisions of "0"s at the origin
 	
	// histogram settings
	gluex_style->SetOptStat(0);     // no stats box by default
	gluex_style->SetOptTitle(1);    // no title by default
	gluex_style->SetHistLineWidth(2); 
	gluex_style->SetNdivisions(508,"xyz"); // some ticks were very bunched, lets reduce the number of divisions to label 
	// gluex_style->SetOptFit(0111);

	gluex_style->SetHistFillColor(920);   // grey
	gluex_style->SetPalette(kViridis); // kViridis is perceptually uniform and colorblind friendly
	gluex_style->cd();
}