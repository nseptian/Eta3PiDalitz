
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

TString dirName, dSelectorName, treeName, tagName, outFileHist, outFileTree, outFileFlatTree;
vector<TString> files;
void runDSelector(const TString dirName, const vector<TString> files, const TString dSelectorName, const TString treeName, const Bool_t useProof, const Int_t numThread, TString outFileHist = "", TString outFileTree = "", TString outFileFlatTree = "");
vector<TString> SelectRootFiles(const TString dirName, const TString PreFileName, const Int_t lowRun, const Int_t highRun, TString PostFileName);

void runDSelector_Eta3Pi_genEtaRegge(){
    R__LOAD_LIBRARY(libDSelector);
    gROOT->ProcessLine(".x $ROOT_ANALYSIS_HOME/scripts/Load_DSelector.C");

    // TString dirName = "/d/grid17/septian/GlueX_MC/nseptian_Genr8_Eta3Pi_2017-01_10M_3839/root/trees/";
    // TString fileName = "tree_eta__eta_pi0pippim__B4_M17_M7_genr8*.root";
    // TString dSelectorName = "DSelector_Eta3Pi_Tree_genEtaRegge.C";
    // TString treeName = "eta__eta_pi0pippim__B4_M17_M7_Tree";



    // TString dirName = "/d/grid17/septian/GlueX_MC/nseptian_GenEtaRegge_Eta3Pi_2017-01_10M_3826_F1/trees/";
    // TString fileName = "tree_eta__eta_pi0pippim__B4_M17_M7_genEtaRegge_*.root";
    // TString dSelectorName = "DSelector_Eta3Pi_Tree_genEtaRegge.C";
    // TString treeName = "eta__eta_pi0pippim__B4_M17_M7_F1_Tree";
    // vector<TString> files = SelectRootFiles(dirName, "tree_eta__eta_pi0pippim__B4_M17_M7_genEtaRegge_0", 30274, 30280, "_decay_evtgen.root");
    // vector<TString> files = SelectRootFiles(dirName, "tree_eta__eta_pi0pippim__B4_M17_M7_genEtaRegge_0", 30274, 30795, "_decay_evtgen.root");
    // vector<TString> files = SelectRootFiles(dirName, "tree_eta__eta_pi0pippim__B4_M17_M7_F1_0", 30274, 31057, ".root");

    // TString dirName = "/d/grid17/septian/GlueX_MC/nseptian_GenEtaRegge_Eta3Pi_2017-01_10M_3858/trees/";
    // TString dirName = "/d/grid17/septian/GlueX_MC/nseptian_GenEtaRegge_Eta3Pi_2018-01_10M_3858/trees/tree_eta__eta_pi0pippim__B4_M17_M7_genEtaRegge/";
    // TString fileName = "tree_eta__eta_pi0pippim__B4_M17_M7_genEtaRegge*.root";
    // TString dSelectorName = "DSelector_Eta3Pi_Tree_genEtaRegge.C";
    // TString treeName = "eta__eta_pi0pippim__B4_M17_M7_Tree";

    // TString dirName = "/d/grid17/septian/GlueX_MC/nseptian_GenEtaRegge_Eta3Pi_2017-01_10M_3892/trees/tree_eta__eta_pi0pippim__B4_F1_M17_M7_genEtaRegge/";
    // vector<TString> files = SelectRootFiles(dirName, "tree_eta__eta_pi0pippim__B4_F1_M17_M7_genEtaRegge_0", 30274, 31057, "_decay_evtgen.root");
    // TString treeName = "eta__eta_pi0pippim__B4_F1_M17_M7_Tree";

    // runDSelector(dirName, files, dSelectorName, treeName, useProof, numThread);

    // dirName = "/d/grid17/septian/GlueX_MC/nseptian_Genr8_Eta3Pi_2017-01_10M_3839/root/thrown/";
    // fileName = "tree_thrown_genr8*.root";

    // dirName = "/d/grid17/septian/GlueX_MC/nseptian_GenEtaRegge_Eta3Pi_2017-01_10M_3826/root/thrown/";
    // dirName = "/d/grid17/septian/GlueX_MC/nseptian_GenEtaRegge_Eta3Pi_2017-01_cobrems_10M_3865/thrown/";
    // dirName = "/d/grid17/septian/GlueX_MC/nseptian_GenEtaRegge_Eta3Pi_2018-01_10M_3858/thrown/";
    // fileName = "tree_thrown_genEtaRegge*.root";
    // dSelectorName = "DSelector_Eta3Pi_Thrown_genEtaRegge.C";
    // treeName = "Thrown_Tree";

    // dirName = "/d/grid17/septian/GlueX_MC/nseptian_GenEtaRegge_Eta3Pi_2017-01_cobrems_10M_3893/thrown/";
    // dSelectorName = "DSelector_Eta3Pi_Thrown_genEtaRegge.C";
    // treeName = "Thrown_Tree";
    // files = SelectRootFiles(dirName, "tree_thrown_genEtaRegge_0", 30274, 30795, "_decay_evtgen.root");

    // runDSelector(dirName, files, dSelectorName, treeName, useProof, numThread);
    
    
    tagName = "2018S_ccdbFlux_genEtaRegge";

    // Run DSelector over 2018S ccdbFlux genEtaRegge MC reconstructed data
    // dirName = "/d/grid17/septian/GlueX_MC/nseptian_GenEtaRegge_Eta3Pi_2018-01_10M_3858_F1/trees/";
    // dSelectorName = "DSelector_Eta3Pi_Tree_genEtaRegge.C";
    // treeName = "eta__eta_pi0pippim__B4_F1_M17_M7_Tree";
    // files = SelectRootFiles(dirName, "tree_eta__eta_pi0pippim__B4_F1_M17_M7_0", 40856, 42577, ".root");
    // outFileHist = "Hist_Eta3Pi_Tree_" + tagName + ".root";
    // outFileTree = "Tree_Eta3Pi_Tree_" + tagName + ".root";
    // outFileFlatTree = "Tree_Eta3Pi_Tree_" + tagName + "_flat.root";
    // runDSelector(dirName, files, dSelectorName, treeName, useProof, numThread, outFileHist, outFileTree, outFileFlatTree);

    // Run DSelector over 2018S ccdbFlux genEtaRegge MC thrown data
    // dirName = "/d/grid17/septian/GlueX_MC/nseptian_GenEtaRegge_Eta3Pi_2018-01_10M_3858/thrown/";
    // dSelectorName = "DSelector_Eta3Pi_Thrown_genEtaRegge.C";
    // treeName = "Thrown_Tree";
    // files = SelectRootFiles(dirName, "tree_thrown_genEtaRegge_0", 40856, 42577, "_decay_evtgen.root");
    // outFileHist = "Hist_Eta3Pi_Thrown_" + tagName + ".root";
    // outFileTree = "Tree_Eta3Pi_Thrown_" + tagName + ".root";
    // outFileFlatTree = "Tree_Eta3Pi_Thrown_" + tagName + "_flat.root";
    // runDSelector(dirName, files, dSelectorName, treeName, useProof, numThread, outFileHist, outFileTree, outFileFlatTree);

    // Run DSelector over 2018F ccdbFlux genEtaRegge MC reconstructed data
    // files.clear();
    // tagName = "2018F_ccdbFlux_genEtaRegge";
    // dirName = "/d/grid17/septian/GlueX_MC/nseptian_GenEtaRegge_Eta3Pi_2018-08_10M_05/root/trees/";
    // treeName = "eta__eta_pi0pippim__B4_F1_M17_M7_Tree";
    // dSelectorName = "DSelector_Eta3Pi_Tree_genEtaRegge.C";
    // files = SelectRootFiles(dirName, "tree_eta__eta_pi0pippim__B4_F1_M17_M7_genEtaRegge_0", 50677, 51768, "_000_decay_evtgen.root");
    // vector<TString> files2 = SelectRootFiles(dirName, "tree_eta__eta_pi0pippim__B4_F1_M17_M7_genEtaRegge_0", 50677, 51768, "_001_decay_evtgen.root");
    // vector<TString> files3 = SelectRootFiles(dirName, "tree_eta__eta_pi0pippim__B4_F1_M17_M7_genEtaRegge_0", 50677, 51768, "_002_decay_evtgen.root");
    // vector<TString> files4 = SelectRootFiles(dirName, "tree_eta__eta_pi0pippim__B4_F1_M17_M7_genEtaRegge_0", 50677, 51768, "_003_decay_evtgen.root");
    // vector<TString> files5 = SelectRootFiles(dirName, "tree_eta__eta_pi0pippim__B4_F1_M17_M7_genEtaRegge_0", 50677, 51768, "_004_decay_evtgen.root");
    // vector<TString> files6 = SelectRootFiles(dirName, "tree_eta__eta_pi0pippim__B4_F1_M17_M7_genEtaRegge_0", 50677, 51768, "_005_decay_evtgen.root");       
    // files.insert(files.end(), files2.begin(), files2.end());
    // files.insert(files.end(), files3.begin(), files3.end());
    // files.insert(files.end(), files4.begin(), files4.end());
    // files.insert(files.end(), files5.begin(), files5.end());
    // files.insert(files.end(), files6.begin(), files6.end());
    // cout << "Number of files: " << files.size() << endl;
    // outFileHist = "Hist_Eta3Pi_Tree_" + tagName + ".root";
    // outFileTree = "Tree_Eta3Pi_Tree_" + tagName + ".root";
    // outFileFlatTree = "Tree_Eta3Pi_Tree_" + tagName + "_flat.root";
    // runDSelector(dirName, files, dSelectorName, treeName, useProof, numThread, outFileHist, outFileTree, outFileFlatTree);

    // Run DSelector over 2018F ccdbFlux genEtaRegge MC thrown data
    // files.clear();
    // tagName = "2018F_ccdbFlux_genEtaRegge";
    // dirName = "/d/grid17/septian/GlueX_MC/nseptian_GenEtaRegge_Eta3Pi_2018-08_10M_05/root/thrown/";
    // treeName = "Thrown_Tree";
    // dSelectorName = "DSelector_Eta3Pi_Thrown_genEtaRegge.C";
    // files = SelectRootFiles(dirName, "tree_thrown_genEtaRegge_0", 50677, 51768, "_000_decay_evtgen.root");
    // vector<TString> files2 = SelectRootFiles(dirName, "tree_thrown_genEtaRegge_0", 50677, 51768, "_001_decay_evtgen.root");
    // vector<TString> files3 = SelectRootFiles(dirName, "tree_thrown_genEtaRegge_0", 50677, 51768, "_002_decay_evtgen.root");
    // vector<TString> files4 = SelectRootFiles(dirName, "tree_thrown_genEtaRegge_0", 50677, 51768, "_003_decay_evtgen.root");
    // vector<TString> files5 = SelectRootFiles(dirName, "tree_thrown_genEtaRegge_0", 50677, 51768, "_004_decay_evtgen.root");
    // vector<TString> files6 = SelectRootFiles(dirName, "tree_thrown_genEtaRegge_0", 50677, 51768, "_005_decay_evtgen.root");
    // files.insert(files.end(), files2.begin(), files2.end());
    // files.insert(files.end(), files3.begin(), files3.end());
    // files.insert(files.end(), files4.begin(), files4.end());
    // files.insert(files.end(), files5.begin(), files5.end());
    // files.insert(files.end(), files6.begin(), files6.end());

    files.clear();
    tagName = "2017_cobremsFlux_30M_genEtaRegge";
    dirName = "/d/grid17/septian/GlueX_MC/nseptian_GenEtaRegge_Eta3Pi_2017-01_cobrems_30M_00/root/trees/";
    files.push_back(Form("%stree_eta__eta_pi0pippim__B4_F1_M17_M7_genEtaRegge_*.root",dirName.Data()));
    dSelectorName = "DSelector_Eta3Pi_Tree_genEtaRegge.C";
    treeName = "eta__eta_pi0pippim__B4_F1_M17_M7_Tree";
    outFileHist = "Hist_Eta3Pi_Tree_" + tagName + ".root";
    outFileTree = "Tree_Eta3Pi_Tree_" + tagName + ".root";
    outFileFlatTree = "Tree_Eta3Pi_Tree_" + tagName + "_flat.root";
    runDSelector(dirName, files, dSelectorName, treeName, useProof, numThread, outFileHist, outFileTree, outFileFlatTree);

    files.clear();
    tagName = "2017_cobremsFlux_30M_genEtaRegge";
    dirName = "/d/grid17/septian/GlueX_MC/nseptian_GenEtaRegge_Eta3Pi_2017-01_cobrems_30M_00/root/thrown/";
    files.push_back(Form("%stree_thrown_genEtaRegge_*.root",dirName.Data()));
    dSelectorName = "DSelector_Eta3Pi_Thrown_genEtaRegge.C";
    treeName = "Thrown_Tree";
    outFileHist = "Hist_Eta3Pi_Thrown_" + tagName + ".root";
    outFileTree = "Tree_Eta3Pi_Thrown_" + tagName + ".root";
    outFileFlatTree = "Tree_Eta3Pi_Thrown_" + tagName + "_flat.root";
    runDSelector(dirName, files, dSelectorName, treeName, useProof, numThread, outFileHist, outFileTree, outFileFlatTree);

}

