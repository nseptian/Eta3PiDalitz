// Description: Reweight MC reco to match data t slope

const Int_t BeamEnergyBin = 0;

TString inputDir = "/d/home/septian/Eta3PiDalitz/run/root4AmptoolsEnergyDep/";
TString fileInputName = Form("eta_2017_data_t01506_ccdbFlux_EgBin_genEtaRegge_%d.root",BeamEnergyBin);
TString fileInputNameMC = Form("mc_rec_2017_data_t01506_ccdbFlux_EgBin_genEtaRegge_%d.root",BeamEnergyBin);

TString outputDir = "/d/home/septian/Eta3PiDalitz/run/root4AmptoolsEnergyDep/";
TString fileOutputName = Form("reweighted_mc_rec_2017_data_t01506_ccdbFlux_EgBin_genEtaRegge_%d.root",BeamEnergyBin);

TString pdfOutDir = "/d/home/septian/Eta3PiDalitzPlots/";
TString pdfOut_base = Form("2017_data_t01506_ccdbFlux_EgBin_genEtaRegge_%d",BeamEnergyBin);

Bool_t isGenerateWeightedTrees = kTRUE;

void gluex_style();
void ReweightMCRec(){

    gluex_style();
	gROOT->ForceStyle();
    //open input file
    TFile *fileInput = new TFile(inputDir+fileInputName);
    TFile *fileInputMC = new TFile(inputDir+fileInputNameMC);

    TDirectory *dir = (TDirectory*)fileInput->Get("hist_SidebandSub");
    TDirectory *dirMC = (TDirectory*)fileInputMC->Get("hist_SidebandSub");

    //get mandelstam_t histograms
    TString hname = "h_mandelstam_t_sbsAll";
    TH1F *h1_mandelstam_t_data = (TH1F*)dir->Get(hname.Data());
    TH1F *h1_mandelstam_t_MC = (TH1F*)dirMC->Get(hname.Data());
    
    //scale h1_mandelstam_t_MC to match h1_mandelstam_t_data
    h1_mandelstam_t_MC->Scale(h1_mandelstam_t_data->Integral()/h1_mandelstam_t_MC->Integral());
    
    //set histogram properties
    h1_mandelstam_t_data->SetTitle("Mandelstam t");
    h1_mandelstam_t_data->GetXaxis()->SetTitle("|t| [GeV^{2}]");
    h1_mandelstam_t_data->GetYaxis()->SetTitle("Events");
    h1_mandelstam_t_data->SetStats(0);
    h1_mandelstam_t_data->SetLineColor(kRed);
    h1_mandelstam_t_data->SetLineWidth(2);
    h1_mandelstam_t_MC->SetLineColor(kBlue);
    h1_mandelstam_t_MC->SetLineWidth(2);
    h1_mandelstam_t_MC->SetTitle("Mandelstam t");

    TCanvas *c1 = new TCanvas("c1","c1",800,600);
    h1_mandelstam_t_data->Draw();
    h1_mandelstam_t_MC->Draw("SAME");
    TLegend *leg1 = new TLegend(0.6,0.7,0.85,0.85);
    leg1->AddEntry(h1_mandelstam_t_data,"Data","l");
    leg1->AddEntry(h1_mandelstam_t_MC,"MC","l");
    leg1->Draw();
    c1->SaveAs((pdfOutDir+pdfOut_base).Data()+TString("_mandelstam_t.pdf"));


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
    TF1 *fit_mandelstam_t_ratio = new TF1("fit_mandelstam_t_ratio","pol4",0.15,0.6);
    h1_mandelstam_t_ratio->Fit(fit_mandelstam_t_ratio,"R");
    h1_mandelstam_t_ratio->Draw();
    fit_mandelstam_t_ratio->Draw("SAME");
    TLegend *leg2 = new TLegend(0.6,0.7,0.85,0.85);
    leg2->AddEntry(h1_mandelstam_t_ratio,"Ratio = Data/MC","l");
    leg2->AddEntry(fit_mandelstam_t_ratio,"Fit","l");
    c1->SaveAs((pdfOutDir+pdfOut_base).Data()+TString("_mandelstam_t_ratio.pdf"));

    //create output file
    TFile *fileOutput = new TFile(outputDir+fileOutputName,"RECREATE");

    // h1_mandelstam_t_data->Write();
    // h1_mandelstam_t_MC->Write();
    h1_mandelstam_t_ratio->SetName("h_mandelstam_t_ratio");
    h1_mandelstam_t_ratio->Write();

    if (isGenerateWeightedTrees) {
        
        //create weighted trees

        TTree *dataChain = (TTree*)fileInputMC->Get("nt");
        
        Double_t EnBeam,PxBeam,PyBeam,PzBeam;
        Double_t EnP1,PxP1,PyP1,PzP1;
        Double_t EnP2,PxP2,PyP2,PzP2;
        Double_t EnP3,PxP3,PyP3,PzP3;
        Double_t weight;

        dataChain->SetBranchAddress("EnBeam",&EnBeam);
        dataChain->SetBranchAddress("PxBeam",&PxBeam);
        dataChain->SetBranchAddress("PyBeam",&PyBeam);
        dataChain->SetBranchAddress("PzBeam",&PzBeam);

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

        //t histogram for unweighted events
        TH1F *h1_mandelstam_t_unweighted = new TH1F("h1_mandelstam_t_unweighted","Unweighted mandelstam t",100,0.15,0.6);
        h1_mandelstam_t_unweighted->GetXaxis()->SetTitle("|t| [GeV^{2}]");
        h1_mandelstam_t_unweighted->GetYaxis()->SetTitle("Events");

        //t histogram for weighted events
        TH1F *h1_mandelstam_t_weighted = new TH1F("h1_mandelstam_t_weighted","Weighted mandelstam t",100,0.15,0.6);
        h1_mandelstam_t_weighted->GetXaxis()->SetTitle("|t| [GeV^{2}]");
        h1_mandelstam_t_weighted->GetYaxis()->SetTitle("Events");

        for (Int_t i=0; i<dataChain->GetEntries(); i++) {
            dataChain->GetEntry(i);
            TLorentzVector P1,P2,P3;
            P1.SetPxPyPzE(PxP1,PyP1,PzP1,EnP1);
            P2.SetPxPyPzE(PxP2,PyP2,PzP2,EnP2);
            P3.SetPxPyPzE(PxP3,PyP3,PzP3,EnP3);
            TLorentzVector P4Eta = P1+P2+P3;

            TLorentzVector P4Beam = TLorentzVector(PxBeam,PyBeam,PzBeam,EnBeam);

            Double_t mandelstam_t = -1*(P4Beam-P4Eta).M2();
            h1_mandelstam_t_unweighted->Fill(mandelstam_t,weight);

            weight *= fit_mandelstam_t_ratio->Eval(mandelstam_t);
            // cout << "weight: " << fit_mandelstam_t_ratio->Eval(mandelstam_t) << endl;
            dataChainWeighted->Fill();
            h1_mandelstam_t_weighted->Fill(mandelstam_t,weight);
        }
        dataChainWeighted->Write();
        h1_mandelstam_t_unweighted->Write();
        h1_mandelstam_t_weighted->Write();
        h1_mandelstam_t_weighted->SetMinimum(0);
        h1_mandelstam_t_weighted->SetLineColor(kRed);
        h1_mandelstam_t_weighted->SetTitle("Weighted and unweighted mandelstam t");
        h1_mandelstam_t_weighted->Draw("");
        // h1_mandelstam_t_unweighted->SetMarkerStyle(2);
        h1_mandelstam_t_unweighted->Draw("E SAME");
        // h1_mandelstam_t_data->Draw("SAME");
        TLegend *leg3 = new TLegend(0.6,0.7,0.85,0.85);
        leg3->AddEntry(h1_mandelstam_t_unweighted,"Unweighted","l");
        leg3->AddEntry(h1_mandelstam_t_weighted,"Weighted","l");
        leg3->Draw();
        c1->SaveAs((pdfOutDir+pdfOut_base).Data()+TString("_mandelstam_t_weighted.pdf"));
    }
    fileOutput->Close();

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