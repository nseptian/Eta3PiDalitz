#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "IUAmpTools/FitResults.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TFitResult.h"

int main(int argc, char* argv[]){
    
    // gStyle->SetOptFit(0001);
    // gStyle->SetOptStat("ne");
    
    if (argc < 5){
        cout << "Usage:" << endl << endl;
        cout << "\tcalcStatUnc <fit results name> <first tag> <last tag> <fit? 0/1>" << endl << endl;
        return 0;
    }

    vector<double> par_A,par_B,par_C,par_D,par_E,par_F,par_G,par_H,par_L;

    // kFit used
    TH1F *hPar_A = new TH1F("h_Par_A","",50,1.02,1.12);
    TH1F *hPar_B = new TH1F("h_Par_B","",50,0.14,0.24);
    TH1F *hPar_C = new TH1F("h_Par_C","",50,-0.04,0.06);
    TH1F *hPar_D = new TH1F("h_Par_D","",50,0.025,0.125);
    TH1F *hPar_E = new TH1F("h_Par_E","",50,-0.05,0.05);
    TH1F *hPar_F = new TH1F("h_Par_F","",50,0.13,0.23);
    TH1F *hPar_G = new TH1F("h_Par_G","",50,-0.05,0.05);
    TH1F *hPar_H = new TH1F("h_Par_H","",50,-0.03,0.07);
    TH1F *hPar_L = new TH1F("h_Par_L","",50,-0.05,0.05);

    //07122022
    // TH1F *hPar_A = new TH1F("h_Par_A","",50,0.92,1.02);
    // TH1F *hPar_B = new TH1F("h_Par_B","",50,0.32,0.42);
    // TH1F *hPar_C = new TH1F("h_Par_C","",50,-0.04,0.06);
    // TH1F *hPar_D = new TH1F("h_Par_D","",50,-0.32,-0.22);
    // TH1F *hPar_E = new TH1F("h_Par_E","",50,-0.05,0.05);
    // TH1F *hPar_F = new TH1F("h_Par_F","",50,0.25,0.35);
    // TH1F *hPar_G = new TH1F("h_Par_G","",50,0.35,0.45);
    // TH1F *hPar_H = new TH1F("h_Par_H","",50,-0.05,0.05);
    // TH1F *hPar_L = new TH1F("h_Par_L","",50,-0.05,0.05);

    string fitResultsBaseName(argv[1]);
    int first_tag = stoi(argv[2]);
    int last_tag = stoi(argv[3]);
    int isFit = stoi(argv[4]);

    for (int i=first_tag;i<last_tag+1;i++){
        stringstream ss;
        string fitResultsName;
        ss << fitResultsBaseName << "_" << i << ".fit";
        ss >> fitResultsName;
        cout << fitResultsName << endl;
        FitResults results(fitResultsName);
        vector<string> parameterName = results.parNameList();
        // for (int j=0;j<parameterName.size();j++) cout << j << " " << parameterName[j] << endl;
        vector<double> parameterValues = results.parValueList();

        hPar_A->Fill(-1*parameterValues[3]);
        hPar_B->Fill(parameterValues[4]);
        hPar_C->Fill(parameterValues[5]);
        hPar_D->Fill(parameterValues[6]);
        hPar_E->Fill(parameterValues[7]);
        hPar_F->Fill(parameterValues[8]);
        hPar_G->Fill(parameterValues[9]);
        hPar_H->Fill(parameterValues[10]);
        hPar_L->Fill(parameterValues[11]);

        // cout << i << " " << parameterValues[3] << endl;

    }

    gStyle->SetOptStat(0);

    Double_t label_size = 0.04;
    Double_t title_size = 0.07;
    Double_t title_offset = 0.65;

    hPar_A->GetXaxis()->SetTitle("-a");
    hPar_A->GetYaxis()->SetTitle("count");
    hPar_A->GetXaxis()->SetTitleSize(title_size);
    hPar_A->GetXaxis()->SetLabelSize(label_size);
    hPar_A->GetYaxis()->SetTitleSize(title_size);
    hPar_A->GetYaxis()->SetLabelSize(label_size);
    hPar_A->GetXaxis()->SetTitleOffset(title_offset);
    hPar_A->GetYaxis()->SetTitleOffset(title_offset);
    
    // c0->Print("par_A_distribution.pdf");
    hPar_B->GetXaxis()->SetTitle("b");
    hPar_B->GetYaxis()->SetTitle("count");
    hPar_B->GetXaxis()->SetTitleSize(title_size);
    hPar_B->GetXaxis()->SetLabelSize(label_size);
    hPar_B->GetYaxis()->SetTitleSize(title_size);
    hPar_B->GetYaxis()->SetLabelSize(label_size);
    hPar_B->GetXaxis()->SetTitleOffset(title_offset);
    hPar_B->GetYaxis()->SetTitleOffset(title_offset);
    
    // c0->Print("par_B_distribution.pdf");
    hPar_C->GetXaxis()->SetTitle("c");
    hPar_C->GetYaxis()->SetTitle("count");
    hPar_C->GetXaxis()->SetTitleSize(title_size);
    hPar_C->GetXaxis()->SetLabelSize(label_size);
    hPar_C->GetYaxis()->SetTitleSize(title_size);
    hPar_C->GetYaxis()->SetLabelSize(label_size);
    hPar_C->GetXaxis()->SetTitleOffset(title_offset);
    hPar_C->GetYaxis()->SetTitleOffset(title_offset);
    
    // c0->Print("par_C_distribution.pdf");
    hPar_D->GetXaxis()->SetTitle("d");
    hPar_D->GetYaxis()->SetTitle("count");
    hPar_D->GetXaxis()->SetTitleSize(title_size);
    hPar_D->GetXaxis()->SetLabelSize(label_size);
    hPar_D->GetYaxis()->SetTitleSize(title_size);
    hPar_D->GetYaxis()->SetLabelSize(label_size);
    hPar_D->GetXaxis()->SetTitleOffset(title_offset);
    hPar_D->GetYaxis()->SetTitleOffset(title_offset);
    
    // c0->Print("par_D_distribution.pdf");
    hPar_E->GetXaxis()->SetTitle("e");
    hPar_E->GetYaxis()->SetTitle("count");
    hPar_E->GetXaxis()->SetTitleSize(title_size);
    hPar_E->GetXaxis()->SetLabelSize(label_size);
    hPar_E->GetYaxis()->SetTitleSize(title_size);
    hPar_E->GetYaxis()->SetLabelSize(label_size);
    hPar_E->GetXaxis()->SetTitleOffset(title_offset);
    hPar_E->GetYaxis()->SetTitleOffset(title_offset);
    
    // c0->Print("par_E_distribution.pdf");
    hPar_F->GetXaxis()->SetTitle("f");
    hPar_F->GetYaxis()->SetTitle("count");
    hPar_F->GetXaxis()->SetTitleSize(title_size);
    hPar_F->GetXaxis()->SetLabelSize(label_size);
    hPar_F->GetYaxis()->SetTitleSize(title_size);
    hPar_F->GetYaxis()->SetLabelSize(label_size);
    hPar_F->GetXaxis()->SetTitleOffset(title_offset);
    hPar_F->GetYaxis()->SetTitleOffset(title_offset);
    
    // c0->Print("par_F_distribution.pdf");
    hPar_G->GetXaxis()->SetTitle("g");
    hPar_G->GetYaxis()->SetTitle("count");
    hPar_G->GetXaxis()->SetTitleSize(title_size);
    hPar_G->GetXaxis()->SetLabelSize(label_size);
    hPar_G->GetYaxis()->SetTitleSize(title_size);
    hPar_G->GetYaxis()->SetLabelSize(label_size);
    hPar_G->GetXaxis()->SetTitleOffset(title_offset);
    hPar_G->GetYaxis()->SetTitleOffset(title_offset);
    
    // c0->Print("par_G_distribution.pdf");
    hPar_H->GetXaxis()->SetTitle("h");
    hPar_H->GetYaxis()->SetTitle("count");
    hPar_H->GetXaxis()->SetTitleSize(title_size);
    hPar_H->GetXaxis()->SetLabelSize(label_size);
    hPar_H->GetYaxis()->SetTitleSize(title_size);
    hPar_H->GetYaxis()->SetLabelSize(label_size);
    hPar_H->GetXaxis()->SetTitleOffset(title_offset);
    hPar_H->GetYaxis()->SetTitleOffset(title_offset);

    // c0->Print("par_H_distribution.pdf");
    hPar_L->GetXaxis()->SetTitle("l");
    hPar_L->GetYaxis()->SetTitle("count");
    hPar_L->GetXaxis()->SetTitleSize(title_size);
    hPar_L->GetXaxis()->SetLabelSize(label_size);
    hPar_L->GetYaxis()->SetTitleSize(title_size);
    hPar_L->GetYaxis()->SetLabelSize(label_size);
    hPar_L->GetXaxis()->SetTitleOffset(title_offset);
    hPar_L->GetYaxis()->SetTitleOffset(title_offset);

    // c0->Print("par_L_distribution.pdf");

    if (isFit) {
        TFitResultPtr fitPar_A = hPar_A->Fit("gaus","S");
        TFitResultPtr fitPar_B = hPar_B->Fit("gaus","S");
        TFitResultPtr fitPar_C = hPar_C->Fit("gaus","S");
        TFitResultPtr fitPar_D = hPar_D->Fit("gaus","S");
        TFitResultPtr fitPar_E = hPar_E->Fit("gaus","S");
        TFitResultPtr fitPar_F = hPar_F->Fit("gaus","S");
        TFitResultPtr fitPar_G = hPar_G->Fit("gaus","S");
        TFitResultPtr fitPar_H = hPar_H->Fit("gaus","S");
        TFitResultPtr fitPar_L = hPar_L->Fit("gaus","S");

        cout << endl << "Mean (-a,b,c,d,e,f,g,h,l):" << endl << "{";
        cout << fitPar_A->Parameter(1) << ",";
        cout << fitPar_B->Parameter(1) << ",";
        cout << fitPar_C->Parameter(1) << ",";
        cout << fitPar_D->Parameter(1) << ",";
        cout << fitPar_E->Parameter(1) << ",";
        cout << fitPar_F->Parameter(1) << ",";
        cout << fitPar_G->Parameter(1) << ",";
        cout << fitPar_H->Parameter(1) << ",";
        cout << fitPar_L->Parameter(1) << "};" << endl;

        cout << endl << "Sigma (-a,b,c,d,e,f,g,h,l):" << endl << "{";
        cout << fitPar_A->Parameter(2) << ",";
        cout << fitPar_B->Parameter(2) << ",";
        cout << fitPar_C->Parameter(2) << ",";
        cout << fitPar_D->Parameter(2) << ",";
        cout << fitPar_E->Parameter(2) << ",";
        cout << fitPar_F->Parameter(2) << ",";
        cout << fitPar_G->Parameter(2) << ",";
        cout << fitPar_H->Parameter(2) << ",";
        cout << fitPar_L->Parameter(2) << "};" << endl;
    }

    TCanvas *c0 = new TCanvas("c0","",2000,1200);
    c0->Divide(3,3,0.00000001,0.0001);
    c0->cd(1);
    hPar_A->Draw();
    c0->cd(2);
    hPar_B->Draw();
    c0->cd(3);
    hPar_C->Draw();
    c0->cd(4);
    hPar_D->Draw();
    c0->cd(5);
    hPar_E->Draw();
    c0->cd(6);
    hPar_F->Draw();
    c0->cd(7);
    hPar_G->Draw();
    c0->cd(8);
    hPar_H->Draw();
    c0->cd(9);
    hPar_L->Draw();
    TString pdfNameDalitzParDist = "DalitzParDistribution_";
    pdfNameDalitzParDist += fitResultsBaseName;
    pdfNameDalitzParDist += ".pdf";
    c0->SaveAs(pdfNameDalitzParDist);

}