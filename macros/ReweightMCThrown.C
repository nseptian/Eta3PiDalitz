// Description: Reweight MC reco to match data t slope

TString pdfOutDir = "/d/home/septian/Eta3PiDalitzPlots/";

void gluex_style();
void ReweightMCRec(Int_t data_set, TString tag, TString SystematicVariationStr, Int_t PhotonBeamEnergyBinIdx, Bool_t isGenerateWeightedTrees=kTRUE) {

    TString runPeriod[3] = {"2017","2018S","2018F"};
    TString workingDir = Form("/d/home/septian/Eta3PiDalitz/run/Eta3PiSelectorParallelTree_%s/",tag.Data());
    TString fileInputName = Form("data_%s_%s_%s_%d.root",runPeriod[data_set].Data(),tag.Data(),SystematicVariationStr.Data(),PhotonBeamEnergyBinIdx);
    TString fileInputNameMC = Form("mc_rec_%s_%s_%s_%d.root",runPeriod[data_set].Data(),tag.Data(),SystematicVariationStr.Data(),PhotonBeamEnergyBinIdx);
    TString fileOutputName = Form("reweighted_mc_rec_%s_%s_%s_%d.root",runPeriod[data_set].Data(),tag.Data(),SystematicVariationStr.Data(),PhotonBeamEnergyBinIdx);

    TString pdfOut_base = Form("%s_%s_%d",runPeriod[data_set].Data(),tag.Data(),PhotonBeamEnergyBinIdx);

    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    
    //open input file
    TFile *fileInput = new TFile(workingDir+fileInputName);
    TFile *fileInputMC = new TFile(workingDir+fileInputNameMC);

    TString dirName = "Histograms";
    TDirectory *dir = (TDirectory*)fileInput->Get(dirName.Data());
    TDirectory *dirMC = (TDirectory*)fileInputMC->Get(dirName.Data());

    //get mandelstam_t histograms
    TString hname = "h1_MandelstamT";
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
    h1_mandelstam_t_MC->GetXaxis()->SetRangeUser(0.15,0.6);
    h1_mandelstam_t_MC->SetLineColor(kBlue);
    h1_mandelstam_t_MC->SetLineWidth(2);
    h1_mandelstam_t_MC->SetTitle("Mandelstam t");

    TCanvas *c1 = new TCanvas("c1","c1",800,600);
    h1_mandelstam_t_MC->Draw();
    h1_mandelstam_t_data->Draw("SAME");
    TLegend *leg1 = new TLegend(0.6,0.7,0.85,0.85);
    leg1->AddEntry(h1_mandelstam_t_data,"Data","l");
    leg1->AddEntry(h1_mandelstam_t_MC,"MC","l");
    leg1->Draw();
    c1->SaveAs((pdfOutDir+pdfOut_base).Data()+TString("_mandelstam_t.pdf"));


    //create histogram for h1_mandelstam_t_data to h1_mandelstam_t_MC ratio
    TH1F *h1_mandelstam_t_ratio = (TH1F*)h1_mandelstam_t_data->Clone();
    h1_mandelstam_t_ratio->Divide(h1_mandelstam_t_MC);
    h1_mandelstam_t_ratio->SetTitle("Mandelstam t ratio");
    h1_mandelstam_t_ratio->GetXaxis()->SetTitle("-t (GeV^{2})");
    h1_mandelstam_t_ratio->GetYaxis()->SetTitle("Ratio [Data/MC]");
    h1_mandelstam_t_ratio->SetStats(0);
    h1_mandelstam_t_ratio->SetLineColor(kBlack);
    h1_mandelstam_t_ratio->SetLineWidth(2);

    //fit ratio with polynomial function and print as pdf
    TF1 *fit_mandelstam_t_ratio = new TF1("fit_mandelstam_t_ratio","pol4",0.15,0.6);
    h1_mandelstam_t_ratio->Fit(fit_mandelstam_t_ratio,"R");
    h1_mandelstam_t_ratio->Draw();
    fit_mandelstam_t_ratio->Draw("SAME");
    TLegend *leg2 = new TLegend(0.2,0.7,0.45,0.85);
    leg2->AddEntry(h1_mandelstam_t_ratio,"Ratio = Data/MC","l");
    leg2->AddEntry(fit_mandelstam_t_ratio,"Polynomial fit","l");
    c1->SaveAs((pdfOutDir+pdfOut_base).Data()+TString("_mandelstam_t_ratio.pdf"));

    //create output file
    TFile *fileOutput = new TFile(workingDir+fileOutputName,"RECREATE");

    // h1_mandelstam_t_data->Write();
    // h1_mandelstam_t_MC->Write();
    h1_mandelstam_t_ratio->SetName("h_mandelstam_t_ratio");
    h1_mandelstam_t_ratio->Write();


    if (isGenerateWeightedTrees) {
        
        //create weighted trees

        TTree *dataChain = (TTree*)fileInputMC->Get("nt");
        
        Double_t EnBeam,PxBeam,PyBeam,PzBeam;
        Double_t EnProton,PxProton,PyProton,PzProton;
        Double_t EnP1,PxP1,PyP1,PzP1;
        Double_t EnP2,PxP2,PyP2,PzP2;
        Double_t EnP3,PxP3,PyP3,PzP3;
        Double_t weight;

        dataChain->SetBranchAddress("EnBeam",&EnBeam);
        dataChain->SetBranchAddress("PxBeam",&PxBeam);
        dataChain->SetBranchAddress("PyBeam",&PyBeam);
        dataChain->SetBranchAddress("PzBeam",&PzBeam);

        dataChain->SetBranchAddress("EnProton",&EnProton);
        dataChain->SetBranchAddress("PxProton",&PxProton);
        dataChain->SetBranchAddress("PyProton",&PyProton);
        dataChain->SetBranchAddress("PzProton",&PzProton);

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
        TH1F *h1_mandelstam_t_unweighted = new TH1F("h1_mandelstam_t_unweighted","Unweighted mandelstam t",101,0.15,0.6);
        h1_mandelstam_t_unweighted->GetXaxis()->SetTitle("|t| [GeV^{2}]");
        h1_mandelstam_t_unweighted->GetYaxis()->SetTitle("Events");

        //t histogram for weighted events
        TH1F *h1_mandelstam_t_weighted = new TH1F("h1_mandelstam_t_weighted","Weighted mandelstam t",101,0.15,0.6);
        h1_mandelstam_t_weighted->GetXaxis()->SetTitle("|t| [GeV^{2}]");
        h1_mandelstam_t_weighted->GetYaxis()->SetTitle("Events");

        // reduce datasets using accept/reject based on the ratio to 2018S data
        Double_t N_2017_ccdb =  272616
        Double_t N_2018S_ccdb =  540145
        Double_t N_2018F_ccdb =  292879

        Double_t ratio[3] = {N_2017_ccdb/N_2018S_ccdb,N_2018S_ccdb/N_2018S_ccdb,N_2018F_ccdb/N_2018S_ccdb};

        for (Int_t i=0; i<dataChain->GetEntries(); i++) {
            
            if (TRandom3().Rndm() > ratio[data_set]) continue;

            dataChain->GetEntry(i);
            TLorentzVector P4_RecoilProton = TLorentzVector(PxProton,PyProton,PzProton,EnProton);
            TLorentzVector P4_TargetProton = TLorentzVector(0,0,0,0.938272);

            Double_t mandelstam_t = -1*(P4_TargetProton-P4_RecoilProton).M2();
            // cout << mandelstam_t << endl;
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
        h1_mandelstam_t_weighted->SetLineColor(kBlack);
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

        // draw data with scaled reweighted MC

        // cout << h1_mandelstam_t_weighted->Integral() << " " << h1_mandelstam_t_data->Integral() << endl;
        h1_mandelstam_t_weighted->Scale(h1_mandelstam_t_data->Integral()/h1_mandelstam_t_weighted->Integral());
        c1->Clear();
        h1_mandelstam_t_weighted->GetXaxis()->SetRangeUser(0.15,0.6);
        h1_mandelstam_t_weighted->SetLineWidth(2);
        h1_mandelstam_t_weighted->Draw();
        h1_mandelstam_t_data->Draw("SAME");
        leg1->Clear();
        leg1->AddEntry(h1_mandelstam_t_data,"Data","l");
        leg1->AddEntry(h1_mandelstam_t_weighted,"Reweighted MC","l");
        leg1->Draw();
        c1->SaveAs((pdfOutDir+pdfOut_base).Data()+TString("_mandelstam_t_data_reweightedMC.pdf"));

        c1->Clear();
        c1->SetWindowSize(2000,800);
        c1->Divide(2,1);
        c1->cd(1);

        // draw the ratio and fit function
        h1_mandelstam_t_ratio->Draw();
        fit_mandelstam_t_ratio->Draw("SAME");
        leg2->Draw();

        c1->cd(2);
        gPad->SetLeftMargin(0.15);

        // draw the weighted MC and data
        h1_mandelstam_t_weighted->GetXaxis()->SetTitle("-t (GeV^{2})");
        h1_mandelstam_t_weighted->GetYaxis()->SetRangeUser(0,1.2*h1_mandelstam_t_data->GetMaximum());
        h1_mandelstam_t_weighted->Draw();
        h1_mandelstam_t_data->Draw("SAME");
        leg1->Draw();

        c1->SaveAs((pdfOutDir+pdfOut_base).Data()+TString("_mandelstam_t_ratio_datareweightedMC.pdf"));
    }
    fileOutput->Close();

}