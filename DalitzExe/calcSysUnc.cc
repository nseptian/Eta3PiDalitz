#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "IUAmpTools/FitResults.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TStyle.h"

int main(int argc, char* argv[]){
    
    // gStyle->SetOptStat(11);
    // gStyle->SetOptStat("ne");
    
    if (argc < 5){
        cout << "Usage:" << endl << endl;
        cout << "\tcalcSysUnc <fit results name> <first tag> <last tag> <fit? 0/1>" << endl << endl;
        return 0;
    }

    vector<double> par_A,par_B,par_C,par_D,par_E,par_F,par_G,par_H,par_L;

    // kFit used 05092022
    TH1F *hPar_A = new TH1F("h_Par_A","",50,1.02,1.12);
    TH1F *hPar_B = new TH1F("h_Par_B","",50,0.14,0.24);
    TH1F *hPar_C = new TH1F("h_Par_C","",50,-0.04,0.06);
    TH1F *hPar_D = new TH1F("h_Par_D","",50,0.025,0.125);
    TH1F *hPar_E = new TH1F("h_Par_E","",50,-0.05,0.05);
    TH1F *hPar_F = new TH1F("h_Par_F","",50,0.13,0.23);
    TH1F *hPar_G = new TH1F("h_Par_G","",50,-0.05,0.05);
    TH1F *hPar_H = new TH1F("h_Par_H","",50,-0.03,0.07);
    TH1F *hPar_L = new TH1F("h_Par_L","",50,-0.05,0.05);

    TH1F *hPar_A_2 = new TH1F("h_Par_A_2","",50,1.02,1.12);
    TH1F *hPar_B_2 = new TH1F("h_Par_B_2","",50,0.14,0.24);
    TH1F *hPar_C_2 = new TH1F("h_Par_C_2","",50,-0.04,0.06);
    TH1F *hPar_D_2 = new TH1F("h_Par_D_2","",50,0.025,0.125);
    TH1F *hPar_E_2 = new TH1F("h_Par_E_2","",50,-0.05,0.05);
    TH1F *hPar_F_2 = new TH1F("h_Par_F_2","",50,0.13,0.23);
    TH1F *hPar_G_2 = new TH1F("h_Par_G_2","",50,-0.05,0.05);
    TH1F *hPar_H_2 = new TH1F("h_Par_H_2","",50,-0.03,0.07);
    TH1F *hPar_L_2 = new TH1F("h_Par_L_2","",50,-0.05,0.05);

    TH1F *hPar_A_3 = new TH1F("h_Par_A_3","",50,1.02,1.12);
    TH1F *hPar_B_3 = new TH1F("h_Par_B_3","",50,0.14,0.24);
    TH1F *hPar_C_3 = new TH1F("h_Par_C_3","",50,-0.04,0.06);
    TH1F *hPar_D_3 = new TH1F("h_Par_D_3","",50,0.025,0.125);
    TH1F *hPar_E_3 = new TH1F("h_Par_E_3","",50,-0.05,0.05);
    TH1F *hPar_F_3 = new TH1F("h_Par_F_3","",50,0.13,0.23);
    TH1F *hPar_G_3 = new TH1F("h_Par_G_3","",50,-0.05,0.05);
    TH1F *hPar_H_3 = new TH1F("h_Par_H_3","",50,-0.03,0.07);
    TH1F *hPar_L_3 = new TH1F("h_Par_L_3","",50,-0.05,0.05);

    // //07122022
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

    ifstream fitFile;
    int nanCounter=0;
    int importedCounter=0;
    int imported2Counter=0;
    int imported3Counter=0;

    for (int i=first_tag;i<last_tag+1;i++){
        stringstream ss;
        string fitResultsName;
        ss << fitResultsBaseName << "_" << i << ".fit";
        ss >> fitResultsName;
        
        // cout << fitResultsName << endl;
        fitFile.open(fitResultsName);
        if (fitFile){
            FitResults results(fitResultsName);
            cout << fitResultsName << " is imported. eMatrixStatus = " << results.eMatrixStatus() << ", lastMinuitCommandStatus = " << results.lastMinuitCommandStatus() << endl;
            importedCounter++;
            // if (results.eMatrixStatus() == 0) return 1;
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

            if (results.lastMinuitCommandStatus()==0) {
                hPar_A_2->Fill(-1*parameterValues[3]);
                hPar_B_2->Fill(parameterValues[4]);
                hPar_C_2->Fill(parameterValues[5]);
                hPar_D_2->Fill(parameterValues[6]);
                hPar_E_2->Fill(parameterValues[7]);
                hPar_F_2->Fill(parameterValues[8]);
                hPar_G_2->Fill(parameterValues[9]);
                hPar_H_2->Fill(parameterValues[10]);
                hPar_L_2->Fill(parameterValues[11]);
                imported2Counter++;
            }
            else if (results.lastMinuitCommandStatus()==4){
                hPar_A_3->Fill(-1*parameterValues[3]);
                hPar_B_3->Fill(parameterValues[4]);
                hPar_C_3->Fill(parameterValues[5]);
                hPar_D_3->Fill(parameterValues[6]);
                hPar_E_3->Fill(parameterValues[7]);
                hPar_F_3->Fill(parameterValues[8]);
                hPar_G_3->Fill(parameterValues[9]);
                hPar_H_3->Fill(parameterValues[10]);
                hPar_L_3->Fill(parameterValues[11]);
                imported3Counter++;
            }

            // if (results.eMatrixStatus()==2) {
            //     hPar_A_2->Fill(-1*parameterValues[3]);
            //     hPar_B_2->Fill(parameterValues[4]);
            //     hPar_C_2->Fill(parameterValues[5]);
            //     hPar_D_2->Fill(parameterValues[6]);
            //     hPar_E_2->Fill(parameterValues[7]);
            //     hPar_F_2->Fill(parameterValues[8]);
            //     hPar_G_2->Fill(parameterValues[9]);
            //     hPar_H_2->Fill(parameterValues[10]);
            //     hPar_L_2->Fill(parameterValues[11]);
            //     imported2Counter++;
            // }
            // else if (results.eMatrixStatus()==3){
            //     hPar_A_3->Fill(-1*parameterValues[3]);
            //     hPar_B_3->Fill(parameterValues[4]);
            //     hPar_C_3->Fill(parameterValues[5]);
            //     hPar_D_3->Fill(parameterValues[6]);
            //     hPar_E_3->Fill(parameterValues[7]);
            //     hPar_F_3->Fill(parameterValues[8]);
            //     hPar_G_3->Fill(parameterValues[9]);
            //     hPar_H_3->Fill(parameterValues[10]);
            //     hPar_L_3->Fill(parameterValues[11]);
            //     imported3Counter++;
            // }

            // cout << i << endl;

            // cout << i << " " << parameterValues[3] << endl;
        }
        else {
            cout << fitResultsName << " does not exist! Likelihood is nan." << endl;
            nanCounter++;
        }
        fitFile.close();
    }

    gStyle->SetOptStat(11);

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
    hPar_B->GetXaxis()->SetTitle("b");
    hPar_B->GetYaxis()->SetTitle("count");
    hPar_B->GetXaxis()->SetTitleSize(title_size);
    hPar_B->GetXaxis()->SetLabelSize(label_size);
    hPar_B->GetYaxis()->SetTitleSize(title_size);
    hPar_B->GetYaxis()->SetLabelSize(label_size);
    hPar_B->GetXaxis()->SetTitleOffset(title_offset);
    hPar_B->GetYaxis()->SetTitleOffset(title_offset);
    hPar_C->GetXaxis()->SetTitle("c");
    hPar_C->GetYaxis()->SetTitle("count");
    hPar_C->GetXaxis()->SetTitleSize(title_size);
    hPar_C->GetXaxis()->SetLabelSize(label_size);
    hPar_C->GetYaxis()->SetTitleSize(title_size);
    hPar_C->GetYaxis()->SetLabelSize(label_size);
    hPar_C->GetXaxis()->SetTitleOffset(title_offset);
    hPar_C->GetYaxis()->SetTitleOffset(title_offset);
    hPar_D->GetXaxis()->SetTitle("d");
    hPar_D->GetYaxis()->SetTitle("count");
    hPar_D->GetXaxis()->SetTitleSize(title_size);
    hPar_D->GetXaxis()->SetLabelSize(label_size);
    hPar_D->GetYaxis()->SetTitleSize(title_size);
    hPar_D->GetYaxis()->SetLabelSize(label_size);
    hPar_D->GetXaxis()->SetTitleOffset(title_offset);
    hPar_D->GetYaxis()->SetTitleOffset(title_offset);
    hPar_E->GetXaxis()->SetTitle("e");
    hPar_E->GetYaxis()->SetTitle("count");
    hPar_E->GetXaxis()->SetTitleSize(title_size);
    hPar_E->GetXaxis()->SetLabelSize(label_size);
    hPar_E->GetYaxis()->SetTitleSize(title_size);
    hPar_E->GetYaxis()->SetLabelSize(label_size);
    hPar_E->GetXaxis()->SetTitleOffset(title_offset);
    hPar_E->GetYaxis()->SetTitleOffset(title_offset);
    hPar_F->GetXaxis()->SetTitle("f");
    hPar_F->GetYaxis()->SetTitle("count");
    hPar_F->GetXaxis()->SetTitleSize(title_size);
    hPar_F->GetXaxis()->SetLabelSize(label_size);
    hPar_F->GetYaxis()->SetTitleSize(title_size);
    hPar_F->GetYaxis()->SetLabelSize(label_size);
    hPar_F->GetXaxis()->SetTitleOffset(title_offset);
    hPar_F->GetYaxis()->SetTitleOffset(title_offset);
    hPar_G->GetXaxis()->SetTitle("g");
    hPar_G->GetYaxis()->SetTitle("count");
    hPar_G->GetXaxis()->SetTitleSize(title_size);
    hPar_G->GetXaxis()->SetLabelSize(label_size);
    hPar_G->GetYaxis()->SetTitleSize(title_size);
    hPar_G->GetYaxis()->SetLabelSize(label_size);
    hPar_G->GetXaxis()->SetTitleOffset(title_offset);
    hPar_G->GetYaxis()->SetTitleOffset(title_offset);
    hPar_H->GetXaxis()->SetTitle("h");
    hPar_H->GetYaxis()->SetTitle("count");
    hPar_H->GetXaxis()->SetTitleSize(title_size);
    hPar_H->GetXaxis()->SetLabelSize(label_size);
    hPar_H->GetYaxis()->SetTitleSize(title_size);
    hPar_H->GetYaxis()->SetLabelSize(label_size);
    hPar_H->GetXaxis()->SetTitleOffset(title_offset);
    hPar_H->GetYaxis()->SetTitleOffset(title_offset);
    hPar_L->GetXaxis()->SetTitle("l");
    hPar_L->GetYaxis()->SetTitle("count");
    hPar_L->GetXaxis()->SetTitleSize(title_size);
    hPar_L->GetXaxis()->SetLabelSize(label_size);
    hPar_L->GetYaxis()->SetTitleSize(title_size);
    hPar_L->GetYaxis()->SetLabelSize(label_size);
    hPar_L->GetXaxis()->SetTitleOffset(title_offset);
    hPar_L->GetYaxis()->SetTitleOffset(title_offset);

    hPar_A_2->GetXaxis()->SetTitle("-a");
    hPar_A_2->GetYaxis()->SetTitle("count");
    hPar_A_2->GetXaxis()->SetTitleSize(title_size);
    hPar_A_2->GetXaxis()->SetLabelSize(label_size);
    hPar_A_2->GetYaxis()->SetTitleSize(title_size);
    hPar_A_2->GetYaxis()->SetLabelSize(label_size);
    hPar_A_2->GetXaxis()->SetTitleOffset(title_offset);
    hPar_A_2->GetYaxis()->SetTitleOffset(title_offset);
    hPar_B_2->GetXaxis()->SetTitle("b");
    hPar_B_2->GetYaxis()->SetTitle("count");
    hPar_B_2->GetXaxis()->SetTitleSize(title_size);
    hPar_B_2->GetXaxis()->SetLabelSize(label_size);
    hPar_B_2->GetYaxis()->SetTitleSize(title_size);
    hPar_B_2->GetYaxis()->SetLabelSize(label_size);
    hPar_B_2->GetXaxis()->SetTitleOffset(title_offset);
    hPar_B_2->GetYaxis()->SetTitleOffset(title_offset);
    hPar_C_2->GetXaxis()->SetTitle("c");
    hPar_C_2->GetYaxis()->SetTitle("count");
    hPar_C_2->GetXaxis()->SetTitleSize(title_size);
    hPar_C_2->GetXaxis()->SetLabelSize(label_size);
    hPar_C_2->GetYaxis()->SetTitleSize(title_size);
    hPar_C_2->GetYaxis()->SetLabelSize(label_size);
    hPar_C_2->GetXaxis()->SetTitleOffset(title_offset);
    hPar_C_2->GetYaxis()->SetTitleOffset(title_offset);
    hPar_D_2->GetXaxis()->SetTitle("d");
    hPar_D_2->GetYaxis()->SetTitle("count");
    hPar_D_2->GetXaxis()->SetTitleSize(title_size);
    hPar_D_2->GetXaxis()->SetLabelSize(label_size);
    hPar_D_2->GetYaxis()->SetTitleSize(title_size);
    hPar_D_2->GetYaxis()->SetLabelSize(label_size);
    hPar_D_2->GetXaxis()->SetTitleOffset(title_offset);
    hPar_D_2->GetYaxis()->SetTitleOffset(title_offset);
    hPar_E_2->GetXaxis()->SetTitle("e");
    hPar_E_2->GetYaxis()->SetTitle("count");
    hPar_E_2->GetXaxis()->SetTitleSize(title_size);
    hPar_E_2->GetXaxis()->SetLabelSize(label_size);
    hPar_E_2->GetYaxis()->SetTitleSize(title_size);
    hPar_E_2->GetYaxis()->SetLabelSize(label_size);
    hPar_E_2->GetXaxis()->SetTitleOffset(title_offset);
    hPar_E_2->GetYaxis()->SetTitleOffset(title_offset);
    hPar_F_2->GetXaxis()->SetTitle("f");
    hPar_F_2->GetYaxis()->SetTitle("count");
    hPar_F_2->GetXaxis()->SetTitleSize(title_size);
    hPar_F_2->GetXaxis()->SetLabelSize(label_size);
    hPar_F_2->GetYaxis()->SetTitleSize(title_size);
    hPar_F_2->GetYaxis()->SetLabelSize(label_size);
    hPar_F_2->GetXaxis()->SetTitleOffset(title_offset);
    hPar_F_2->GetYaxis()->SetTitleOffset(title_offset);
    hPar_G_2->GetXaxis()->SetTitle("g");
    hPar_G_2->GetYaxis()->SetTitle("count");
    hPar_G_2->GetXaxis()->SetTitleSize(title_size);
    hPar_G_2->GetXaxis()->SetLabelSize(label_size);
    hPar_G_2->GetYaxis()->SetTitleSize(title_size);
    hPar_G_2->GetYaxis()->SetLabelSize(label_size);
    hPar_G_2->GetXaxis()->SetTitleOffset(title_offset);
    hPar_G_2->GetYaxis()->SetTitleOffset(title_offset);
    hPar_H_2->GetXaxis()->SetTitle("h");
    hPar_H_2->GetYaxis()->SetTitle("count");
    hPar_H_2->GetXaxis()->SetTitleSize(title_size);
    hPar_H_2->GetXaxis()->SetLabelSize(label_size);
    hPar_H_2->GetYaxis()->SetTitleSize(title_size);
    hPar_H_2->GetYaxis()->SetLabelSize(label_size);
    hPar_H_2->GetXaxis()->SetTitleOffset(title_offset);
    hPar_H_2->GetYaxis()->SetTitleOffset(title_offset);
    hPar_L_2->GetXaxis()->SetTitle("l");
    hPar_L_2->GetYaxis()->SetTitle("count");
    hPar_L_2->GetXaxis()->SetTitleSize(title_size);
    hPar_L_2->GetXaxis()->SetLabelSize(label_size);
    hPar_L_2->GetYaxis()->SetTitleSize(title_size);
    hPar_L_2->GetYaxis()->SetLabelSize(label_size);
    hPar_L_2->GetXaxis()->SetTitleOffset(title_offset);
    hPar_L_2->GetYaxis()->SetTitleOffset(title_offset);

    hPar_A_3->GetXaxis()->SetTitle("-a");
    hPar_A_3->GetYaxis()->SetTitle("count");
    hPar_A_3->GetXaxis()->SetTitleSize(title_size);
    hPar_A_3->GetXaxis()->SetLabelSize(label_size);
    hPar_A_3->GetYaxis()->SetTitleSize(title_size);
    hPar_A_3->GetYaxis()->SetLabelSize(label_size);
    hPar_A_3->GetXaxis()->SetTitleOffset(title_offset);
    hPar_A_3->GetYaxis()->SetTitleOffset(title_offset);
    hPar_B_3->GetXaxis()->SetTitle("b");
    hPar_B_3->GetYaxis()->SetTitle("count");
    hPar_B_3->GetXaxis()->SetTitleSize(title_size);
    hPar_B_3->GetXaxis()->SetLabelSize(label_size);
    hPar_B_3->GetYaxis()->SetTitleSize(title_size);
    hPar_B_3->GetYaxis()->SetLabelSize(label_size);
    hPar_B_3->GetXaxis()->SetTitleOffset(title_offset);
    hPar_B_3->GetYaxis()->SetTitleOffset(title_offset);
    hPar_C_3->GetXaxis()->SetTitle("c");
    hPar_C_3->GetYaxis()->SetTitle("count");
    hPar_C_3->GetXaxis()->SetTitleSize(title_size);
    hPar_C_3->GetXaxis()->SetLabelSize(label_size);
    hPar_C_3->GetYaxis()->SetTitleSize(title_size);
    hPar_C_3->GetYaxis()->SetLabelSize(label_size);
    hPar_C_3->GetXaxis()->SetTitleOffset(title_offset);
    hPar_C_3->GetYaxis()->SetTitleOffset(title_offset);
    hPar_D_3->GetXaxis()->SetTitle("d");
    hPar_D_3->GetYaxis()->SetTitle("count");
    hPar_D_3->GetXaxis()->SetTitleSize(title_size);
    hPar_D_3->GetXaxis()->SetLabelSize(label_size);
    hPar_D_3->GetYaxis()->SetTitleSize(title_size);
    hPar_D_3->GetYaxis()->SetLabelSize(label_size);
    hPar_D_3->GetXaxis()->SetTitleOffset(title_offset);
    hPar_D_3->GetYaxis()->SetTitleOffset(title_offset);
    hPar_E_3->GetXaxis()->SetTitle("e");
    hPar_E_3->GetYaxis()->SetTitle("count");
    hPar_E_3->GetXaxis()->SetTitleSize(title_size);
    hPar_E_3->GetXaxis()->SetLabelSize(label_size);
    hPar_E_3->GetYaxis()->SetTitleSize(title_size);
    hPar_E_3->GetYaxis()->SetLabelSize(label_size);
    hPar_E_3->GetXaxis()->SetTitleOffset(title_offset);
    hPar_E_3->GetYaxis()->SetTitleOffset(title_offset);
    hPar_F_3->GetXaxis()->SetTitle("f");
    hPar_F_3->GetYaxis()->SetTitle("count");
    hPar_F_3->GetXaxis()->SetTitleSize(title_size);
    hPar_F_3->GetXaxis()->SetLabelSize(label_size);
    hPar_F_3->GetYaxis()->SetTitleSize(title_size);
    hPar_F_3->GetYaxis()->SetLabelSize(label_size);
    hPar_F_3->GetXaxis()->SetTitleOffset(title_offset);
    hPar_F_3->GetYaxis()->SetTitleOffset(title_offset);
    hPar_G_3->GetXaxis()->SetTitle("g");
    hPar_G_3->GetYaxis()->SetTitle("count");
    hPar_G_3->GetXaxis()->SetTitleSize(title_size);
    hPar_G_3->GetXaxis()->SetLabelSize(label_size);
    hPar_G_3->GetYaxis()->SetTitleSize(title_size);
    hPar_G_3->GetYaxis()->SetLabelSize(label_size);
    hPar_G_3->GetXaxis()->SetTitleOffset(title_offset);
    hPar_G_3->GetYaxis()->SetTitleOffset(title_offset);
    hPar_H_3->GetXaxis()->SetTitle("h");
    hPar_H_3->GetYaxis()->SetTitle("count");
    hPar_H_3->GetXaxis()->SetTitleSize(title_size);
    hPar_H_3->GetXaxis()->SetLabelSize(label_size);
    hPar_H_3->GetYaxis()->SetTitleSize(title_size);
    hPar_H_3->GetYaxis()->SetLabelSize(label_size);
    hPar_H_3->GetXaxis()->SetTitleOffset(title_offset);
    hPar_H_3->GetYaxis()->SetTitleOffset(title_offset);
    hPar_L_3->GetXaxis()->SetTitle("l");
    hPar_L_3->GetYaxis()->SetTitle("count");
    hPar_L_3->GetXaxis()->SetTitleSize(title_size);
    hPar_L_3->GetXaxis()->SetLabelSize(label_size);
    hPar_L_3->GetYaxis()->SetTitleSize(title_size);
    hPar_L_3->GetYaxis()->SetLabelSize(label_size);
    hPar_L_3->GetXaxis()->SetTitleOffset(title_offset);
    hPar_L_3->GetYaxis()->SetTitleOffset(title_offset);

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

        // cout << endl << "Mean (-a,b,c,d,e,f,g,h,l):" << endl << "{";
        // cout << fitPar_A->Parameter(2) << ",";
        // cout << fitPar_B->Parameter(2) << ",";
        // cout << fitPar_C->Parameter(2) << ",";
        // cout << fitPar_D->Parameter(2) << ",";
        // cout << fitPar_E->Parameter(2) << ",";
        // cout << fitPar_F->Parameter(2) << ",";
        // cout << fitPar_G->Parameter(2) << ",";
        // cout << fitPar_H->Parameter(2) << ",";
        // cout << fitPar_L->Parameter(2) << "};" << endl;

        // cout << endl << "Sigma (-a,b,c,d,e,f,g,h,l):" << endl << "{";
        // cout << fitPar_A->Parameter(3) << ",";
        // cout << fitPar_B->Parameter(3) << ",";
        // cout << fitPar_C->Parameter(3) << ",";
        // cout << fitPar_D->Parameter(3) << ",";
        // cout << fitPar_E->Parameter(3) << ",";
        // cout << fitPar_F->Parameter(3) << ",";
        // cout << fitPar_G->Parameter(3) << ",";
        // cout << fitPar_H->Parameter(3) << ",";
        // cout << fitPar_L->Parameter(3) << "};" << endl;
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
    TString dalitzParAllPdfName = fitResultsBaseName;
    dalitzParAllPdfName += "_all.pdf";
    c0->SaveAs(dalitzParAllPdfName);

    TCanvas *c1 = new TCanvas("c1","",2000,1200);
    c1->Divide(3,3,0.00000001,0.0001);
    c1->cd(1);
    hPar_A_2->Draw();
    c1->cd(2);
    hPar_B_2->Draw();
    c1->cd(3);
    hPar_C_2->Draw();
    c1->cd(4);
    hPar_D_2->Draw();
    c1->cd(5);
    hPar_E_2->Draw();
    c1->cd(6);
    hPar_F_2->Draw();
    c1->cd(7);
    hPar_G_2->Draw();
    c1->cd(8);
    hPar_H_2->Draw();
    c1->cd(9);
    hPar_L_2->Draw();
    TString dalitzParAllPdfName2 = fitResultsBaseName;
    dalitzParAllPdfName2 += "_lastMinuitCommandStatus0.pdf";
    c1->SaveAs(dalitzParAllPdfName2);

    TCanvas *c2 = new TCanvas("c2","",2000,1200);
    c2->Divide(3,3,0.00000001,0.0001);
    c2->cd(1);
    hPar_A_3->Draw();
    c2->cd(2);
    hPar_B_3->Draw();
    c2->cd(3);
    hPar_C_3->Draw();
    c2->cd(4);
    hPar_D_3->Draw();
    c2->cd(5);
    hPar_E_3->Draw();
    c2->cd(6);
    hPar_F_3->Draw();
    c2->cd(7);
    hPar_G_3->Draw();
    c2->cd(8);
    hPar_H_3->Draw();
    c2->cd(9);
    hPar_L_3->Draw();
    TString dalitzParAllPdfName3 = fitResultsBaseName;
    dalitzParAllPdfName3 += "_lastMinuitCommandStatus4.pdf";
    c2->SaveAs(dalitzParAllPdfName3);

    cout << "Total imported fit files: " << importedCounter << endl;
    // cout << "Total imported fit files with eMatrixStatus 2 = " << imported2Counter << endl;
    // cout << "Total imported fit files with eMatrixStatus 3 = " << imported3Counter << endl;

    cout << "Total imported fit files with lastMinuitCommandStatus 0 = " << imported2Counter << endl;
    cout << "Total imported fit files with lastMinuitCommandStatus 4 = " << imported3Counter << endl;
    cout << "Total nan fit files: " << nanCounter << endl;

}