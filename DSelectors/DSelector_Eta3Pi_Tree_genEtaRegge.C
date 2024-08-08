#include "DSelector_Eta3Pi_Tree_genEtaRegge.h"

void DSelector_Eta3Pi_Tree_genEtaRegge::Init(TTree *locTree)
{
	// USERS: IN THIS FUNCTION, ONLY MODIFY SECTIONS WITH A "USER" OR "EXAMPLE" LABEL. LEAVE THE REST ALONE.

	// The Init() function is called when the selector needs to initialize a new tree or chain.
	// Typically here the branch addresses and branch pointers of the tree will be set.
	// Init() will be called many times when running on PROOF (once per file to be processed).

	//USERS: SET OUTPUT FILE NAME //can be overriden by user in PROOF
	dOutputFileName = "Hist_Eta3Pi_Tree_genEtaRegge.root"; //"" for none
	dOutputTreeFileName = "Tree_Eta3Pi_Tree_genEtaRegge.root"; //"" for none
	dFlatTreeFileName = "Tree_Eta3Pi_Tree_genEtaRegge_flat.root"; //output flat tree (one combo per tree entry), "" for none
	dFlatTreeName = "myTree"; //if blank, default name will be chosen
	//dSaveDefaultFlatBranches = true; // False: don't save default branches, reduce disk footprint.
	//dSaveTLorentzVectorsAsFundamentaFlatTree = false; // Default (or false): save particles as TLorentzVector objects. True: save as four doubles instead.

	//Because this function gets called for each TTree in the TChain, we must be careful:
		//We need to re-initialize the tree interface & branch wrappers, but don't want to recreate histograms
	bool locInitializedPriorFlag = dInitializedFlag; //save whether have been initialized previously
	DSelector::Init(locTree); //This must be called to initialize wrappers for each new TTree
	//gDirectory now points to the output file with name dOutputFileName (if any)
	if(locInitializedPriorFlag)
		return; //have already created histograms, etc. below: exit

	Get_ComboWrappers();
	dPreviousRunNumber = 0;

	/*********************************** EXAMPLE USER INITIALIZATION: ANALYSIS ACTIONS **********************************/

	// EXAMPLE: Create deque for histogramming particle masses:
	// // For histogramming the phi mass in phi -> K+ K-
	// // Be sure to change this and dAnalyzeCutActions to match reaction
	// std::deque<Particle_t> MyPhi;
	// MyPhi.push_back(KPlus); MyPhi.push_back(KMinus);

	std::deque<Particle_t> MyEta;
	MyEta.push_back(PiPlus); MyEta.push_back(PiMinus); MyEta.push_back(Pi0);

	//ANALYSIS ACTIONS: //Executed in order if added to dAnalysisActions
	//false/true below: use measured/kinfit data

	//PID
	dAnalysisActions.push_back(new DHistogramAction_ParticleID(dComboWrapper, false));
	//below: value: +/- N ns, Unknown: All PIDs, SYS_NULL: all timing systems
	//dAnalysisActions.push_back(new DCutAction_PIDDeltaT(dComboWrapper, false, 0.5, KPlus, SYS_BCAL));

	//PIDFOM (for charged tracks)
	dAnalysisActions.push_back(new DHistogramAction_PIDFOM(dComboWrapper));
	//dAnalysisActions.push_back(new DCutAction_PIDFOM(dComboWrapper, KPlus, 0.1));
	//dAnalysisActions.push_back(new DCutAction_EachPIDFOM(dComboWrapper, 0.1));

	//MASSES
	dAnalysisActions.push_back(new DHistogramAction_InvariantMass(dComboWrapper, true, PiPlus, 100, 0.1, 0.15, "PiPlus"));
	dAnalysisActions.push_back(new DHistogramAction_InvariantMass(dComboWrapper, true, PiMinus, 100, 0.1, 0.15, "PiMinus"));
	dAnalysisActions.push_back(new DHistogramAction_InvariantMass(dComboWrapper, true, Pi0, 100, 0.1, 0.15, "Pi0"));
	dAnalysisActions.push_back(new DHistogramAction_InvariantMass(dComboWrapper, true, 1, MyEta, 100, 0.0, 1.0, "Eta"));
	//dAnalysisActions.push_back(new DHistogramAction_MissingMassSquared(dComboWrapper, false, 1000, -0.1, 0.1));

	//KINFIT RESULTS
	dAnalysisActions.push_back(new DHistogramAction_KinFitResults(dComboWrapper));

	//CUT MISSING MASS
	//dAnalysisActions.push_back(new DCutAction_MissingMassSquared(dComboWrapper, false, -0.03, 0.02));

	//CUT ON SHOWER QUALITY
	//dAnalysisActions.push_back(new DCutAction_ShowerQuality(dComboWrapper, SYS_FCAL, 0.5));

	//BEAM ENERGY
	dAnalysisActions.push_back(new DHistogramAction_BeamEnergy(dComboWrapper, false));
	// dAnalysisActions.push_back(new DCutAction_BeamEnergy(dComboWrapper, false, 8.2, 8.8));  // Coherent peak for runs in the range 30000-59999
	// dAnalysisActions.push_back(new DCutAction_BeamEnergy(dComboWrapper, true, 6.5, 12.0));  // ccdb beam flux 2017

	//KINEMATICS
	dAnalysisActions.push_back(new DHistogramAction_ParticleComboKinematics(dComboWrapper, false));

	// ANALYZE CUT ACTIONS
	// // Change MyPhi to match reaction
	dAnalyzeCutActions = new DHistogramAction_AnalyzeCutActions( dAnalysisActions, dComboWrapper, false, 0, MyEta, 1000, 0.0, 1.0, "CutActionEffect" );

	//INITIALIZE ACTIONS
	//If you create any actions that you want to run manually (i.e. don't add to dAnalysisActions), be sure to initialize them here as well
	Initialize_Actions();
	dAnalyzeCutActions->Initialize(); // manual action, must call Initialize()

	/******************************** EXAMPLE USER INITIALIZATION: STAND-ALONE HISTOGRAMS *******************************/

	//EXAMPLE MANUAL HISTOGRAMS:
	dHist_MissingMassSquared = new TH1I("MissingMassSquared", ";Missing Mass Squared (GeV/c^{2})^{2}", 600, -0.06, 0.06);
	dHist_BeamEnergy = new TH1I("BeamEnergy", ";Beam Energy (GeV)", 600, 0.0, 12.0);

	/************************** EXAMPLE USER INITIALIZATION: CUSTOM OUTPUT BRANCHES - MAIN TREE *************************/

	//EXAMPLE MAIN TREE CUSTOM BRANCHES (OUTPUT ROOT FILE NAME MUST FIRST BE GIVEN!!!! (ABOVE: TOP)):
	//The type for the branch must be included in the brackets
	//1st function argument is the name of the branch
	//2nd function argument is the name of the branch that contains the size of the array (for fundamentals only)
	/*
	dTreeInterface->Create_Branch_Fundamental<Int_t>("my_int"); //fundamental = char, int, float, double, etc.
	dTreeInterface->Create_Branch_FundamentalArray<Int_t>("my_int_array", "my_int");
	dTreeInterface->Create_Branch_FundamentalArray<Float_t>("my_combo_array", "NumCombos");
	dTreeInterface->Create_Branch_NoSplitTObject<TLorentzVector>("my_p4");
	dTreeInterface->Create_Branch_ClonesArray<TLorentzVector>("my_p4_array");
	*/

	/************************** EXAMPLE USER INITIALIZATION: CUSTOM OUTPUT BRANCHES - FLAT TREE *************************/

	// RECOMMENDED: CREATE ACCIDENTAL WEIGHT BRANCH
	// dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("accidweight");

	//EXAMPLE FLAT TREE CUSTOM BRANCHES (OUTPUT ROOT FILE NAME MUST FIRST BE GIVEN!!!! (ABOVE: TOP)):
	//The type for the branch must be included in the brackets
	//1st function argument is the name of the branch
	//2nd function argument is the name of the branch that contains the size of the array (for fundamentals only)

	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("kfit_prob");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("mimass");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("imass");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("imassGG");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("mimass_kfit");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("imass_kfit");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("imassGG_kfit");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("missingP");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("missingE");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("imassPiPi");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("mimassPiPi");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("imassPiPi_kfit");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("mimassPiPi_kfit");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("rVertex");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("zVertex");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("rVertex_decay");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("zVertex_decay");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("tSlope_rec");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("tSlope_fit");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("dBRT");
	
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("TPlus");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("TMinus");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("TZero");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("TAll");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("DP_X");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("DP_Y");
	
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("TZero_mass");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("TAll_mass");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("DP_X_mass");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("DP_Y_mass");
	
	dFlatTreeInterface->Create_Branch_Fundamental<Int_t>("NDP_bins");
	dFlatTreeInterface->Create_Branch_FundamentalArray<Int_t>("N_DP_Bins","NDP_bins");
	dFlatTreeInterface->Create_Branch_FundamentalArray<Int_t>("Global_DP_Bin","NDP_bins");
	dFlatTreeInterface->Create_Branch_FundamentalArray<Int_t>("Global_DP_Bin_mass","NDP_bins");
    
    dIsMC = (dTreeInterface->Get_Branch("MCWeight") != NULL);
    
    if(dIsMC){
        dFlatTreeInterface->Create_Branch_NoSplitTObject<TLorentzVector>("thrown_beam_p4");
        dFlatTreeInterface->Create_Branch_NoSplitTObject<TLorentzVector>("thrown_proton_p4");
        dFlatTreeInterface->Create_Branch_NoSplitTObject<TLorentzVector>("thrown_piplus_p4");
        dFlatTreeInterface->Create_Branch_NoSplitTObject<TLorentzVector>("thrown_piminus_p4");
        dFlatTreeInterface->Create_Branch_NoSplitTObject<TLorentzVector>("thrown_gamma1_p4");
        dFlatTreeInterface->Create_Branch_NoSplitTObject<TLorentzVector>("thrown_gamma2_p4");
        
        /*
        dFlatTreeInterface->Create_Branch_ClonesArray<TLorentzVector>("thrown_beam_p4");
        dFlatTreeInterface->Create_Branch_ClonesArray<TLorentzVector>("thrown_proton_p4");
        dFlatTreeInterface->Create_Branch_ClonesArray<TLorentzVector>("thrown_piplus_p4");
        dFlatTreeInterface->Create_Branch_ClonesArray<TLorentzVector>("thrown_piminus_p4");
        dFlatTreeInterface->Create_Branch_ClonesArray<TLorentzVector>("thrown_gamma1_p4");
        dFlatTreeInterface->Create_Branch_ClonesArray<TLorentzVector>("thrown_gamma2_p4");
         */
    }

	/*
	dFlatTreeInterface->Create_Branch_Fundamental<Int_t>("flat_my_int"); //fundamental = char, int, float, double, etc.
	dFlatTreeInterface->Create_Branch_FundamentalArray<Int_t>("flat_my_int_array", "flat_my_int");
	dFlatTreeInterface->Create_Branch_NoSplitTObject<TLorentzVector>("flat_my_p4");
	dFlatTreeInterface->Create_Branch_ClonesArray<TLorentzVector>("flat_my_p4_array");
	*/

	/************************************* ADVANCED EXAMPLE: CHOOSE BRANCHES TO READ ************************************/

	//TO SAVE PROCESSING TIME
		//If you know you don't need all of the branches/data, but just a subset of it, you can speed things up
		//By default, for each event, the data is retrieved for all branches
		//If you know you only need data for some branches, you can skip grabbing data from the branches you don't need
		//Do this by doing something similar to the commented code below

	//dTreeInterface->Clear_GetEntryBranches(); //now get none
	//dTreeInterface->Register_GetEntryBranch("Proton__P4"); //manually set the branches you want

	/************************************** DETERMINE IF ANALYZING SIMULATED DATA *************************************/

	dIsMC = (dTreeInterface->Get_Branch("MCWeight") != NULL);

}

