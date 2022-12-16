#include <iostream>

#include "TSystem.h"
#include "TH2F.h"
#include "TF2.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TColor.h"
#include "TStyle.h"

using namespace std;

void gluex_style() {
	///////// 
	// Make the plots prettier with bigger fonts
	// Original code is https://halldweb.jlab.org/wiki-private/index.php/Guidance_for_Presentations 
	// 	slightly modified
	/////////
	TStyle *gluex_style = new TStyle("GlueX","Default GlueX Style");

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
    // return par[0] + par[1]*Y + par[2]*X*Y + par[3]*TMath::Power(X,2) + par[4]*TMath::Power(Y,2) + par[5]*Y*TMath::Power(X,2);
    // return par[0] + par[1]*X + par[2]*Y + par[3]*X*Y + par[4]*TMath::Power(X,2) + par[5]*TMath::Power(Y,2) + par[6]*Y*TMath::Power(X,2);
    return par[0] + par[1]*X + par[2]*Y + par[3]*X*Y + par[4]*TMath::Power(X,2) + par[5]*TMath::Power(Y,2) + par[6]*Y*TMath::Power(X,2) + par[7]*X*TMath::Power(Y,2);
}

Double_t polynomial2DX(Double_t *x, Double_t *par){
    Double_t X = x[0];
    Double_t Y = x[1];
    return par[0] + par[1]*X + par[2]*Y + par[3]*X*Y + par[4]*TMath::Power(X,2) + par[5]*TMath::Power(Y,2) + par[6]*Y*TMath::Power(X,2) + par[7]*X*TMath::Power(Y,2);
}

Double_t polynomial2DY(Double_t *x, Double_t *par){
    Double_t X = x[0];
    Double_t Y = x[1];
    return par[0] + par[1]*X + par[3]*X*Y + par[5]*TMath::Power(Y,2) + par[6]*Y*TMath::Power(X,2) + par[7]*X*TMath::Power(Y,2);
}

// Double_t GetGlobalBin(Int_t N, Double_t X, Double_t Y){\
    // const Double_t delta = 2.2/N;
//     const Double_t delta = 1;
//     return TMath::FloorNint((X+1.1)/delta) + N*TMath::FloorNint((Y+1.1)/delta);
// }

