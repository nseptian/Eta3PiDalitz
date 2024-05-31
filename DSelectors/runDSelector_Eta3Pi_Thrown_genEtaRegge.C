#include <TChain.h>
#include "TProof.h"
#include "TProofDebug.h"

const TString dirName = "/d/grid17/septian/GlueX_MC/nseptian_GenEtaRegge_Eta3Pi_2017-01_10M_3799/thrown/";
const TString fileName = "tree_thrown_genEtaRegge*.root";

const Bool_t useProof = kTRUE;
const Int_t numThread = 36;

// TString dirName = "/d/grid17/septian/GlueX_MC/nseptian_GenEtaRegge_Eta3Pi_2017-01_10M_3799/thrown/";
// TString fileName = "tree_thrown_genEtaRegge_030346_decay_evtgen.root";

void runDSelector_Eta3Pi_Thrown_genEtaRegge(){
    R__LOAD_LIBRARY(libDSelector);
    TChain chain("Thrown_Tree");
    chain.Add(Form("%s%s", dirName.Data(), fileName.Data()));
    gROOT->ProcessLine(".x $ROOT_ANALYSIS_HOME/scripts/Load_DSelector.C");
    if (useProof) {
        gEnv->SetValue("ProofLite.Sandbox", "$PWD/.proof/");
        DPROOFLiteManager *dproof = new DPROOFLiteManager();
        dproof->Process_Chain(&chain, "DSelector_Eta3Pi_Thrown_genEtaRegge.C+", numThread);
    }
    else {
        chain.Process("DSelector_Eta3Pi_Thrown_genEtaRegge.C+");
    }
}