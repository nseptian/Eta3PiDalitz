#include "DSelector_Eta3Pi_Thrown_genEtaRegge.h"

void DSelector_Eta3Pi_Thrown_genEtaRegge::Init(TTree *locTree)
{
	// USERS: IN THIS FUNCTION, ONLY MODIFY SECTIONS WITH A "USER" OR "EXAMPLE" LABEL. LEAVE THE REST ALONE.

	// The Init() function is called when the selector needs to initialize a new tree or chain.
	// Typically here the branch addresses and branch pointers of the tree will be set.
	// Init() will be called many times when running on PROOF (once per file to be processed).

	//USERS: SET OUTPUT FILE NAME //can be overriden by user in PROOF
	dOutputFileName = "Tree_Eta3Pi_Thrown_genEtaRegge.root"; //"" for none
	dFlatTreeFileName = "Tree_Eta3Pi_Thrown_genEtaRegge_flat.root"; //output flat tree (one combo per tree entry), "" for none
	dFlatTreeName = "nt"; //if blank, default name will be chosen
	//USERS: SET OUTPUT TREE FILES/NAMES //e.g. binning into separate files for AmpTools
	// dOutputTreeFileNameMap["Bin1"] = "mcgen_bin1.root"; //key is user-defined, value is output file name
	//dOutputTreeFileNameMap["Bin2"] = "mcgen_bin2.root"; //key is user-defined, value is output file name
	//dOutputTreeFileNameMap["Bin3"] = "mcgen_bin3.root"; //key is user-defined, value is output file name

	//Because this function gets called for each TTree in the TChain, we must be careful:
		//We need to re-initialize the tree interface & branch wrappers, but don't want to recreate histograms
	bool locInitializedPriorFlag = dInitializedFlag; //save whether have been initialized previously
	DSelector::Init(locTree); //This must be called to initialize wrappers for each new TTree
	//gDirectory now points to the output file with name dOutputFileName (if any)
	if(locInitializedPriorFlag)
		return; //have already created histograms, etc. below: exit

	dPreviousRunNumber = 0;

	/******************************** EXAMPLE USER INITIALIZATION: STAND-ALONE HISTOGRAMS *******************************/

	/************************************* ADVANCED EXAMPLE: CHOOSE BRANCHES TO READ ************************************/

	//TO SAVE PROCESSING TIME
		//If you know you don't need all of the branches/data, but just a subset of it, you can speed things up
		//By default, for each event, the data is retrieved for all branches
		//If you know you only need data for some branches, you can skip grabbing data from the branches you don't need
		//Do this by doing something similar to the commented code below

	//dTreeInterface->Clear_GetEntryBranches(); //now get none
	//dTreeInterface->Register_GetEntryBranch("Proton__P4"); //manually set the branches you want

	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("EnP1");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("PxP1");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("PyP1");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("PzP1");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("EnP2");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("PxP2");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("PyP2");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("PzP2");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("EnP3");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("PxP3");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("PyP3");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("PzP3");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("t");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("EnBeam");

	dHist_PiPlusPiMinusPi0Mass = new TH1F("PiPlusPiMinusPi0InvMass", ";M_{#pi^{+}#pi^{-}#pi^{0}} [GeV];", 100, 0.3, 1.3);
	dHist_MandelstamT = new TH1F("MandelstamT", ";t [GeV^{2}];", 100, -20.0, 0.0);
	dHist_Dalitz = new TH2F("h2_DalitzPlotEta3Pi_thrown", ";X;Y;", 101, -1.0, 1.0, 101, -1.0, 1.0);

}

