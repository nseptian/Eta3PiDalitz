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

	gluex_style->SetHistFillColor(920);   // grey
	//gluex_style->SetPalette(kViridis); // kViridis is perceptually uniform and colorblind friendly
	gluex_style->cd();
}

void drawHist(){
	/////////////////////////
	// Load the histograms from the histogram root file and draw 
	// 	some shaded boxes / curves to denote the selection.
	// 	For all other variables, simply loop over them and
	// 	draw them all
	/////////////////////////
	gSystem->Exec("mkdir -p plots");	

	gluex_style();
	gROOT->ForceStyle();	

	string drawOptions="HIST";

	TFile* f=new TFile("root4Amptools/eta_2017_data_sbs_testNew_10092022.root");
	TH1F* h1;
	TH2F* h2;
	TBox* box=new TBox();
	TCanvas* c=new TCanvas("","",800,600);
	string hname;

	vector<Double_t>* vec = (vector<Double_t>*)f->Get("ranges");
	vector<Double_t> v = *vec;

	Double_t signalRange[2] = {v[0],v[1]};
	Double_t leftSidebandRange[2] = {v[2],v[3]};
	Double_t rightSidebandRange[2] = {v[4],v[5]};

	hname="h_pippimg1g2mass";
	f->GetObject(hname.c_str(),h1);
	h1->Draw(drawOptions.c_str());
	// h1->SetMinimum(0);
	box->SetFillColorAlpha(kRed+2,0.3);
	box->DrawBox(leftSidebandRange[0],0,leftSidebandRange[1],h1->GetMaximum()*1.05);
	box->DrawBox(rightSidebandRange[0],0,rightSidebandRange[1],h1->GetMaximum()*1.05);
	box->SetFillColorAlpha(kGreen+2,0.3);
	box->DrawBox(signalRange[0],0,signalRange[1],h1->GetMaximum()*1.05);
	c->SaveAs(("plots/"+hname+"_2017_sbs_testNew.pdf").c_str());

	hname="h_pippimg1g2mass_sbsAll";
	f->GetObject(hname.c_str(),h1);
	h1->Draw(drawOptions.c_str());
	// h1->SetMinimum(0);
	box->SetFillColorAlpha(kRed+2,0.3);
	box->DrawBox(leftSidebandRange[0],h1->GetMinimum()*1.05,leftSidebandRange[1],h1->GetMaximum()*1.05);
	box->DrawBox(rightSidebandRange[0],h1->GetMinimum()*1.05,rightSidebandRange[1],h1->GetMaximum()*1.05);
	box->SetFillColorAlpha(kGreen+2,0.3);
	box->DrawBox(signalRange[0],h1->GetMinimum()*1.05,signalRange[1],h1->GetMaximum()*1.05);
	c->SaveAs(("plots/"+hname+"_2017_sbs_testNew.pdf").c_str());

	hname="h_g1g2mass";
	f->GetObject(hname.c_str(),h1);
	h1->GetXaxis()->SetTitle("M_{#gamma#gamma} (GeV)");
	h1->Draw(drawOptions.c_str());
	// h1->SetMinimum(0);
	c->SaveAs(("plots/"+hname+"_2017_sbs_testNew.pdf").c_str());

	hname="h_g1g2mass_sbsAll";
	f->GetObject(hname.c_str(),h1);
	h1->GetXaxis()->SetTitle("M_{#gamma#gamma} (GeV)");
	h1->Draw(drawOptions.c_str());
	// h1->SetMinimum(0);
	c->SaveAs(("plots/"+hname+"_2017_sbs_testNew.pdf").c_str());

	hname="h_g1g2mass_sbsSidebandOnlyUnweighted";
	f->GetObject(hname.c_str(),h1);
	h1->GetXaxis()->SetTitle("M_{#gamma#gamma} (GeV)");
	h1->Draw(drawOptions.c_str());
	// h1->SetMinimum(0);
	c->SaveAs(("plots/"+hname+"_2017_sbs_testNew.pdf").c_str());

	hname="h_g1g2mass_sbsSidebandOnlyWeighted";
	f->GetObject(hname.c_str(),h1);
	TH1F* h1_0 = new TH1F(*h1);
	h1->Draw(drawOptions.c_str());
	// h1->SetMinimum(0);
	c->SaveAs(("plots/"+hname+"_2017_sbs_testNew.pdf").c_str());

	hname="h_g1g2mass_sbsSignalOnly";
	f->GetObject(hname.c_str(),h1);
	TH1F* h1_1 = new TH1F(*h1);
	h1->Draw(drawOptions.c_str());
	// h1->SetMinimum(0);
	c->SaveAs(("plots/"+hname+"_2017_sbs_testNew.pdf").c_str());

	hname = "h_g1g2mass_sbsSignalOnlySidebandOnlyWeighted";
	h1_0->Draw(drawOptions.c_str());
	h1_0->SetFillColor(kRed);
	h1_1->Draw("HISTSAME");
	h1_0->GetYaxis()->SetRangeUser(h1_0->GetMinimum()*1.05,h1_1->GetMaximum()*1.05);
	// h1_0->GetYaxis()->SetRangeUser(-2100.,25000.);

	auto legend = new TLegend(0.25,0.75,0.48,0.9);
	legend->AddEntry(h1_0,"Sideband only (weighted)","f");
	legend->AddEntry(h1_1,"Signal only","f");
   	legend->Draw();
	c->SaveAs(("plots/"+hname+"_2017_sbs_testNew.pdf").c_str());

	hname = "h2_DalitzPlotEta3Pi_sbsSidebandOnlyUnweighted";
	f->GetObject(hname.c_str(),h2);
	h2->GetXaxis()->SetTitle("X");
	h2->GetYaxis()->SetTitle("Y");
	h2->SetTitle("Dalitz plot");
	h2->Draw("COLZ");
	h2->SetMinimum(0);
	c->SaveAs(("plots/"+hname+"_2017_sbs_testNew.pdf").c_str());

	hname = "h2_DalitzPlotEta3Pi_sbsAll";
	f->GetObject(hname.c_str(),h2);
	h2->GetXaxis()->SetTitle("X");
	h2->GetYaxis()->SetTitle("Y");
	h2->SetTitle("Dalitz plot");
	h2->Draw("COLZ");
	h2->SetMinimum(0);
	c->SaveAs(("plots/"+hname+"_2017_sbs_testNew.pdf").c_str());

	hname = "h2_DalitzPlotEta3Pi_sbsSignalOnly";
	f->GetObject(hname.c_str(),h2);
	h2->GetXaxis()->SetTitle("X");
	h2->GetYaxis()->SetTitle("Y");
	h2->SetTitle("Dalitz plot");
	h2->Draw("COLZ");
	h2->SetMinimum(0);
	c->SaveAs(("plots/"+hname+"_2017_sbs_testNew.pdf").c_str());

	c->SetCanvasSize(1200,600);
	RooPlot *rp = (RooPlot*)f->Get("frame_h_pippimg1g2massRooFit_46b37a0");
	rp->SetStats(0);
	rp->Print();
	// TPaveText::gauspoly_paramBox *paramBox = (TPaveText::gauspoly_paramBox*)rp->FindObject()
	// rp->getAttText("gauspoly_paramBox")
	TPaveText *paramBox = (TPaveText*)rp->getObject(5);
	paramBox->Print();
	paramBox->Clear();
	paramBox->SetX1(0.0);
	paramBox->SetX2(0.0);
	paramBox->SetY1(0.0);
	paramBox->SetY2(0.0);
	rp->Draw();
	box->SetFillColorAlpha(kRed+2,0.3);
	box->DrawBox(leftSidebandRange[0],0,leftSidebandRange[1],rp->GetMaximum());
	box->DrawBox(rightSidebandRange[0],0,rightSidebandRange[1],rp->GetMaximum());
	box->SetFillColorAlpha(kGreen+2,0.3);
	box->DrawBox(signalRange[0],0,signalRange[1],rp->GetMaximum());
	c->SaveAs("plots/hfit_2017_sbs_testNew.pdf");

	

}