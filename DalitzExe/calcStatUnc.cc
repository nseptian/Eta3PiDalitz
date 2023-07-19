#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "IUAmpTools/FitResults.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TFitResult.h"
#include "TString.h"

void SetHistAxis(TH1F *h1f,TString par){
    const Double_t label_size = 0.04;
    const Double_t title_size = 0.07;
    const Double_t title_offset = 0.65;
    h1f->GetXaxis()->SetTitle(par);
    h1f->GetYaxis()->SetTitle("count");
    h1f->GetXaxis()->SetTitleSize(title_size);
    h1f->GetXaxis()->SetLabelSize(label_size);
    h1f->GetYaxis()->SetTitleSize(title_size);
    h1f->GetYaxis()->SetLabelSize(label_size);
    h1f->GetXaxis()->SetTitleOffset(title_offset);
    h1f->GetYaxis()->SetTitleOffset(title_offset);
    h1f->SetLineColor(kBlack);
}

int main(int argc, char* argv[]){
    
    gStyle->SetOptFit(0001);
    gStyle->SetOptStat("ne");
    // gStyle->SetOptStat(0);
    
    if (argc < 5){
        cout << "Usage:" << endl << endl;
        cout << "\tcalcStatUnc <fit results name> <first tag> <last tag> <fit? 0/1>" << endl << endl;
        return 0;
    }

    vector<double> par_A,par_B,par_C,par_D,par_E,par_F,par_G,par_H,par_L;

    const Int_t NPar = 9;
    const Int_t hParNBins = 50;
    // const Double_t hParRange[NPar][2] = {{1.02,1.12} // kFit used
    //                                     ,{0.14,0.24}
    //                                     ,{-0.04,0.06}
    //                                     ,{0.025,0.125}
    //                                     ,{-0.05,0.05}
    //                                     ,{0.13,0.23}
    //                                     ,{-0.05,0.05}
    //                                     ,{-0.03,0.07}
    //                                     ,{-0.05,0.05}};

    //07122022
    // const Double_t hParRange[NPar][2] = {{0.92,1.02}
    //                                     ,{0.32,0.42}
    //                                     ,{-0.04,0.06}
    //                                     ,{-0.32,-0.22}
    //                                     ,{-0.05,0.05}
    //                                     ,{0.25,0.35}
    //                                     ,{0.35,0.45}
    //                                     ,{-0.05,0.05}
    //                                     ,{-0.05,0.05}};

    // 12262022a
    // const Double_t hParRange[NPar][2] = {{0.94,1.14} //-a
    //                                     ,{0.04,0.24} //b
    //                                     ,{-0.1,0.1} //c
    //                                     ,{0.07,0.27} //d
    //                                     ,{-0.1,0.1} //e
    //                                     ,{0.14,0.34} //f
    //                                     ,{-0.1,0.1} //g
    //                                     ,{-0.1,0.1} //h
    //                                     ,{-0.1,0.1}}; //l

    // 12262022c/d/e
    // const Double_t hParRange[NPar][2] = {{1.06,1.26} //-a
    //                                     ,{0.1,0.3} //b
    //                                     ,{-0.1,0.1} //c
    //                                     ,{0.00,0.2} //d
    //                                     ,{-0.1,0.1} //e
    //                                     ,{0.04,0.24} //f
    //                                     ,{-0.1,0.1} //g
    //                                     ,{-0.1,0.1} //h
    //                                     ,{-0.1,0.1}}; //l

    const Double_t hParRange[NPar][2] = {{1.00,1.20} //-a
                                        ,{0.1,0.3} //b
                                        ,{-0.1,0.1} //c
                                        ,{0.00,0.2} //d
                                        ,{-0.1,0.1} //e
                                        ,{0.04,0.24} //f
                                        ,{-0.1,0.1} //g
                                        ,{-0.1,0.1} //h
                                        ,{-0.1,0.1}}; //l

    TH1F *hPar[NPar];
    TString hParName[NPar] = {"h_Par_A","h_Par_B","h_Par_C","h_Par_D","h_Par_E","h_Par_F","h_Par_G","h_Par_H","h_Par_L"};
    TString parName[NPar] = {"-a","b","c","d","e","f","g","h","l"};

    for (size_t i = 0; i < NPar; i++)
    {
        hPar[i] = new TH1F(hParName[i],"",hParNBins,hParRange[i][0],hParRange[i][1]);
    }

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

        for (int j=0;j<NPar;j++){
            if (j==0) hPar[j]->Fill(-1*parameterValues[3]);
            else hPar[j]->Fill(parameterValues[3+j]);
        }
    }

    for (size_t i = 0; i < NPar; i++)
    {
        SetHistAxis(hPar[i],parName[i]);
    }
    
    TFitResultPtr fitResultPar[NPar];
    if (isFit) {
        for (size_t i = 0; i < NPar; i++) fitResultPar[i] = hPar[i]->Fit("gaus","S");
        
        cout << endl << "Mean (-a,b,c,d,e,f,g,h,l):" << endl << "{";
        for (size_t i = 0; i < NPar; i++) {
            cout << fitResultPar[i]->Parameter(1);
            if (i!=NPar-1) cout << ",";
            else cout << "};" << endl;
        }

        cout << endl << "Sigma (-a,b,c,d,e,f,g,h,l):" << endl << "{";
        for (size_t i = 0; i < NPar; i++) {
            cout << fitResultPar[i]->Parameter(2);
            if (i!=NPar-1) cout << ",";
            else cout << "};" << endl;
        }
    }

    TCanvas *c0 = new TCanvas("c0","",2000,1200);
    c0->Divide(3,3,0.00000001,0.0001);
    for (size_t i = 0; i < NPar; i++)
    {
        c0->cd(i+1);
        hPar[i]->Draw();
    }
    TString pdfNameDalitzParDist = "DalitzParDistribution_";
    pdfNameDalitzParDist += fitResultsBaseName;
    pdfNameDalitzParDist += ".pdf";
    c0->SaveAs(pdfNameDalitzParDist);

}