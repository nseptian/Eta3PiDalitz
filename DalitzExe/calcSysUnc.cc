#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "IUAmpTools/FitResults.h"
#include "TH1F.h"
#include "TFitResult.h"
#include "TCanvas.h"
#include "TStyle.h"

int main(int argc, char* argv[]){
    
    gStyle->SetOptStat(11);
    // gStyle->SetOptStat("ne");
    
    if (argc < 6){
        cout << "Usage:" << endl << endl;
        cout << "\tcalcSysUnc <central value fit result name> <fit results name> <first tag> <last tag> <fit? 0/1>" << endl << endl;
        return 0;
    }

    string centralValueFitResultName(argv[1]);
    FitResults centralValueResults(centralValueFitResultName);
    vector<double> centralValue = centralValueResults.parValueList();

    centralValue[3] = -1*centralValue[3];
    cout << "Central value fit results: " << centralValueFitResultName << endl;
    cout << "a = " << centralValue[3] << endl;
    cout << "b = " << centralValue[4] << endl;
    cout << "c = " << centralValue[5] << endl;
    cout << "d = " << centralValue[6] << endl;
    cout << "e = " << centralValue[7] << endl;
    cout << "f = " << centralValue[8] << endl;
    cout << "g = " << centralValue[9] << endl;
    cout << "h = " << centralValue[10] << endl;
    cout << "l = " << centralValue[11] << endl;
    
    string parameterName[9] = {"-a","b","c","d","e","f","g","h","l"};


    TH1F *hPar_all[9];
    TH1F *hPar_MinuitCommandStatus0[9];
    TH1F *hPar_MinuitCommandStatus4[9];

    for (int i=0;i<9;i++){
        cout << "Creating histogram for " << parameterName[i] << endl;
        stringstream ss;
        ss << "hPar_" << i;
        string hParName;
        ss >> hParName;
        hPar_all[i] = new TH1F(hParName.c_str(),"",50,centralValue[i+3]-0.05,centralValue[i+3]+0.05);
        string hParName2 = hParName + "_MinuitCommandStatus0";
        hPar_MinuitCommandStatus0[i] = new TH1F(hParName2.c_str(),"",50,centralValue[i+3]-0.05,centralValue[i+3]+0.05);
        string hParName3 = hParName + "_MinuitCommandStatus4";
        hPar_MinuitCommandStatus4[i] = new TH1F(hParName3.c_str(),"",50,centralValue[i+3]-0.05,centralValue[i+3]+0.05);
    }

    string fitResultsBaseName(argv[2]);
    int first_tag = stoi(argv[3]);
    cout << "First tag: " << first_tag << endl;
    int last_tag = stoi(argv[4]);
    cout << "Last tag: " << last_tag << endl;
    int isFit = stoi(argv[5]);
    cout << "Fit? " << isFit << endl;

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

            for (int j=0;j<9;j++){
                if (j==0) hPar_all[j]->Fill(-1*parameterValues[j+3]);
                else hPar_all[j]->Fill(parameterValues[j+3]);
                if (results.lastMinuitCommandStatus()==0) {
                    if (j==0) hPar_MinuitCommandStatus0[j]->Fill(-1*parameterValues[j+3]);
                    else hPar_MinuitCommandStatus0[j]->Fill(parameterValues[j+3]);
                }
                if (results.lastMinuitCommandStatus()==4) {
                    if (j==0) hPar_MinuitCommandStatus4[j]->Fill(-1*parameterValues[j+3]);
                    else hPar_MinuitCommandStatus4[j]->Fill(parameterValues[j+3]);
                }
            }
            if (results.lastMinuitCommandStatus()==0) imported2Counter++;
            if (results.lastMinuitCommandStatus()==4) imported3Counter++;
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

    for (int i=0;i<9;i++){
        hPar_all[i]->GetXaxis()->SetTitle(parameterName[i].c_str());
        hPar_all[i]->GetYaxis()->SetTitle("count");
        hPar_all[i]->GetXaxis()->SetTitleSize(title_size);
        hPar_all[i]->GetXaxis()->SetLabelSize(label_size);
        hPar_all[i]->GetYaxis()->SetTitleSize(title_size);
        hPar_all[i]->GetYaxis()->SetLabelSize(label_size);
        hPar_all[i]->GetXaxis()->SetTitleOffset(title_offset);
        hPar_all[i]->GetYaxis()->SetTitleOffset(title_offset);

        hPar_MinuitCommandStatus0[i]->GetXaxis()->SetTitle(parameterName[i].c_str());
        hPar_MinuitCommandStatus0[i]->GetYaxis()->SetTitle("count");
        hPar_MinuitCommandStatus0[i]->GetXaxis()->SetTitleSize(title_size);
        hPar_MinuitCommandStatus0[i]->GetXaxis()->SetLabelSize(label_size);
        hPar_MinuitCommandStatus0[i]->GetYaxis()->SetTitleSize(title_size);
        hPar_MinuitCommandStatus0[i]->GetYaxis()->SetLabelSize(label_size);
        hPar_MinuitCommandStatus0[i]->GetXaxis()->SetTitleOffset(title_offset);
        hPar_MinuitCommandStatus0[i]->GetYaxis()->SetTitleOffset(title_offset);

        hPar_MinuitCommandStatus4[i]->GetXaxis()->SetTitle(parameterName[i].c_str());
        hPar_MinuitCommandStatus4[i]->GetYaxis()->SetTitle("count");
        hPar_MinuitCommandStatus4[i]->GetXaxis()->SetTitleSize(title_size);
        hPar_MinuitCommandStatus4[i]->GetXaxis()->SetLabelSize(label_size);
        hPar_MinuitCommandStatus4[i]->GetYaxis()->SetTitleSize(title_size);
        hPar_MinuitCommandStatus4[i]->GetYaxis()->SetLabelSize(label_size);
        hPar_MinuitCommandStatus4[i]->GetXaxis()->SetTitleOffset(title_offset);
        hPar_MinuitCommandStatus4[i]->GetYaxis()->SetTitleOffset(title_offset);
    }

    if (isFit) {
        TFitResultPtr fitResultPtr[9];
        for (int i=0;i<9;i++){
            fitResultPtr[i] = hPar_all[i]->Fit("gaus","S");
        }

        cout << endl << "Mean (-a,b,c,d,e,f,g,h,l):" << endl << "{";
        for (int i=0;i<9;i++){
            if (i!=8) cout << fitResultPtr[i]->Parameter(1) << ",";
            else cout << fitResultPtr[i]->Parameter(1) << "};" << endl;
        }

        cout << endl << "Sigma (-a,b,c,d,e,f,g,h,l):" << endl << "{";
        for (int i=0;i<9;i++){
            if (i!=8) cout << fitResultPtr[i]->Parameter(2) << ",";
            else cout << fitResultPtr[i]->Parameter(2) << "};" << endl;
        }
    }

    TCanvas *c0 = new TCanvas("c0","",2000,1200);
    c0->Divide(3,3,0.00000001,0.0001);
    for (int i=0;i<9;i++){
        c0->cd(i+1);
        hPar_all[i]->Draw();
    }
    TString dalitzParAllPdfName = fitResultsBaseName;
    dalitzParAllPdfName += "_all.pdf";
    c0->SaveAs(dalitzParAllPdfName);

    TCanvas *c1 = new TCanvas("c1","",2000,1200);
    c1->Divide(3,3,0.00000001,0.0001);
    for (int i=0;i<9;i++){
        c1->cd(i+1);
        hPar_MinuitCommandStatus0[i]->Draw();
    }

    TString dalitzParAllPdfName2 = fitResultsBaseName;
    dalitzParAllPdfName2 += "_lastMinuitCommandStatus0.pdf";
    c1->SaveAs(dalitzParAllPdfName2);

    TCanvas *c2 = new TCanvas("c2","",2000,1200);
    c2->Divide(3,3,0.00000001,0.0001);
    for (int i=0;i<9;i++){
        c2->cd(i+1);
        hPar_MinuitCommandStatus4[i]->Draw();
    }
    TString dalitzParAllPdfName3 = fitResultsBaseName;
    dalitzParAllPdfName3 += "_lastMinuitCommandStatus4.pdf";
    c2->SaveAs(dalitzParAllPdfName3);

    cout << "Total imported fit files: " << importedCounter << endl;

    cout << "Total imported fit files with lastMinuitCommandStatus 0 = " << imported2Counter << endl;
    cout << "Total imported fit files with lastMinuitCommandStatus 4 = " << imported3Counter << endl;
    cout << "Total nan fit files: " << nanCounter << endl;

}