Bool_t DSelector_Eta3Pi_Thrown_genEtaRegge::Process(Long64_t locEntry)
{
	// The Process() function is called for each entry in the tree. The entry argument
	// specifies which entry in the currently loaded tree is to be processed.
	//
	// This function should contain the "body" of the analysis. It can contain
	// simple or elaborate selection criteria, run algorithms on the data
	// of the event and typically fill histograms.
	//
	// The processing can be stopped by calling Abort().
	// Use fStatus to set the return value of TTree::Process().
	// The return value is currently not used.

	//CALL THIS FIRST
	DSelector::Process(locEntry); //Gets the data from the tree for the entry
	//cout << "RUN " << Get_RunNumber() << ", EVENT " << Get_EventNumber() << endl;

	/******************************************** GET POLARIZATION ORIENTATION ******************************************/

	//Only if the run number changes
	//RCDB environment must be setup in order for this to work! (Will return false otherwise)
	UInt_t locRunNumber = Get_RunNumber();
	if(locRunNumber != dPreviousRunNumber)
	{
		dIsPolarizedFlag = dAnalysisUtilities.Get_IsPolarizedBeam(locRunNumber, dIsPARAFlag);
		dPreviousRunNumber = locRunNumber;
	}

	/********************************************* SETUP UNIQUENESS TRACKING ********************************************/

	//INSERT USER ANALYSIS UNIQUENESS TRACKING HERE

	/******************************************* LOOP OVER THROWN DATA ***************************************/

	//Thrown beam: just use directly
	double locBeamEnergyUsedForBinning = 0.0;
	if(dThrownBeam != NULL)
		locBeamEnergyUsedForBinning = dThrownBeam->Get_P4().E();

	dFlatTreeInterface->Fill_Fundamental<Double_t>("EnBeam", locBeamEnergyUsedForBinning);
	
	//Loop over throwns
	TLorentzVector locThrownP4RecoilProton;
	TLorentzVector locThrownP4PiPlus;
	TLorentzVector locThrownP4PiMinus;
	TLorentzVector locThrownP4Pi0;
	TLorentzVector locThrownP4Gamma1;
	TLorentzVector locThrownP4Gamma2;
	
	Double_t GammaCounter = 0;
	for(UInt_t loc_i = 0; loc_i < Get_NumThrown(); ++loc_i)
	{
		//Set branch array indices corresponding to this particle
		dThrownWrapper->Set_ArrayIndex(loc_i);

		//Do stuff with the wrapper here ...
		Particle_t locPID = dThrownWrapper->Get_PID();
		TLorentzVector locThrownP4 = dThrownWrapper->Get_P4();
		//pi+
		if (locPID == 8) {
			dFlatTreeInterface->Fill_Fundamental<Double_t>("EnP1", locThrownP4.E());
			dFlatTreeInterface->Fill_Fundamental<Double_t>("PxP1", locThrownP4.Px());
			dFlatTreeInterface->Fill_Fundamental<Double_t>("PyP1", locThrownP4.Py());
			dFlatTreeInterface->Fill_Fundamental<Double_t>("PzP1", locThrownP4.Pz());
			locThrownP4PiPlus = locThrownP4;
		}
		//pi-
		if (locPID == 9) {
			dFlatTreeInterface->Fill_Fundamental<Double_t>("EnP2", locThrownP4.E());
			dFlatTreeInterface->Fill_Fundamental<Double_t>("PxP2", locThrownP4.Px());
			dFlatTreeInterface->Fill_Fundamental<Double_t>("PyP2", locThrownP4.Py());
			dFlatTreeInterface->Fill_Fundamental<Double_t>("PzP2", locThrownP4.Pz());
			locThrownP4PiMinus = locThrownP4;
		}
		//pi0
		// if (locPID == 7) {
		// 	dFlatTreeInterface->Fill_Fundamental<Double_t>("EnP3", locThrownP4.E());
		// 	dFlatTreeInterface->Fill_Fundamental<Double_t>("PxP3", locThrownP4.Px());
		// 	dFlatTreeInterface->Fill_Fundamental<Double_t>("PyP3", locThrownP4.Py());
		// 	dFlatTreeInterface->Fill_Fundamental<Double_t>("PzP3", locThrownP4.Pz());
		// 	locThrownP4Pi0 = locThrownP4;
		// }
		// pi0 -> 2gamma (new MC)
		if (locPID==1) {
			if (GammaCounter==0) locThrownP4Gamma1 = locThrownP4;
			else {
				locThrownP4Gamma2 = locThrownP4;
				locThrownP4Pi0 = locThrownP4Gamma1 + locThrownP4Gamma2;
				dFlatTreeInterface->Fill_Fundamental<Double_t>("EnP3", locThrownP4Pi0.E());
				dFlatTreeInterface->Fill_Fundamental<Double_t>("PxP3", locThrownP4Pi0.Px());
				dFlatTreeInterface->Fill_Fundamental<Double_t>("PyP3", locThrownP4Pi0.Py());
				dFlatTreeInterface->Fill_Fundamental<Double_t>("PzP3", locThrownP4Pi0.Pz());
			}
			GammaCounter++;
		}
		//recoil proton
		if (locPID == 14) {
			locThrownP4RecoilProton = locThrownP4;
		}

		// cout << "Thrown " << loc_i << ": " << locPID << ", " << locThrownP4.Px() << ", " << locThrownP4.Py() << ", " << locThrownP4.Pz() << ", " << locThrownP4.E() << endl;
	}

	//Calculate mandelstam t
	TLorentzVector locThrownP4TargetProton = TLorentzVector(0.0, 0.0, 0.0, 0.938272);
	Double_t locMandelstamT = (locThrownP4RecoilProton - locThrownP4TargetProton).M2();
	dHist_MandelstamT->Fill(locMandelstamT);

	//calculate pi+pi-pi0 invariant mass
	TLorentzVector locPiPlusPiMinusPi0P4 = locThrownP4PiPlus + locThrownP4PiMinus + locThrownP4Pi0;
	Double_t locPiPlusPiMinusPi0Mass = locPiPlusPiMinusPi0P4.M();
	dHist_PiPlusPiMinusPi0Mass->Fill(locPiPlusPiMinusPi0Mass);

	TVector3 BoostVector = locPiPlusPiMinusPi0P4.BoostVector();
	
	TLorentzVector boosted_PiPlus = locThrownP4PiPlus;
	boosted_PiPlus.Boost(-BoostVector);
	TLorentzVector boosted_PiMinus = locThrownP4PiMinus;
	boosted_PiMinus.Boost(-BoostVector);
	TLorentzVector boosted_Pi0 = locThrownP4Pi0;
	boosted_Pi0.Boost(-BoostVector);

	Double_t TPlus = boosted_PiPlus.E() - boosted_PiPlus.M();
	Double_t TMinus = boosted_PiMinus.E() - boosted_PiMinus.M();
	Double_t T0 = boosted_Pi0.E() - boosted_Pi0.M();
	Double_t sumT = TPlus + TMinus + T0;
	Double_t X = TMath::Sqrt(3.0)*(TPlus - TMinus) / sumT;
	Double_t Y = 3.0*T0/sumT - 1.0;

	dHist_Dalitz->Fill(X, Y);


	//Fill flat tree
	dFlatTreeInterface->Fill_Fundamental<Double_t>("t", locMandelstamT);
	dFlatTreeInterface->Fill_Fundamental<Float_t>("weight", 1.0);
	Fill_FlatTree();

	//OR Manually:
	//BEWARE: Do not expect the particles to be at the same array indices from one event to the next!!!!
	//Why? Because while your channel may be the same, the pions/kaons/etc. will decay differently each event.

	//BRANCHES: https://halldweb.jlab.org/wiki/index.php/Analysis_TTreeFormat#TTree_Format:_Simulated_Data
	TClonesArray** locP4Array = dTreeInterface->Get_PointerToPointerTo_TClonesArray("Thrown__P4");
	TBranch* locPIDBranch = dTreeInterface->Get_Branch("Thrown__PID");
/*
	Particle_t locThrown1PID = PDGtoPType(((Int_t*)locPIDBranch->GetAddress())[0]);
	TLorentzVector locThrown1P4 = *((TLorentzVector*)(*locP4Array)->At(0));
	cout << "Particle 1: " << locThrown1PID << ", " << locThrown1P4.Px() << ", " << locThrown1P4.Py() << ", " << locThrown1P4.Pz() << ", " << locThrown1P4.E() << endl;
	Particle_t locThrown2PID = PDGtoPType(((Int_t*)locPIDBranch->GetAddress())[1]);
	TLorentzVector locThrown2P4 = *((TLorentzVector*)(*locP4Array)->At(1));
	cout << "Particle 2: " << locThrown2PID << ", " << locThrown2P4.Px() << ", " << locThrown2P4.Py() << ", " << locThrown2P4.Pz() << ", " << locThrown2P4.E() << endl;
*/




	/******************************************* BIN THROWN DATA INTO SEPARATE TREES FOR AMPTOOLS ***************************************/

/*
	//THESE KEYS MUST BE DEFINED IN THE INIT SECTION (along with the output file names)
	if((locBeamEnergyUsedForBinning >= 8.0) && (locBeamEnergyUsedForBinning < 9.0))
		Fill_OutputTree("Bin1"); //your user-defined key
	else if((locBeamEnergyUsedForBinning >= 9.0) && (locBeamEnergyUsedForBinning < 10.0))
		Fill_OutputTree("Bin2"); //your user-defined key
	else if((locBeamEnergyUsedForBinning >= 10.0) && (locBeamEnergyUsedForBinning < 11.0))
		Fill_OutputTree("Bin3"); //your user-defined key
*/
	return kTRUE;
}

void DSelector_Eta3Pi_Thrown_genEtaRegge::Finalize(void)
{
	//Save anything to output here that you do not want to be in the default DSelector output ROOT file.

	//Otherwise, don't do anything else (especially if you are using PROOF).
		//If you are using PROOF, this function is called on each thread,
		//so anything you do will not have the combined information from the various threads.
		//Besides, it is best-practice to do post-processing (e.g. fitting) separately, in case there is a problem.

	//DO YOUR STUFF HERE

	//CALL THIS LAST
	DSelector::Finalize(); //Saves results to the output file
}
