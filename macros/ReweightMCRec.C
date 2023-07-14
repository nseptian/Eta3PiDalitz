// Description: Reweight MC reco to match data t slope

TString inputDir = "/d/home/septian/Eta3PiDalitz/run/root4AmptoolsEnergyDep/";
TString fileInputName = "eta_2017_data_t0006_0.root";
TString fileInputNameMC = "mc_rec_2017_data_t0006_0.root";

TString outputDir = "/d/home/septian/Eta3PiDalitz/run/root4AmptoolsEnergyDep/";
TString fileOutputName = "reweighted_mc_rec_2017_data_t0006_0.root";

TString pdfOut_base = "2017_data_t0006_0";

Bool_t isGenerateWeightedTrees = kFALSE;

void gluex_style();
void ReweightMCRec(){

    gluex_style();
	gROOT->ForceStyle();
    //open input file
    TFile *fileInput = new TFile(inputDir+fileInputName);
    TFile *fileInputMC = new TFile(inputDir+fileInputNameMC);

    //get mandelstam_t histograms
    TString hname = "h_mandelstam_t";
    TH1F *h1_mandelstam_t_data = (TH1F*)fileInput->Get(hname.Data());
    TH1F *h1_mandelstam_t_MC = (TH1F*)fileInputMC->Get(hname.Data());
    
    //scale h1_mandelstam_t_MC to match h1_mandelstam_t_data
    h1_mandelstam_t_MC->Scale(h1_mandelstam_t_data->Integral()/h1_mandelstam_t_MC->Integral());
    
    //set histogram properties
    h1_mandelstam_t_data->SetTitle("Mandelstam t");
    h1_mandelstam_t_data->GetXaxis()->SetTitle("Mandelstam t [GeV^{2}]");
    h1_mandelstam_t_data->GetYaxis()->SetTitle("Events");
    h1_mandelstam_t_data->SetStats(0);
    h1_mandelstam_t_data->SetLineColor(kRed);
    h1_mandelstam_t_data->SetLineWidth(2);
    h1_mandelstam_t_MC->SetLineColor(kBlue);
    h1_mandelstam_t_MC->SetLineWidth(2);

    TCanvas *c1 = new TCanvas("c1","c1",800,600);
    h1_mandelstam_t_data->Draw();
    h1_mandelstam_t_MC->Draw("SAME");
    c1->SaveAs(pdfOut_base.Data()+TString("mandelstam_t.pdf"));


    //create histogram for h1_mandelstam_t_data to h1_mandelstam_t_MC ratio
    TH1F *h1_mandelstam_t_ratio = (TH1F*)h1_mandelstam_t_data->Clone();
    h1_mandelstam_t_ratio->Divide(h1_mandelstam_t_MC);
    h1_mandelstam_t_ratio->SetTitle("Mandelstam t ratio");
    h1_mandelstam_t_ratio->GetXaxis()->SetTitle("Mandelstam t [GeV^{2}]");
    h1_mandelstam_t_ratio->GetYaxis()->SetTitle("Ratio");
    h1_mandelstam_t_ratio->SetStats(0);
    h1_mandelstam_t_ratio->SetLineColor(kRed);
    h1_mandelstam_t_ratio->SetLineWidth(2);

    //fit ratio with polynomial function and print as pdf
    // TF1 *fit_mandelstam_t_ratio = new TF1("fit_mandelstam_t_ratio","pol2",-1.0,1.0);
    // h1_mandelstam_t_ratio->Fit(fit_mandelstam_t_ratio,"R");
    h1_mandelstam_t_ratio->Draw();
    // fit_mandelstam_t_ratio->Draw("SAME");
    c1->SaveAs(pdfOut_base.Data()+TString("mandelstam_t_ratio.pdf"));

    //create output file
    TFile *fileOutput = new TFile(outputDir+fileOutputName,"RECREATE");

    h1_mandelstam_t_data->Write();
    h1_mandelstam_t_MC->Write();
    h1_mandelstam_t_ratio->Write();

    if (isGenerateWeightedTrees) {
        
        //create weighted trees

        TTree *dataChain = (TTree*)fileInputMC->Get("nt");
        
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

        TTree *dataChainWeighted = dataChain->CloneTree(0);

        for (Int_t i=0; i<dataChain->GetEntries(); i++) {
            dataChain->GetEntry(i);
            weight = h1_mandelstam_t_ratio->GetBinContent(h1_mandelstam_t_ratio->FindBin(TMath::Abs((PzP1+PzP2+PzP3)*(PzP1+PzP2+PzP3))));
            dataChainWeighted->Fill();
        }
    }

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