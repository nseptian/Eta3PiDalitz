#include <TChain.h>
#include "TProof.h"
#include "TProofDebug.h"

// const TString dirName = "/d/grid17/septian/GlueX_MC/nseptian_GenEtaRegge_Eta3Pi_2017-01_10M_3799/trees/tree_eta__eta_pi0pippim__B4_M17_genEtaRegge/";
// const TString dirName = "/d/grid17/septian/GlueX_MC/GenEtaRegge_Eta3Pi_2017-01_nseptian_100k_3777/trees/tree_eta__eta_pi0pippim__B4_M14_M17_genEtaRegge/";
// const TString fileName = "tree_eta__eta_pi0pippim__B4_M14_M17_genEtaRegge_031004_decay_evtgen.root";
// const TString fileName = "tree_eta__eta_pi0pippim__B4_M14_M17_genEtaRegge*.root";

// const TString dSelectorName = "DSelector_PiPiGG.C";
// const TString dSelectorName = "DSelector_Eta3Pi_Tree_genEtaRegge.C";
// const TString treeName = "eta__eta_pi0pippim__B4_M14_M17_Tree";

const Bool_t useProof = kTRUE;
const Int_t numThread = 36;

// TString dirName = "/d/grid17/septian/GlueX_MC/nseptian_GenEtaRegge_Eta3Pi_2017-01_10M_3799/thrown/";
// TString fileName = "tree_thrown_genEtaRegge_030346_decay_evtgen.root";
void runDSelector(const TString dirName, const TString fileName, const TString dSelectorName, const TString treeName, const Bool_t useProof, const Int_t numThread);

void runDSelector_Eta3Pi_genEtaRegge(){
    R__LOAD_LIBRARY(libDSelector);
    gROOT->ProcessLine(".x $ROOT_ANALYSIS_HOME/scripts/Load_DSelector.C");

    // TString dirName = "/d/grid17/septian/GlueX_MC/nseptian_Genr8_Eta3Pi_2017-01_10M_3839/root/trees/";
    // TString fileName = "tree_eta__eta_pi0pippim__B4_M17_M7_genr8*.root";
    // TString dSelectorName = "DSelector_Eta3Pi_Tree_genEtaRegge.C";
    // TString treeName = "eta__eta_pi0pippim__B4_M17_M7_Tree";

    TString dirName = "/d/grid17/septian/GlueX_MC/nseptian_GenEtaRegge_Eta3Pi_2017-01_10M_3826/root/trees/";
    TString fileName = "tree_eta__eta_pi0pippim__B4_M17_M7_genEtaRegge*.root";
    TString dSelectorName = "DSelector_Eta3Pi_Tree_genEtaRegge.C";
    TString treeName = "eta__eta_pi0pippim__B4_M17_M7_Tree";

    runDSelector(dirName, fileName, dSelectorName, treeName, useProof, numThread);

    // dirName = "/d/grid17/septian/GlueX_MC/nseptian_Genr8_Eta3Pi_2017-01_10M_3839/root/thrown/";
    // fileName = "tree_thrown_genr8*.root";
    // dSelectorName = "DSelector_Eta3Pi_Thrown_genEtaRegge.C";
    // treeName = "Thrown_Tree";

    dirName = "/d/grid17/septian/GlueX_MC/nseptian_GenEtaRegge_Eta3Pi_2017-01_10M_3826/root/thrown/";
    fileName = "tree_thrown_genEtaRegge*.root";
    dSelectorName = "DSelector_Eta3Pi_Thrown_genEtaRegge.C";
    treeName = "Thrown_Tree";

    runDSelector(dirName, fileName, dSelectorName, treeName, useProof, numThread);
    
}

void runDSelector(const TString dirName, const TString fileName, const TString dSelectorName, const TString treeName, const Bool_t useProof, const Int_t numThread){
    TChain chain(treeName.Data());
    chain.Add(Form("%s%s", dirName.Data(), fileName.Data()));
    cout << "Adding " << Form("%s%s", dirName.Data(), fileName.Data()) << " to chain" << endl;
    if (useProof) {
        gEnv->SetValue("ProofLite.Sandbox", "$PWD/.proof/");
        DPROOFLiteManager *dproof = new DPROOFLiteManager();
        dproof->Process_Chain(&chain, Form("%s+", dSelectorName.Data()), numThread);
        delete dproof;
    }
    else {
        cout << "Processing " << dSelectorName << endl;
        chain.Process(Form("%s+", dSelectorName.Data()));
    }
}