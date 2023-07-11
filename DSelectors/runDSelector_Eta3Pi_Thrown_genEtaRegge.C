TString dirName = "/d/grid17/dlersch/eta3pi_trees/thrown_mc2017/";
TString fileName = "tree_thrown_genEtaRegge*.root";

void runDSelector_Eta3Pi_Thrown_genEtaRegge(){
    TChain chain("Thrown_Tree");
    chain.Add(Form("%s%s", dirName.Data(), fileName.Data()));
    gROOT->ProcessLine(".x $ROOT_ANALYSIS_HOME/scripts/Load_DSelector.C");
    chain.Process("DSelector_Eta3Pi_Thrown_genEtaRegge.C++");
}