void fitXYResolution(TString fName = ""){
    gSystem->Exec("mkdir -p plots");

	gluex_style();
	gROOT->ForceStyle();

    TFile *f = TFile::Open(fName,"READ");
    TCanvas* c=new TCanvas("","",800,600);

    TH2I *h2NumberBinLabel = (TH2I*)f->Get("h2_BinNumber");

    TH2F *h2XBinnedSigma = (TH2F*)f->Get("h2_XBinnedSigma");
    // Double_t iniParamsX[10] = {0.03,0.0,7.,-0.05,1.5,0.05,0.1,0.5,0.8,0.3};
    // TF2 *fit2DFuncX = new TF2("funcX",double2DGaussian,-1.,1.,-1.,1.,10);

    // Double_t iniParamsX[15] = {0.03,0.0,7.,-0.05,1.5,0.05,0.1,0.5,0.8,0.3,-0.01,0.,1.0,-1.,0.8};
    // TF2 *fit2DFuncX = new TF2("funcX",triple2DGaussiana,-1.,1.,-1.,1.,15);
    
    Double_t iniParamsX[8] = {0.045,0.,-0.006,0.,-0.03,0.,-0.02,0.};
    TF2 *fit2DFuncX = new TF2("funcX",polynomial2D,-1.,1.,-1.,1.,8);

    fit2DFuncX->SetParameters(iniParamsX);
    h2XBinnedSigma->Draw("SURF2");
    c->SaveAs("Fittedh2XBinnedSigmaSurf2Polym.pdf");
    
    h2XBinnedSigma->Fit(fit2DFuncX,"E");
    cout << "Chi2/NDF of funcX = " << fit2DFuncX->GetChisquare()/fit2DFuncX->GetNDF() << endl;
    h2XBinnedSigma->Draw("COLZ");
    fit2DFuncX->Draw("CONT1 SAME");
    h2NumberBinLabel->Draw("TEXT SAME");
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

    Double_t iniParamsY[8] = {0.045,0.,-0.002,0.,0.002,-0.03,-0.015,0.001};
    TF2 *fit2DFuncY = new TF2("funcY",polynomial2D,-1.,1.,-1.,1.,8);

    fit2DFuncY->SetParameters(iniParamsY);
    h2YBinnedSigma->Draw("SURF2");
    c->SaveAs("Fittedh2YBinnedSigmaSurf2Polym.pdf");
    h2YBinnedSigma->Fit(fit2DFuncY,"E");
    cout << "Chi2/NDF of funcY = " << fit2DFuncY->GetChisquare()/fit2DFuncY->GetNDF() << endl;
    h2YBinnedSigma->Draw("COLZ");
    fit2DFuncY->Draw("CONT1 SAME");
    h2NumberBinLabel->Draw("TEXT SAME");
    c->SaveAs("Fittedh2YBinnedSigmaPolym.pdf");
    
    fit2DFuncY->Draw("SURF2");
    fit2DFuncY->GetXaxis()->SetTitle("X");
    fit2DFuncY->GetYaxis()->SetTitle("Y");
    c->SaveAs("FittedTF2YBinnedSigmaSurf2Polym.pdf");

    TH1F *h1_XResBinned = new TH1F("h1_XResBinned","X Resolution",286,0.5,286.5);
    TH1F *h1_YResBinned = new TH1F("h1_YResBinned","Y Resolution",286,0.5,286.5);

    Double_t arr_evalF2ResX[286], arr_evalF2ResY[286], arr_count[286];
    Int_t count = 0;

    for (size_t i = 0; i < 20; i++)
    {
        for (size_t j = 0; j < 20; j++)
        {
            if (h2NumberBinLabel->GetBinContent(i+1,j+1) != 0) {
                count++;
                h1_XResBinned->SetBinContent(count,h2XBinnedSigma->GetBinContent(i+1,j+1));
                h1_YResBinned->SetBinContent(count,h2YBinnedSigma->GetBinContent(i+1,j+1));
                arr_count[count-1] = count;
                Double_t X = (-9.5+i)/10;
                Double_t Y = (-9.5+j)/10;
                arr_evalF2ResX[count-1] = fit2DFuncX->Eval(X,Y);
                arr_evalF2ResY[count-1] = fit2DFuncY->Eval(X,Y);
                cout << count << ". " << X << " " << Y << " " << fit2DFuncX->Eval(X,Y) << " " << fit2DFuncY->Eval(X,Y)  << endl;
            }
        }
    }
    
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.035);

    double_t chi2fitX = fit2DFuncX->GetChisquare()/fit2DFuncX->GetNDF();
    TString chi2X = Form("#chi^{2}/ndf = %.2f",chi2fitX);
    // cout << "chi2fitX = " << chi2fitX << endl; 
    double_t chi2fitY = fit2DFuncY->GetChisquare()/fit2DFuncY->GetNDF();
    TString chi2Y = Form("#chi^{2}/ndf = %.2f",chi2fitY);
    // cout << "chi2fitY = " << chi2fitY << endl;

    h1_XResBinned->Draw();
    h1_XResBinned->GetXaxis()->SetTitle("Bin number");
    h1_XResBinned->GetYaxis()->SetTitle("#sigma_{X}");
    TGraph *gr1_f1X = new TGraph(286,arr_count,arr_evalF2ResX);
    gr1_f1X->Draw("SAME");
    gr1_f1X->SetLineColor(kRed);
    Tl.DrawLatex(20,0.048,chi2X);
    c->SaveAs("h1XResBinnedFit.pdf");

    h1_YResBinned->Draw();
    h1_YResBinned->GetXaxis()->SetTitle("Bin number");
    h1_YResBinned->GetYaxis()->SetTitle("#sigma_{Y}");
    TGraph *gr1_f1Y = new TGraph(286,arr_count,arr_evalF2ResY);
    gr1_f1Y->Draw("SAME");
    gr1_f1Y->SetLineColor(kRed);
    Tl.DrawLatex(20,0.015,chi2Y);
    c->SaveAs("h1YResBinnedFit.pdf");
}