Bool_t DSelector_Eta3Pi_Tree_genEtaRegge::Process(Long64_t locEntry)
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
	//TLorentzVector locProductionX4 = Get_X4_Production();

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

	//ANALYSIS ACTIONS: Reset uniqueness tracking for each action
	//For any actions that you are executing manually, be sure to call Reset_NewEvent() on them here
	Reset_Actions_NewEvent();
	dAnalyzeCutActions->Reset_NewEvent(); // manual action, must call Reset_NewEvent()

	//PREVENT-DOUBLE COUNTING WHEN HISTOGRAMMING
		//Sometimes, some content is the exact same between one combo and the next
			//e.g. maybe two combos have different beam particles, but the same data for the final-state
		//When histogramming, you don't want to double-count when this happens: artificially inflates your signal (or background)
		//So, for each quantity you histogram, keep track of what particles you used (for a given combo)
		//Then for each combo, just compare to what you used before, and make sure it's unique

	//EXAMPLE 1: Particle-specific info:
	set<Int_t> locUsedSoFar_BeamEnergy; //Int_t: Unique ID for beam particles. set: easy to use, fast to search

	//EXAMPLE 2: Combo-specific info:
		//In general: Could have multiple particles with the same PID: Use a set of Int_t's
		//In general: Multiple PIDs, so multiple sets: Contain within a map
		//Multiple combos: Contain maps within a set (easier, faster to search)
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_MissingMass;
	// set<map<Particle_t, set<Int_t> > > locUsedSoFar_Topology;
	// bool usedTopology = false;

	//INSERT USER ANALYSIS UNIQUENESS TRACKING HERE

	/**************************************** EXAMPLE: FILL CUSTOM OUTPUT BRANCHES **************************************/

	/*
	Int_t locMyInt = 7;
	dTreeInterface->Fill_Fundamental<Int_t>("my_int", locMyInt);

	TLorentzVector locMyP4(4.0, 3.0, 2.0, 1.0);
	dTreeInterface->Fill_TObject<TLorentzVector>("my_p4", locMyP4);

	for(int loc_i = 0; loc_i < locMyInt; ++loc_i)
		dTreeInterface->Fill_Fundamental<Int_t>("my_int_array", 3*loc_i, loc_i); //2nd argument = value, 3rd = array index
	*/

	//Loop over the throwns:
    if(dIsMC){
        if(dThrownBeam != NULL)thrown_beam_p4 = dThrownBeam->Get_P4();
        
        Int_t chose_gamma = 0;
       //+++++++++++++++++++++++++++++++++++++++++++
       for(UInt_t t=0;t<Get_NumThrown();t++){
           dThrownWrapper->Set_ArrayIndex(t);
           Int_t current_id = dThrownWrapper->Get_PID();
           //-------------------------------------------
           if(current_id == 14){
               thrown_proton_p4 = dThrownWrapper->Get_P4();
           }else if(current_id == 8){
               thrown_piplus_p4 = dThrownWrapper->Get_P4();
           }else if(current_id == 9){
	       thrown_piminus_p4 = dThrownWrapper->Get_P4();
           }else if(current_id == 1){
               if(chose_gamma == 0){
                   thrown_gamma1_p4 = dThrownWrapper->Get_P4();
               }else thrown_gamma2_p4 = dThrownWrapper->Get_P4();
               chose_gamma++;
           }
           //-------------------------------------------
       }
       //+++++++++++++++++++++++++++++++++++++++++++
    }

	/************************************************* LOOP OVER COMBOS *************************************************/

	//Loop over combos
	for(UInt_t loc_i = 0; loc_i < Get_NumCombos(); ++loc_i)
	{
		//Set branch array indices for combo and all combo particles
		dComboWrapper->Set_ComboIndex(loc_i);

		// Is used to indicate when combos have been cut
		if(dComboWrapper->Get_IsComboCut()) // Is false when tree originally created
			continue; // Combo has been cut previously

		/********************************************** GET PARTICLE INDICES *********************************************/

		//Used for tracking uniqueness when filling histograms, and for determining unused particles

		//Step 0
		Int_t locBeamID = dComboBeamWrapper->Get_BeamID();
		Int_t locProtonTrackID = dProtonWrapper->Get_TrackID();

		//Step 1
		Int_t locPiPlusTrackID = dPiPlusWrapper->Get_TrackID();
		Int_t locPiMinusTrackID = dPiMinusWrapper->Get_TrackID();

		//Step 2
		Int_t locPhoton1NeutralID = dPhoton1Wrapper->Get_NeutralID();
		Int_t locPhoton2NeutralID = dPhoton2Wrapper->Get_NeutralID();

		/*********************************************** GET FOUR-MOMENTUM **********************************************/

		// Get P4's: //is kinfit if kinfit performed, else is measured
		//dTargetP4 is target p4
		//Step 0
		TLorentzVector locBeamP4 = dComboBeamWrapper->Get_P4();
		TLorentzVector locProtonP4 = dProtonWrapper->Get_P4();
		//Step 1
		TLorentzVector locPiPlusP4 = dPiPlusWrapper->Get_P4();
		TLorentzVector locPiMinusP4 = dPiMinusWrapper->Get_P4();
		//Step 2
		// TLorentzVector locDecayingPi0P4 = dDecayingPi0Wrapper->Get_P4();
		TLorentzVector locPhoton1P4 = dPhoton1Wrapper->Get_P4();
		TLorentzVector locPhoton2P4 = dPhoton2Wrapper->Get_P4();

		// Get Measured P4's:
		//Step 0
		TLorentzVector locBeamP4_Measured = dComboBeamWrapper->Get_P4_Measured();
		TLorentzVector locProtonP4_Measured = dProtonWrapper->Get_P4_Measured();
		//Step 1
		TLorentzVector locPiPlusP4_Measured = dPiPlusWrapper->Get_P4_Measured();
		TLorentzVector locPiMinusP4_Measured = dPiMinusWrapper->Get_P4_Measured();
		//Step 2
		TLorentzVector locPhoton1P4_Measured = dPhoton1Wrapper->Get_P4_Measured();
		TLorentzVector locPhoton2P4_Measured = dPhoton2Wrapper->Get_P4_Measured();

		/********************************************* GET COMBO RF TIMING INFO *****************************************/

		TLorentzVector locBeamX4_Measured = dComboBeamWrapper->Get_X4_Measured();
        TLorentzVector locProtonX4_Measured = dProtonWrapper->Get_X4_Measured();
		TLorentzVector locPiPlusX4_Measured = dPiPlusWrapper->Get_X4_Measured();
        TLorentzVector locPiMinusX4_Measured = dPiMinusWrapper->Get_X4_Measured();
        TLorentzVector locPhoton1X4_Measured = dPhoton1Wrapper->Get_X4_Measured();
        TLorentzVector locPhoton2X4_Measured = dPhoton2Wrapper->Get_X4_Measured();
		//Double_t locBunchPeriod = dAnalysisUtilities.Get_BeamBunchPeriod(Get_RunNumber());
		// Double_t locDeltaT_RF = dAnalysisUtilities.Get_DeltaT_RF(Get_RunNumber(), locBeamX4_Measured, dComboWrapper);
		// Int_t locRelBeamBucket = dAnalysisUtilities.Get_RelativeBeamBucket(Get_RunNumber(), locBeamX4_Measured, dComboWrapper); // 0 for in-time events, non-zero integer for out-of-time photons
		// Int_t locNumOutOfTimeBunchesInTree = XXX; //YOU need to specify this number
			//Number of out-of-time beam bunches in tree (on a single side, so that total number out-of-time bunches accepted is 2 times this number for left + right bunches) 

		// Bool_t locSkipNearestOutOfTimeBunch = true; // True: skip events from nearest out-of-time bunch on either side (recommended).
		// Int_t locNumOutOfTimeBunchesToUse = locSkipNearestOutOfTimeBunch ? locNumOutOfTimeBunchesInTree-1:locNumOutOfTimeBunchesInTree; 
		// Double_t locAccidentalScalingFactor = dAnalysisUtilities.Get_AccidentalScalingFactor(Get_RunNumber(), locBeamP4.E(), dIsMC); // Ideal value would be 1, but deviations require added factor, which is different for data and MC.
		// Double_t locAccidentalScalingFactorError = dAnalysisUtilities.Get_AccidentalScalingFactorError(Get_RunNumber(), locBeamP4.E()); // Ideal value would be 1, but deviations observed, need added factor.
		// Double_t locHistAccidWeightFactor = locRelBeamBucket==0 ? 1 : -locAccidentalScalingFactor/(2*locNumOutOfTimeBunchesToUse) ; // Weight by 1 for in-time events, ScalingFactor*(1/NBunches) for out-of-time
		// if(locSkipNearestOutOfTimeBunch && abs(locRelBeamBucket)==1) { // Skip nearest out-of-time bunch: tails of in-time distribution also leak in
		// 	dComboWrapper->Set_IsComboCut(true); 
		// 	continue; 
		// } 

		/********************************************* COMBINE FOUR-MOMENTUM ********************************************/

		// DO YOUR STUFF HERE

		//Get the dbrt:
        dBRT = locBeamX4_Measured.T() - dComboWrapper->Get_RFTime_Measured();

		// Combine 4-vectors
		TLorentzVector locMissingP4_Measured = locBeamP4_Measured + dTargetP4;
		locMissingP4_Measured -= locPiPlusP4_Measured + locPiMinusP4_Measured + locPhoton1P4_Measured + locPhoton2P4_Measured;

		missingP = locMissingP4_Measured.P();
        missingE = locMissingP4_Measured.E();
        
        em_balance = missingP - TMath::Abs(missingE);
        mimass_all = locMissingP4_Measured.M2();

        mimassVec = locBeamP4_Measured + dTargetP4 - locProtonP4_Measured;
        imassVec = locPiPlusP4_Measured + locPiMinusP4_Measured + locPhoton1P4_Measured + locPhoton2P4_Measured;
        imassGGVec = locPhoton1P4_Measured + locPhoton2P4_Measured;
        imassPiPiVec = locPiPlusP4_Measured + locPiMinusP4_Measured;
        mimassPiPiVec = mimassVec - imassPiPiVec;
        
        mimass = mimassVec.M();
        imass = imassVec.M();
        imassGG = imassGGVec.M();
        mimassPiPi = mimassPiPiVec.M2();
        imassPiPi = imassPiPiVec.M();
        Double_t tSlope_rec = (locProtonP4_Measured - dTargetP4).M2();
        
        kfit_prob = TMath::Prob(dComboWrapper->Get_ChiSq_KinFit(),dComboWrapper->Get_NDF_KinFit());
        mimassVec_kfit = locBeamP4 + dTargetP4 - locProtonP4;
        imassVec_kfit = locPiPlusP4 + locPiMinusP4 + locPhoton1P4 + locPhoton2P4;
        imassGGVec_kfit = locPhoton1P4 + locPhoton2P4;
        imassPiPiVec_kfit = locPiPlusP4 + locPiMinusP4;
        mimassPiPiVec_kfit = mimassVec_kfit - imassPiPiVec_kfit;
        
        mimass_kfit = mimassVec_kfit.M();
        imass_kfit = imassVec_kfit.M();
        imassGG_kfit = imassGGVec_kfit.M();
        mimassPiPi_kfit = mimassPiPiVec_kfit.M2();
        imassPiPi_kfit = imassPiPiVec_kfit.M();
        Double_t tSlope_fit = (locProtonP4 - dTargetP4).M2();
        
        TLorentzVector vertexVec_decay = locProtonX4_Measured + locPiPlusX4_Measured + locPiMinusX4_Measured + locPhoton1X4_Measured + locPhoton2X4_Measured;
        Double_t missX_decay = vertexVec_decay.X();
        Double_t missY_decay = vertexVec_decay.Y();
        Double_t missZ_decay = vertexVec_decay.Z();
        Double_t rVertex_decay = TMath::Sqrt(missX_decay*missX_decay + missY_decay*missY_decay);
        
        TLorentzVector vertexVec = (locBeamX4_Measured + dTargetP4) - vertexVec_decay;
        Double_t missX = vertexVec.X();
        Double_t missY = vertexVec.Y();
        Double_t missZ = vertexVec.Z();
        Double_t rVertex = TMath::Sqrt(missX*missX + missY*missY);
        
        
        //Look at various opening angles:
        //##################################################################
		boostVectorEta = mimassVec.BoostVector();
		boostVectorCM = (locBeamP4 + dTargetP4).BoostVector();
		
		boostedPip = locPiPlusP4;
		boostedPip.Boost(boostVectorEta*(-1));
		boostedPim = locPiMinusP4;
		boostedPim.Boost(boostVectorEta*(-1));
		opangPiPi = TMath::Cos( (boostedPip.Vect()).Angle(boostedPim.Vect()) );
		
		boostedEta = mimassVec;
		boostedEta.Boost(boostVectorCM*(-1));
		opangProd = TMath::Cos( (boostedEta.Vect()).Theta() );
		//##################################################################
		
		//Do some DP calculations:
		//##################################################################
		boostVectorEta_KFit = mimassVec_kfit.BoostVector();
		
		pip_etaFrame = locPiPlusP4;
		pip_etaFrame.Boost(boostVectorEta_KFit*(-1));
		pim_etaFrame = locPiMinusP4;
		pim_etaFrame.Boost(boostVectorEta_KFit*(-1));
		pi0_etaFrame = imassGGVec_kfit;
		pi0_etaFrame.Boost(boostVectorEta_KFit*(-1));
		
		TPlus = pip_etaFrame.E() - pip_etaFrame.M();
		TMinus = pim_etaFrame.E() - pim_etaFrame.M();
		TZero = pi0_etaFrame.E() - pi0_etaFrame.M();
		TAll = TPlus + TMinus + TZero;
		
		DP_X = TMath::Sqrt(3)*(TPlus - TMinus)/TAll;
		DP_Y = 3*TZero/TAll - 1.0;
		
		//Alternative calculation of DP-Variables:
		TZero_mass = mimassVec_kfit.M() - TPlus - TMinus - pip_etaFrame.M() - pim_etaFrame.M() - imassGGVec_kfit.M();
		TAll_mass = TPlus + TMinus + TZero_mass;
		DP_X_mass = TMath::Sqrt(3)*(TPlus - TMinus)/TAll_mass;
		DP_Y_mass = 3*TZero_mass/TAll_mass - 1.0;
		

		/******************************************** EXECUTE ANALYSIS ACTIONS *******************************************/

		// Loop through the analysis actions, executing them in order for the active particle combo
		dAnalyzeCutActions->Perform_Action(); // Must be executed before Execute_Actions()
		if(!Execute_Actions()) //if the active combo fails a cut, IsComboCutFlag automatically set
			continue;

		//if you manually execute any actions, and it fails a cut, be sure to call:
			//dComboWrapper->Set_IsComboCut(true);

		/**************************************** EXAMPLE: FILL CUSTOM OUTPUT BRANCHES **************************************/

		/*
		TLorentzVector locMyComboP4(8.0, 7.0, 6.0, 5.0);
		//for arrays below: 2nd argument is value, 3rd is array index
		//NOTE: By filling here, AFTER the cuts above, some indices won't be updated (and will be whatever they were from the last event)
			//So, when you draw the branch, be sure to cut on "IsComboCut" to avoid these.
		dTreeInterface->Fill_Fundamental<Float_t>("my_combo_array", -2*loc_i, loc_i);
		dTreeInterface->Fill_TObject<TLorentzVector>("my_p4_array", locMyComboP4, loc_i);
		*/

		/**************************************** EXAMPLE: HISTOGRAM BEAM ENERGY *****************************************/

		//Histogram beam energy (if haven't already)
		if(locUsedSoFar_BeamEnergy.find(locBeamID) == locUsedSoFar_BeamEnergy.end())
		{
			dHist_BeamEnergy->Fill(locBeamP4.E()); // Fills in-time and out-of-time beam photon combos
			//dHist_BeamEnergy->Fill(locBeamP4.E(),locHistAccidWeightFactor); // Alternate version with accidental subtraction

			locUsedSoFar_BeamEnergy.insert(locBeamID);
		}

		/************************************ EXAMPLE: HISTOGRAM MISSING MASS SQUARED ************************************/

		//Missing Mass Squared
		double locMissingMassSquared = locMissingP4_Measured.M2();

		//Uniqueness tracking: Build the map of particles used for the missing mass
			//For beam: Don't want to group with final-state photons. Instead use "Unknown" PID (not ideal, but it's easy).
		map<Particle_t, set<Int_t> > locUsedThisCombo_MissingMass;
		locUsedThisCombo_MissingMass[Unknown].insert(locBeamID); //beam
		locUsedThisCombo_MissingMass[Proton].insert(locProtonTrackID);
		locUsedThisCombo_MissingMass[PiPlus].insert(locPiPlusTrackID);
		locUsedThisCombo_MissingMass[PiMinus].insert(locPiMinusTrackID);
		locUsedThisCombo_MissingMass[Gamma].insert(locPhoton1NeutralID);
		locUsedThisCombo_MissingMass[Gamma].insert(locPhoton2NeutralID);

		//compare to what's been used so far
		if(locUsedSoFar_MissingMass.find(locUsedThisCombo_MissingMass) == locUsedSoFar_MissingMass.end())
		{
			//unique missing mass combo: histogram it, and register this combo of particles
			dHist_MissingMassSquared->Fill(locMissingMassSquared); // Fills in-time and out-of-time beam photon combos
			//dHist_MissingMassSquared->Fill(locMissingMassSquared,locHistAccidWeightFactor); // Alternate version with accidental subtraction

			locUsedSoFar_MissingMass.insert(locUsedThisCombo_MissingMass);
		}

		// map<Particle_t, set<Int_t> > locUsedThisCombo_Topology;
		// locUsedThisCombo_Topology[Unknown].insert(locBeamID); //beam
		// locUsedThisCombo_Topology[Proton].insert(locProtonTrackID);
		// locUsedThisCombo_Topology[PiPlus].insert(locPiPlusTrackID);
		// locUsedThisCombo_Topology[PiMinus].insert(locPiMinusTrackID);
		// locUsedThisCombo_Topology[Gamma].insert(locPhoton1NeutralID);
		// locUsedThisCombo_Topology[Gamma].insert(locPhoton2NeutralID);

		// if(locUsedSoFar_Topology.find(locUsedThisCombo_Topology) == locUsedSoFar_Topology.end())
		// {
		// 	locUsedSoFar_Topology.insert(locUsedThisCombo_Topology);
		// 	usedTopology = true;
		// }else usedTopology = false;

		if ((missZ<-320) || (missZ>-200)) continue;

		if(kfit_prob > 0.0){
				 dFlatTreeInterface->Fill_Fundamental<Double_t>("kfit_prob", kfit_prob);
				 dFlatTreeInterface->Fill_Fundamental<Double_t>("mimass", mimass);
				 dFlatTreeInterface->Fill_Fundamental<Double_t>("imass", imass);
				 dFlatTreeInterface->Fill_Fundamental<Double_t>("imassGG", imassGG);
				 dFlatTreeInterface->Fill_Fundamental<Double_t>("mimass_kfit", mimass_kfit);
				 dFlatTreeInterface->Fill_Fundamental<Double_t>("imass_kfit", imass_kfit);
				 dFlatTreeInterface->Fill_Fundamental<Double_t>("imassGG_kfit", imassGG_kfit);
				 dFlatTreeInterface->Fill_Fundamental<Double_t>("missingP", missingP);
				 dFlatTreeInterface->Fill_Fundamental<Double_t>("missingE", missingE);
                 dFlatTreeInterface->Fill_Fundamental<Double_t>("imassPiPi", imassPiPi);
                 dFlatTreeInterface->Fill_Fundamental<Double_t>("mimassPiPi", mimassPiPi);
                 dFlatTreeInterface->Fill_Fundamental<Double_t>("imassPiPi_kfit", imassPiPi_kfit);
                 dFlatTreeInterface->Fill_Fundamental<Double_t>("mimassPiPi_kfit", mimassPiPi_kfit);
                 dFlatTreeInterface->Fill_Fundamental<Double_t>("rVertex", rVertex);
                 dFlatTreeInterface->Fill_Fundamental<Double_t>("zVertex", missZ);
                 dFlatTreeInterface->Fill_Fundamental<Double_t>("rVertex_decay", rVertex_decay);
                 dFlatTreeInterface->Fill_Fundamental<Double_t>("zVertex_decay", missZ_decay);
                 dFlatTreeInterface->Fill_Fundamental<Double_t>("tSlope_rec", tSlope_rec);
                 dFlatTreeInterface->Fill_Fundamental<Double_t>("tSlope_fit", tSlope_fit);
                 dFlatTreeInterface->Fill_Fundamental<Double_t>("dBRT", dBRT);
				 
				 dFlatTreeInterface->Fill_Fundamental<Double_t>("TPlus", TPlus);
				 dFlatTreeInterface->Fill_Fundamental<Double_t>("TMinus", TMinus);
				 dFlatTreeInterface->Fill_Fundamental<Double_t>("TZero", TZero);
				 dFlatTreeInterface->Fill_Fundamental<Double_t>("TAll", TAll);
				 dFlatTreeInterface->Fill_Fundamental<Double_t>("DP_X", DP_X);
				 dFlatTreeInterface->Fill_Fundamental<Double_t>("DP_Y", DP_Y);
				 dFlatTreeInterface->Fill_Fundamental<Int_t>("NDP_bins", NDP_bins);
				 
				 dFlatTreeInterface->Fill_Fundamental<Double_t>("TZero_mass", TZero_mass);
				 dFlatTreeInterface->Fill_Fundamental<Double_t>("TAll_mass", TAll_mass);
				 dFlatTreeInterface->Fill_Fundamental<Double_t>("DP_X_mass", DP_X_mass);
				 dFlatTreeInterface->Fill_Fundamental<Double_t>("DP_Y_mass", DP_Y_mass);
				 
                 if(dIsMC){
                     dFlatTreeInterface->Fill_TObject<TLorentzVector>("thrown_beam_p4",thrown_beam_p4);
                     dFlatTreeInterface->Fill_TObject<TLorentzVector>("thrown_proton_p4",thrown_proton_p4);
                     dFlatTreeInterface->Fill_TObject<TLorentzVector>("thrown_piplus_p4",thrown_piplus_p4);
                     dFlatTreeInterface->Fill_TObject<TLorentzVector>("thrown_piminus_p4",thrown_piminus_p4);
                     dFlatTreeInterface->Fill_TObject<TLorentzVector>("thrown_gamma1_p4",thrown_gamma1_p4);
                     dFlatTreeInterface->Fill_TObject<TLorentzVector>("thrown_gamma2_p4",thrown_gamma2_p4);
                 }
            
				 //cout <<"TZero: "<<TZero<<" and TZero_mass: "<<TZero_mass<< endl;
				 
				 //*************************************
				 for(Int_t k=0;k<NDP_bins;k++){
					  current_delta = DP_delta/NBins[k];
		              Global_Bin[k] = TMath::FloorNint( (DP_X + DP_max)/current_delta ) + NBins[k]*TMath::FloorNint( (DP_Y + DP_max)/current_delta );	
		              
		              dFlatTreeInterface->Fill_Fundamental<Int_t>("N_DP_Bins",NBins[k],k);
		              dFlatTreeInterface->Fill_Fundamental<Int_t>("Global_DP_Bin",Global_Bin[k],k);
		              
		              Global_Bin_mass[k] = TMath::FloorNint( (DP_X_mass + DP_max)/current_delta ) + NBins[k]*TMath::FloorNint( (DP_Y_mass + DP_max)/current_delta );
		              dFlatTreeInterface->Fill_Fundamental<Int_t>("Global_DP_Bin_mass",Global_Bin_mass[k],k);
		          }    
		         //*************************************
		}

		//E.g. Cut
		//if((locMissingMassSquared < -0.04) || (locMissingMassSquared > 0.04))
		//{
		//	dComboWrapper->Set_IsComboCut(true);
		//	continue;
		//}

		/****************************************** FILL FLAT TREE (IF DESIRED) ******************************************/

		// RECOMMENDED: FILL ACCIDENTAL WEIGHT
		// dFlatTreeInterface->Fill_Fundamental<Double_t>("accidweight",locHistAccidWeightFactor);

		/*
		//FILL ANY CUSTOM BRANCHES FIRST!!
		Int_t locMyInt_Flat = 7;
		dFlatTreeInterface->Fill_Fundamental<Int_t>("flat_my_int", locMyInt_Flat);

		TLorentzVector locMyP4_Flat(4.0, 3.0, 2.0, 1.0);
		dFlatTreeInterface->Fill_TObject<TLorentzVector>("flat_my_p4", locMyP4_Flat);

		for(int loc_j = 0; loc_j < locMyInt_Flat; ++loc_j)
		{
			dFlatTreeInterface->Fill_Fundamental<Int_t>("flat_my_int_array", 3*loc_j, loc_j); //2nd argument = value, 3rd = array index
			TLorentzVector locMyComboP4_Flat(8.0, 7.0, 6.0, 5.0);
			dFlatTreeInterface->Fill_TObject<TLorentzVector>("flat_my_p4_array", locMyComboP4_Flat, loc_j);
		}
		*/

		//FILL FLAT TREE
		Fill_FlatTree(); //for the active combo
	} // end of combo loop

	//FILL HISTOGRAMS: Num combos / events surviving actions
	Fill_NumCombosSurvivedHists();

	/******************************************* LOOP OVER THROWN DATA (OPTIONAL) ***************************************/
