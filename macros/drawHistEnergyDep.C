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
	gluex_style->SetOptTitle(0);    // no title by default
	gluex_style->SetHistLineWidth(2); 
	gluex_style->SetNdivisions(508,"xyz"); // some ticks were very bunched, lets reduce the number of divisions to label 
	// gluex_style->SetOptFit(0111);

	gluex_style->SetHistFillColor(920);   // grey
	gluex_style->SetPalette(kViridis); // kViridis is perceptually uniform and colorblind friendly
	gluex_style->cd();
}

const TString outDir = "/d/home/septian/Eta3PiDalitzPlots/";

void drawTH1ScaledMC(TString histName, TH1F* h1, TH1F* h1MC, TString XAxisLabel, TString YAxisLabel, TString outputTag);

void drawHistEnergyDep(TString fileName = "", TString fileNameMC = "", TString inputTag = "", TString outputTag = ""){
	// gSystem->Exec("mkdir -p plots");

	gluex_style();
	gROOT->ForceStyle();	

	TString drawOptions="P";

	TFile* f=new TFile(fileName,"READ");
	TFile* fMC=new TFile(fileNameMC,"READ");
	TDirectory *dir = (TDirectory*)f->Get("hist");
	TDirectory *dirMC = (TDirectory*)fMC->Get("hist");
	TH1F* h1;
	TH1F* h1MC;
	TH2F* h2;
	TBox* box=new TBox();
	TCanvas* c=new TCanvas("","",800,600);
	TString hname;

	vector<bool>* pCutConfigBool = (vector<bool>*)f->Get("cutConfigBool");
	// vector<bool> cutConfigBool = *pCutConfigBool;

	// bool is_mc = pCutConfigBool->at(0);
	bool enableSidebandSubs = pCutConfigBool->at(1);

	hname="h_kFitProb";
	dir->GetObject(hname.Data(),h1);
	dirMC->GetObject(hname.Data(),h1MC);
	h1->Draw(drawOptions.Data());
	h1->GetXaxis()->SetTitle("kinematic fit probability");
	h1->GetYaxis()->SetTitle("counts");
	h1MC->Scale(h1->Integral()/h1MC->Integral());
	h1MC->Draw(Form("%s SAME",drawOptions.Data()));
	h1MC->SetLineColor(kRed);
	c->SaveAs((outDir+hname+"_"+outputTag+".pdf").Data());

	hname="h_pippimg1g2mass";
	dir->GetObject(hname.Data(),h1);
	dirMC->GetObject(hname.Data(),h1MC);
	drawTH1ScaledMC(hname,h1,h1MC,"M_{#pi^{+}#pi^{-}#gamma#gamma} (GeV)","counts",outputTag);

}

void drawTH1ScaledMC(TString histName, TH1F* h1, TH1F* h1MC, TString XAxisLabel, TString YAxisLabel, TString outputTag){
	TCanvas *ctemp = new TCanvas("ctemp","ctemp",800,600);

	h1MC->Scale(h1->Integral()/h1MC->Integral());
	h1MC->Draw("P");
	h1MC->SetLineColor(kRed);

	h1->Draw("P SAME");
	h1->GetXaxis()->SetTitle(XAxisLabel);
	h1->GetYaxis()->SetTitle(YAxisLabel);

	TLegend *legend = new TLegend(0.2,0.8,0.4,0.9);
	legend->AddEntry(h1,"GlueX-2017","l");
	legend->AddEntry(h1MC,"Signal MC (genEtaRegge)","l");
	legend->Draw();
	ctemp->SaveAs((outDir+histName+"_"+outputTag+".pdf").Data());
	delete ctemp;
}

