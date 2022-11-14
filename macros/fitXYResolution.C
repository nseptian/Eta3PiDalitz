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

Double_t gauss2D(Double_t *x, Double_t *par){
    Double_t z1 = Double_t((x[0]-par[1])/par[2]);
    Double_t z2 = Double_t((x[1]-par[3])/par[4]);
    return par[0]*exp(-0.5*(z1*z1+z2*z2));
}

Double_t double2DGaussian(Double_t *x, Double_t *par) {
    return gauss2D(x,&par[0]) + gauss2D(x,&par[5]);
}

Double_t triple2DGaussian(Double_t *x, Double_t *par) {
    return gauss2D(x,&par[0]) + gauss2D(x,&par[5]) + gauss2D(x,&par[10]);
}

Double_t polynomial2D(Double_t *x, Double_t *par){
    Double_t X = x[0];
    Double_t Y = x[1];
    return par[0] + par[1]*X + par[2]*Y + par[3]*X*Y + par[4]*TMath::Power(X,2) + par[5]*TMath::Power(Y,2) + par[6]*Y*TMath::Power(X,2) + par[7]*X*TMath::Power(Y,2);
}

void fitXYResolution(TString fName = ""){
    gSystem->Exec("mkdir -p plots");

	gluex_style();
	gROOT->ForceStyle();

    TFile *f = TFile::Open(fName,"READ");
    TCanvas* c=new TCanvas("","",800,600);

    TH2F *h2XBinnedSigma = (TH2F*)f->Get("h2_XBinnedSigma");
    // Double_t iniParamsX[10] = {0.03,0.0,7.,-0.05,1.5,0.05,0.1,0.5,0.8,0.3};
    // TF2 *fit2DFuncX = new TF2("funcX",double2DGaussian,-1.,1.,-1.,1.,10);

    // Double_t iniParamsX[15] = {0.03,0.0,7.,-0.05,1.5,0.05,0.1,0.5,0.8,0.3,-0.01,0.,1.0,-1.,0.8};
    // TF2 *fit2DFuncX = new TF2("funcX",triple2DGaussian,-1.,1.,-1.,1.,15);
    
    Double_t iniParamsX[8] = {1.,1.,1.,1.,1.,1.,1.,1.};
    TF2 *fit2DFuncX = new TF2("funcX",polynomial2D,-1.,1.,-1.,1.,8);

    fit2DFuncX->SetParameters(iniParamsX);
    h2XBinnedSigma->Draw("SURF2");
    c->SaveAs("Fittedh2XBinnedSigmaSurf2Polym.pdf");
    
    h2XBinnedSigma->Fit(fit2DFuncX,"W");
    h2XBinnedSigma->Draw("COLZ");
    fit2DFuncX->Draw("CONT1 SAME");
    c->SaveAs("Fittedh2XBinnedSigmaPolym.pdf");
    
    fit2DFuncX->Draw("SURF2");
    fit2DFuncX->GetXaxis()->SetTitle("X");
    fit2DFuncX->GetYaxis()->SetTitle("Y");
    c->SaveAs("FittedTF2XBinnedSigmaSurf2Polym.pdf");

    TH2F *h2YBinnedSigma = (TH2F*)f->Get("h2_YBinnedSigma");
    // Double_t iniParamsY[10] = {0.05,-2.,-2.,50,20.,0.05,0.05,10.,0.,1.};
    // TF2 *fit2DFuncY = new TF2("funcY",double2DGaussian,-1.,1.,-1.,1.,10);

    // Double_t iniParamsY[15] = {0.05,-2.,-2.,50,20.,0.05,0.05,10.,0.,1.,-0.01,0.,1.0,-1.,0.8};
    // TF2 *fit2DFuncY = new TF2("funcY",triple2DGaussian,-1.,1.,-1.,1.,15);

    Double_t iniParamsY[8] = {1.,1.,1.,1.,1.,1.,1.,1.};
    TF2 *fit2DFuncY = new TF2("funcY",polynomial2D,-1.,1.,-1.,1.,8);

    fit2DFuncY->SetParameters(iniParamsY);
    h2YBinnedSigma->Draw("SURF2");
    c->SaveAs("Fittedh2YBinnedSigmaSurf2Polym.pdf");
    h2YBinnedSigma->Fit(fit2DFuncY,"W");
    h2YBinnedSigma->Draw("COLZ");
    fit2DFuncY->Draw("CONT1 SAME");
    c->SaveAs("Fittedh2YBinnedSigmaPolym.pdf");
    
    fit2DFuncY->Draw("SURF2");
    fit2DFuncY->GetXaxis()->SetTitle("X");
    fit2DFuncY->GetYaxis()->SetTitle("Y");
    c->SaveAs("FittedTF2YBinnedSigmaSurf2Polym.pdf");
}