void runDSelector(const TString dirName, const vector<TString> files, const TString dSelectorName, const TString treeName, const Bool_t useProof, const Int_t numThread, TString outFileHist, TString outFileTree, TString outFileFlatTree){
    TChain chain(treeName.Data());
    // Add files to chain
    for (auto file : files){
        cout << "Adding file " << file << endl;
        chain.Add(file.Data());
    }
    if (useProof) {
        gEnv->SetValue("ProofLite.Sandbox", "$PWD/.proof/");
        DPROOFLiteManager *dproof = new DPROOFLiteManager();
        dproof->Process_Chain(&chain, Form("%s+", dSelectorName.Data()), numThread, outFileHist.Data(), outFileTree.Data());
        delete dproof;
        // rename output file
        if (dSelectorName.Contains("Thrown")) {
            cout << "Renaming output flat tree file: Tree_Eta3Pi_Thrown_genEtaRegge_flat.root -> " << outFileFlatTree.Data() << endl;
            gSystem->Rename("Tree_Eta3Pi_Thrown_genEtaRegge_flat.root", outFileFlatTree.Data());
        }
        else {
            cout << "Renaming output flat tree file: Tree_Eta3Pi_Tree_genEtaRegge_flat.root -> " << outFileFlatTree.Data() << endl;
            gSystem->Rename("Tree_Eta3Pi_Tree_genEtaRegge_flat.root", outFileFlatTree.Data());
        }
    }
    else {
        cout << "Processing " << dSelectorName << endl;
        chain.Process(Form("%s+", dSelectorName.Data()));
    }
}

vector<TString> SelectRootFiles(const TString dirName, const TString PreFileName, const Int_t lowRun, const Int_t highRun, TString PostFileName){
    // check the directory if the files exist
    vector<TString> files;
    for (Int_t i = lowRun; i <= highRun; i++){
        TString file = Form("%s%s%d%s", dirName.Data(), PreFileName.Data(), i, PostFileName.Data());
        if (gSystem->AccessPathName(file)){
            cout << "File " << file << " does not exist" << endl;
        }
        else {
            // cout << "File " << file << " exists" << endl;
            files.push_back(file);
        }
    }
    return files;
}