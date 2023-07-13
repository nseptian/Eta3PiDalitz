TString rootFilesDir = "/d/home/septian/Eta3PiDalitz/run/root4AmptoolsEnergyDep/";
TString outDir = "/d/home/septian/Eta3PiDalitz/run/plots/";
TString fileName = "eta_2017_data";
TString fileNameMC = "mc_rec_2017_data";
TString inputTag = "NoSSB_t_30300";
TString outputTag = "NoSSB";
const Int_t NPhotonEnergyBin = 5;
const Double_t PhotonEnergyBin[NPhotonEnergyBin+1] = {0.0,7.0,8.0,9.0,10.0,99.0};
const Double_t MandelstamTBin[2] = {3.0,30.0};

void gluex_style();
void drawTHistEnergyDep(){
	gSystem->Exec("mkdir -p plots");

	// gluex_style();
	// gROOT->ForceStyle();
	gStyle->SetOptStat(0);	

	TString drawOptions="L";

	// TFile* f=new TFile(fileName,"READ");
	// TFile* fMC=new TFile(fileNameMC,"READ");
	TH1F* h1_MandelstamT[NPhotonEnergyBin];
	TH1F* h1_MandelstamT_MC[NPhotonEnergyBin];
	TBox* box=new TBox();
	TCanvas* c=new TCanvas("","",800,600);
	TString hname = TString("h_mandelstam_t");

	TH1F* h1_MandelstamT_5;
	TH1F* h1_MandelstamT_MC_5;

	//get histograms
	for(Int_t i=0;i<NPhotonEnergyBin;i++){
		TString fName = Form("%s%s_%s_%d.root",rootFilesDir.Data(),fileName.Data(),inputTag.Data(),i);
		TString fNameMC = Form("%s%s_%s_%d.root",rootFilesDir.Data(),fileNameMC.Data(),inputTag.Data(),i);
		TFile* f=new TFile(fName,"READ");
		TFile* fMC=new TFile(fNameMC,"READ");

		hname = TString("h_mandelstam_t");
		h1_MandelstamT[i] = (TH1F*)f->Get(hname);
		h1_MandelstamT[i]->GetXaxis()->SetTitle("|t| (GeV^{2})");
		h1_MandelstamT[i]->GetYaxis()->SetTitle("");
		h1_MandelstamT[i]->SetTitle(Form("Mandelstam t distribution for %0.2f < E_{#gamma} < %0.2f GeV",PhotonEnergyBin[i],PhotonEnergyBin[i+1]));
		h1_MandelstamT[i]->GetXaxis()->SetRangeUser(MandelstamTBin[0],MandelstamTBin[1]);
		
		h1_MandelstamT_MC[i] = (TH1F*)fMC->Get(hname);
		h1_MandelstamT_MC[i]->SetLineColor(kRed);
		h1_MandelstamT_MC[i]->SetLineWidth(2);

		if (i==0) {
			h1_MandelstamT_5 = new TH1F(*h1_MandelstamT[i]);
			h1_MandelstamT_MC_5 = new TH1F(*h1_MandelstamT_MC[i]);
		}
		else {
			h1_MandelstamT_5->Add(h1_MandelstamT[i]);
			h1_MandelstamT_MC_5->Add(h1_MandelstamT_MC[i]);
		}

		// f->Close();
		// fMC->Close();
	}


	// draw histograms in one canvas
	c->Divide(3,2);
	for(Int_t i=0;i<NPhotonEnergyBin;i++){
		c->cd(i+1);
		h1_MandelstamT[i]->Draw(drawOptions.Data());

		//draw MC with scale
		Double_t scale = h1_MandelstamT[i]->Integral()/h1_MandelstamT_MC[i]->Integral();
		h1_MandelstamT_MC[i]->Scale(scale);
		h1_MandelstamT_MC[i]->Draw(Form("%s SAME",drawOptions.Data()));
	}
	c->cd(6);
	h1_MandelstamT_5->SetTitle(Form("Mandelstam t distribution for %0.2f < E_{#gamma} < %0.2f GeV",PhotonEnergyBin[0],PhotonEnergyBin[NPhotonEnergyBin]));
	h1_MandelstamT_5->Draw(drawOptions.Data());
	h1_MandelstamT_MC_5->Scale(h1_MandelstamT_5->Integral()/h1_MandelstamT_MC_5->Integral());
	h1_MandelstamT_MC_5->Draw(Form("%s SAME",drawOptions.Data()));
	TLegend *leg = new TLegend(0.4,0.7,0.6,0.8);
	leg->AddEntry(h1_MandelstamT_5,"Data","l");
	leg->AddEntry(h1_MandelstamT_MC_5,"Scaled MC","l");
	leg->Draw();

	c->SaveAs(Form("%s%s_%s_%s.pdf",outDir.Data(),fileName.Data(),inputTag.Data(),outputTag.Data()));

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
	gluex_style->SetPadBottomMargin(0.15);
	gluex_style->SetPadLeftMargin(0.2);
	gluex_style->SetPadTopMargin(0.08);
	gluex_style->SetPadRightMargin(0.15);

	// axis labels and settings
    gluex_style->SetStripDecimals(0);
 	gluex_style->SetLabelSize(0.045,"xyz"); // size of axis value font
 	gluex_style->SetTitleSize(0.06,"xyz"); // size of axis title font
 	gluex_style->SetTitleFont(42,"xyz"); // font option
 	gluex_style->SetLabelFont(42,"xyz"); 
 	gluex_style->SetTitleOffset(1.5,"y"); 
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