/*
	//Thrown beam: just use directly
	if(dThrownBeam != NULL)
		double locEnergy = dThrownBeam->Get_P4().E();

	//Loop over throwns
	for(UInt_t loc_i = 0; loc_i < Get_NumThrown(); ++loc_i)
	{
		//Set branch array indices corresponding to this particle
		dThrownWrapper->Set_ArrayIndex(loc_i);

		//Do stuff with the wrapper here ...
	}
*/
	/****************************************** LOOP OVER OTHER ARRAYS (OPTIONAL) ***************************************/
/*
	//Loop over beam particles (note, only those appearing in combos are present)
	for(UInt_t loc_i = 0; loc_i < Get_NumBeam(); ++loc_i)
	{
		//Set branch array indices corresponding to this particle
		dBeamWrapper->Set_ArrayIndex(loc_i);

		//Do stuff with the wrapper here ...
	}

	//Loop over charged track hypotheses
	for(UInt_t loc_i = 0; loc_i < Get_NumChargedHypos(); ++loc_i)
	{
		//Set branch array indices corresponding to this particle
		dChargedHypoWrapper->Set_ArrayIndex(loc_i);

		//Do stuff with the wrapper here ...
	}

	//Loop over neutral particle hypotheses
	for(UInt_t loc_i = 0; loc_i < Get_NumNeutralHypos(); ++loc_i)
	{
		//Set branch array indices corresponding to this particle
		dNeutralHypoWrapper->Set_ArrayIndex(loc_i);

		//Do stuff with the wrapper here ...
	}
*/

	/************************************ EXAMPLE: FILL CLONE OF TTREE HERE WITH CUTS APPLIED ************************************/
/*
	Bool_t locIsEventCut = true;
	for(UInt_t loc_i = 0; loc_i < Get_NumCombos(); ++loc_i) {
		//Set branch array indices for combo and all combo particles
		dComboWrapper->Set_ComboIndex(loc_i);
		// Is used to indicate when combos have been cut
		if(dComboWrapper->Get_IsComboCut())
			continue;
		locIsEventCut = false; // At least one combo succeeded
		break;
	}
	if(!locIsEventCut && dOutputTreeFileName != "")
		Fill_OutputTree();
*/

	return kTRUE;
}

void DSelector_Eta3Pi_Tree_genEtaRegge::Finalize(void)
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