/*
if(is_mc){
		hname = "h2_DalitzPlotEta3Pi_kin";
		f->GetObject(hname.Data(),h2);
		h2->GetXaxis()->SetTitle("X");
		h2->GetYaxis()->SetTitle("Y");
		h2->SetTitle("Dalitz plot reconstructed MC");
		h2->Draw("COLZ");
		h2->SetMinimum(0);
		c->SaveAs((outDir+hname+"_"+outputTag+".pdf").Data());

		hname = "h2_DalitzPlotEta3Pi_efficiency";
		f->GetObject(hname.Data(),h2);
		h2->GetXaxis()->SetTitle("X");
		h2->GetYaxis()->SetTitle("Y");
		h2->SetTitle("Dalitz plot efficiency");
		h2->Draw("COLZ");
		h2->SetMinimum(0.);
		h2->SetMaximum(1.);
		c->SaveAs((outDir+hname+"_"+outputTag+".pdf").Data());

		hname = "h1_XResolution";
		f->GetObject(hname.Data(),h1);
		TF1 *fXResolution = new TF1("f_XResolution", "gaus", -0.5, 0.5);
		h1->Fit(fXResolution,"R");
		h1->GetFunction("f_XResolution")->SetLineColor(kRed);
		h1->SetMarkerStyle(3);
		h1->GetXaxis()->SetTitle("X_{thrown} - X_{reconstructed}");
		h1->Draw();
		// h1->SetMinimum(0);
		c->SaveAs((outDir+hname+"_"+outputTag+".pdf").Data());

		hname = "h1_YResolution";
		f->GetObject(hname.Data(),h1);
		TF1 *fYResolution = new TF1("f_YResolution", "gaus", -0.5, 0.5);
		h1->Fit(fYResolution,"R");
		h1->GetFunction("f_YResolution")->SetLineColor(kRed);
		h1->SetMarkerStyle(3);
		h1->GetXaxis()->SetTitle("Y_{thrown} - Y_{reconstructed}");
		h1->Draw();
		// h1->SetMinimum(0);
		c->SaveAs((outDir+hname+"_"+outputTag+".pdf").Data());

		TDirectory *dirRes = (TDirectory*)f->Get("BinnedXYResolution");
		dirRes->cd();

		const Int_t XResBin = 40;
    	const Int_t YResBin = 40;

    	TH1F *hXRes[XResBin][YResBin],*hYRes[XResBin][YResBin];
		TF1 *g1 = new TF1("f_g1","gaus",-0.5,0.5);
   		TF1 *g2 = new TF1("f_g2","gaus",-0.5,0.5);

		TString fBinnedResolutionName = "BinnedResolution";
		fBinnedResolutionName += outputTag;
		fBinnedResolutionName += ".root";

		TFile *fBinnedResolution = new TFile(fBinnedResolutionName,"RECREATE");

		TH2F *h2NXYBinned = new TH2F("h2_NXYBinned","Number of entries on each bin",XResBin,-1.0,1.0,YResBin,-1.0,1.0);
		TH2F *h2NXYBinnedEntriesCut = new TH2F("h2_NXYBinnedEntriesCut","Number of entries on each bin",XResBin,-1.0,1.0,YResBin,-1.0,1.0);
		TH2F *h2XBinnedSigma = new TH2F("h2_XBinnedSigma","X Resolution",XResBin,-1.0,1.0,YResBin,-1.0,1.0);
		TH2F *h2YBinnedSigma = new TH2F("h2_YBinnedSigma","Y Resolution",XResBin,-1.0,1.0,YResBin,-1.0,1.0);
		TH2I *h2NumberBinLabel = new TH2I("h2_BinNumber","Bin number",XResBin,-1.0,1.0,YResBin,-1.0,1.0);

		gSystem->Exec(("mkdir -p plots/XYBinnedResolution_"+outputTag).Data());

		Int_t counter=0;

		for (Int_t i=0;i<XResBin;i++){
			for (Int_t j=0;j<YResBin;j++){
				TString hXResName = "h1_XResolution_";
          		hXResName += i;
          		hXResName += "_";
          		hXResName += j;

          		TString hYResName = "h1_YResolution_";
          		hYResName += i;
          		hYResName += "_";
          		hYResName += j;

				hXRes[i][j] = (TH1F*)dirRes->Get(hXResName.Data());
				hYRes[i][j] = (TH1F*)dirRes->Get(hYResName.Data());
				h2NXYBinned->SetBinContent(i+1,j+1,hXRes[i][j]->GetEntries());
				if ((hXRes[i][j]->GetEntries() > 500)){
					counter++;
					h2NumberBinLabel->SetBinContent(i+1,j+1,counter);
					h2NXYBinnedEntriesCut->SetBinContent(i+1,j+1,hXRes[i][j]->GetEntries());
					
					hXRes[i][j]->Fit(g1,"R");
					hXRes[i][j]->GetFunction("f_g1")->SetLineColor(kRed);
					hXRes[i][j]->SetMarkerStyle(3);
					hXRes[i][j]->GetXaxis()->SetTitle("X_{thrown} - X_{reconstructed}");
					hXRes[i][j]->GetYaxis()->SetTitle("count");
					hXRes[i][j]->Draw();
					hname = "XBinnedResolution";
					c->SaveAs(("plots/XYBinnedResolution_"+outputTag+"/"+hname+"_"+std::to_string(i)+"_"+std::to_string(j)+"_"+outputTag+".pdf").Data());					
					h2XBinnedSigma->SetBinContent(i+1,j+1,g1->GetParameter(2));
					h2XBinnedSigma->SetBinError(i+1,j+1,g1->GetParError(2));

					hYRes[i][j]->Fit(g2,"R");
					hYRes[i][j]->GetFunction("f_g2")->SetLineColor(kRed);
					hYRes[i][j]->SetMarkerStyle(3);
					hYRes[i][j]->GetXaxis()->SetTitle("Y_{thrown} - Y_{reconstructed}");
					hYRes[i][j]->GetYaxis()->SetTitle("count");
					hYRes[i][j]->Draw();
					hname = "YBinnedResolution";
					c->SaveAs(("plots/XYBinnedResolution_"+outputTag+"/"+hname+"_"+std::to_string(i)+"_"+std::to_string(j)+"_"+outputTag+".pdf").Data());
					h2YBinnedSigma->SetBinContent(i+1,j+1,g2->GetParameter(2));
					h2YBinnedSigma->SetBinError(i+1,j+1,g2->GetParError(2));

				}
			}
		}
		h2NXYBinned->GetXaxis()->SetTitle("X");
		h2NXYBinned->GetYaxis()->SetTitle("Y");
		h2NXYBinned->Draw("COLZ");
		hname = "h2NXYBinned";
		c->SaveAs((outDir+hname+"_"+outputTag+".pdf").Data());

		h2NXYBinnedEntriesCut->GetXaxis()->SetTitle("X");
		h2NXYBinnedEntriesCut->GetYaxis()->SetTitle("Y");
		h2NXYBinnedEntriesCut->Draw("COLZ");
		hname = "h2NXYBinnedEntriesCut";
		c->SaveAs((outDir+hname+"_"+outputTag+".pdf").Data());

		h2XBinnedSigma->GetXaxis()->SetTitle("X");
		h2XBinnedSigma->GetYaxis()->SetTitle("Y");
		h2XBinnedSigma->Draw("COLZ");
		hname = "h2XBinnedResolution";
		c->SaveAs((outDir+hname+"_"+outputTag+".pdf").Data());

		h2YBinnedSigma->GetXaxis()->SetTitle("X");
		h2YBinnedSigma->GetYaxis()->SetTitle("Y");
		h2YBinnedSigma->Draw("COLZ");
		hname = "h2YBinnedResolution";
		c->SaveAs((outDir+hname+"_"+outputTag+".pdf").Data());
		fBinnedResolution->Write();

	}
	else{
		if (enableSidebandSubs) {
			
			vector<Double_t>* vec = (vector<Double_t>*)f->Get("ranges");
			vector<Double_t> v = *vec;
	
			Double_t signalRange[2] = {v[0],v[1]};
			Double_t leftSidebandRange[2] = {v[2],v[3]};
			Double_t rightSidebandRange[2] = {v[4],v[5]};
			hname="h_pippimg1g2mass";
			f->GetObject(hname.Data(),h1);
			h1->Draw(drawOptions.Data());
			// h1->SetMinimum(0);
			box->SetFillColorAlpha(kRed+2,0.3);
			box->DrawBox(leftSidebandRange[0],0,leftSidebandRange[1],h1->GetMaximum()*1.05);
			box->DrawBox(rightSidebandRange[0],0,rightSidebandRange[1],h1->GetMaximum()*1.05);
			box->SetFillColorAlpha(kGreen+2,0.3);
			box->DrawBox(signalRange[0],0,signalRange[1],h1->GetMaximum()*1.05);
			c->SaveAs((outDir+hname+"_"+outputTag+".pdf").Data());
	
			hname="h_pippimg1g2mass_sbsAll";
			f->GetObject(hname.Data(),h1);
			h1->Draw(drawOptions.Data());
			// h1->SetMinimum(0);
			box->SetFillColorAlpha(kRed+2,0.3);
			box->DrawBox(leftSidebandRange[0],h1->GetMinimum()*1.05,leftSidebandRange[1],h1->GetMaximum()*1.05);
			box->DrawBox(rightSidebandRange[0],h1->GetMinimum()*1.05,rightSidebandRange[1],h1->GetMaximum()*1.05);
			box->SetFillColorAlpha(kGreen+2,0.3);
			box->DrawBox(signalRange[0],h1->GetMinimum()*1.05,signalRange[1],h1->GetMaximum()*1.05);
			c->SaveAs((outDir+hname+"_"+outputTag+".pdf").Data());
	
			hname="h_g1g2mass";
			f->GetObject(hname.Data(),h1);
			h1->GetXaxis()->SetTitle("M_{#gamma#gamma} (GeV)");
			h1->Draw(drawOptions.Data());
			// h1->SetMinimum(0);
			c->SaveAs((outDir+hname+"_"+outputTag+".pdf").Data());
	
			hname="h_g1g2mass_sbsAll";
			f->GetObject(hname.Data(),h1);
			h1->GetXaxis()->SetTitle("M_{#gamma#gamma} (GeV)");
			h1->Draw(drawOptions.Data());
			// h1->SetMinimum(0);
			c->SaveAs((outDir+hname+"_"+outputTag+".pdf").Data());
	
			hname="h_g1g2mass_sbsSidebandOnlyUnweighted";
			f->GetObject(hname.Data(),h1);
			h1->GetXaxis()->SetTitle("M_{#gamma#gamma} (GeV)");
			h1->Draw(drawOptions.Data());
			// h1->SetMinimum(0);
			c->SaveAs((outDir+hname+"_"+outputTag+".pdf").Data());
	
			hname="h_g1g2mass_sbsSidebandOnlyWeighted";
			f->GetObject(hname.Data(),h1);
			TH1F* h1_0 = new TH1F(*h1);
			h1->Draw(drawOptions.Data());
			// h1->SetMinimum(0);
			c->SaveAs((outDir+hname+"_"+outputTag+".pdf").Data());
	
			hname="h_g1g2mass_sbsSignalOnly";
			f->GetObject(hname.Data(),h1);
			TH1F* h1_1 = new TH1F(*h1);
			h1->Draw(drawOptions.Data());
			// h1->SetMinimum(0);
			c->SaveAs((outDir+hname+"_"+outputTag+".pdf").Data());
	
			hname = "h_g1g2mass_sbsSignalOnlySidebandOnlyWeighted";
			h1_0->Draw(drawOptions.Data());
			h1_0->SetFillColor(kRed);
			h1_1->Draw("HISTSAME");
			h1_0->GetYaxis()->SetRangeUser(h1_0->GetMinimum()*1.05,h1_1->GetMaximum()*1.05);
			// h1_0->GetYaxis()->SetRangeUser(-2100.,25000.);
	
			auto legend = new TLegend(0.25,0.75,0.48,0.9);
			legend->AddEntry(h1_0,"Sideband only (weighted)","f");
			legend->AddEntry(h1_1,"Signal only","f");
			legend->Draw();
			c->SaveAs((outDir+hname+"_"+outputTag+".pdf").Data());
	
			hname = "h2_DalitzPlotEta3Pi_sbsSidebandOnlyUnweighted";
			f->GetObject(hname.Data(),h2);
			h2->GetXaxis()->SetTitle("X");
			h2->GetYaxis()->SetTitle("Y");
			h2->SetTitle("Dalitz plot");
			h2->Draw("COLZ");
			h2->SetMinimum(0);
			c->SaveAs((outDir+hname+"_"+outputTag+".pdf").Data());
	
			hname = "h2_DalitzPlotEta3Pi_sbsAll";
			f->GetObject(hname.Data(),h2);
			h2->GetXaxis()->SetTitle("X");
			h2->GetYaxis()->SetTitle("Y");
			h2->SetTitle("Dalitz plot");
			h2->Draw("COLZ");
			h2->SetMinimum(0);
			c->SaveAs((outDir+hname+"_"+outputTag+".pdf").Data());
	
			hname = "h2_DalitzPlotEta3Pi_sbsSignalOnly";
			f->GetObject(hname.Data(),h2);
			h2->GetXaxis()->SetTitle("X");
			h2->GetYaxis()->SetTitle("Y");
			h2->SetTitle("Dalitz plot");
			h2->Draw("COLZ");
			h2->SetMinimum(0);
			c->SaveAs((outDir+hname+"_"+outputTag+".pdf").Data());
	
			hname = "h2_DalitzPlotEta3Pi_tree";
			f->GetObject(hname.Data(),h2);
			h2->GetXaxis()->SetTitle("X");
			h2->GetYaxis()->SetTitle("Y");
			h2->SetTitle("Dalitz plot");
			h2->Draw("COLZ");
			h2->SetMinimum(0);
			c->SaveAs((outDir+hname+"_"+outputTag+".pdf").Data());
	
			// c->SetCanvasSize(1200,600);
			// RooPlot *rp = (RooPlot*)f->Get("frame_h_pippimg1g2massRooFit_65274a0");
			// rp->SetStats(0);
			// // rp->Print();
			// TPaveText *paramBox = (TPaveText*)rp->getObject(5);
			// paramBox->Print();
			// paramBox->Clear();
			// paramBox->SetX1(0.0);
			// paramBox->SetX2(0.0);
			// paramBox->SetY1(0.0);
			// paramBox->SetY2(0.0);
			// rp->Draw();
			// box->SetFillColorAlpha(kRed+2,0.3);
			// box->DrawBox(leftSidebandRange[0],0,leftSidebandRange[1],rp->GetMaximum());
			// box->DrawBox(rightSidebandRange[0],0,rightSidebandRange[1],rp->GetMaximum());
			// box->SetFillColorAlpha(kGreen+2,0.3);
			// box->DrawBox(signalRange[0],0,signalRange[1],rp->GetMaximum());
			// c->SaveAs(("plots/hfit_"+outputTag+".pdf").Data());
		}
	}
	*/