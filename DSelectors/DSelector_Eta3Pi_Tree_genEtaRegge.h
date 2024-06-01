#ifndef DSelector_Eta3Pi_Tree_genEtaRegge_h
#define DSelector_Eta3Pi_Tree_genEtaRegge_h

#include <iostream>

#include "DSelector/DSelector.h"
#include "DSelector/DHistogramActions.h"
#include "DSelector/DCutActions.h"

#include "TH1I.h"
#include "TH2I.h"

class DSelector_Eta3Pi_Tree_genEtaRegge : public DSelector
{
	public:

		DSelector_Eta3Pi_Tree_genEtaRegge(TTree* locTree = NULL) : DSelector(locTree){}
		virtual ~DSelector_Eta3Pi_Tree_genEtaRegge(){}

		void Init(TTree *tree);
		Bool_t Process(Long64_t entry);

	private:

		void Get_ComboWrappers(void);
		void Finalize(void);

		// BEAM POLARIZATION INFORMATION
		UInt_t dPreviousRunNumber;
		bool dIsPolarizedFlag; //else is AMO
		bool dIsPARAFlag; //else is PERP or AMO

		bool dIsMC = false;
		TLorentzVector thrown_beam_p4;
        TLorentzVector thrown_proton_p4;
        TLorentzVector thrown_piplus_p4;
        TLorentzVector thrown_piminus_p4;
        TLorentzVector thrown_gamma1_p4;
        TLorentzVector thrown_gamma2_p4;

		// ANALYZE CUT ACTIONS
		// // Automatically makes mass histograms where one cut is missing
		DHistogramAction_AnalyzeCutActions* dAnalyzeCutActions;

		//CREATE REACTION-SPECIFIC PARTICLE ARRAYS

		//Step 0
		DParticleComboStep* dStep0Wrapper;
		DBeamParticle* dComboBeamWrapper;
		DChargedTrackHypothesis* dProtonWrapper;

		//Step 1
		DParticleComboStep* dStep1Wrapper;
		DChargedTrackHypothesis* dPiPlusWrapper;
		DChargedTrackHypothesis* dPiMinusWrapper;

		//Step 2
		DParticleComboStep* dStep2Wrapper;
		DKinematicData* dDecayingPi0Wrapper;
		DNeutralParticleHypothesis* dPhoton1Wrapper;
		DNeutralParticleHypothesis* dPhoton2Wrapper;

		Double_t missingP,missingE,mimass,imass,imassGG;
		TLorentzVector mimassVec,imassVec,imassGGVec;
       
        Double_t dBRT;
    
        Double_t kfit_prob;
        TLorentzVector mimassVec_kfit,imassVec_kfit,imassGGVec_kfit;
        TLorentzVector mimassPiPiVec,mimassPiPiVec_kfit,imassPiPiVec,imassPiPiVec_kfit;
        Double_t mimass_kfit,imass_kfit,imassGG_kfit;
        Double_t em_balance,mimass_all;
        Double_t mimassPiPi,mimassPiPi_kfit,imassPiPi,imassPiPi_kfit;
        
        Double_t opangPiPi;
        TLorentzVector boostedPip,boostedPim;
        TVector3 boostVectorEta;
         
        Double_t opangProd;
        TLorentzVector boostedEta;
        TVector3 boostVectorCM;

        //Stuff for the Dalitz Plot analysis:
        TLorentzVector pip_etaFrame,pim_etaFrame,pi0_etaFrame;
        TVector3 boostVectorEta_KFit;
        
        Double_t TPlus,TMinus,TZero,TAll;
        Double_t DP_X,DP_Y;
        Int_t Global_Bin[5] = {0,0,0,0,0};
        Int_t NBins[5] = {9,10,11,12,13}; 
        Int_t NDP_bins = 5;
        
        Double_t TZero_mass,TAll_mass,DP_X_mass,DP_Y_mass;
        Int_t Global_Bin_mass[5] = {0,0,0,0,0};
        
        Double_t DP_max = 1.1;
        Double_t DP_min = -1.1;
        Double_t current_delta;
        Double_t DP_delta = DP_max - DP_min;
        
        Double_t m_pipm = 0.139;
        Double_t m_pi0 = 0.135;

		// DEFINE YOUR HISTOGRAMS HERE
		// EXAMPLES:
		TH1I* dHist_MissingMassSquared;
		TH1I* dHist_BeamEnergy;

	ClassDef(DSelector_Eta3Pi_Tree_genEtaRegge, 0);
};

void DSelector_Eta3Pi_Tree_genEtaRegge::Get_ComboWrappers(void)
{
	//Step 0
	dStep0Wrapper = dComboWrapper->Get_ParticleComboStep(0);
	dComboBeamWrapper = static_cast<DBeamParticle*>(dStep0Wrapper->Get_InitialParticle());
	dProtonWrapper = static_cast<DChargedTrackHypothesis*>(dStep0Wrapper->Get_FinalParticle(1));

	//Step 1
	dStep1Wrapper = dComboWrapper->Get_ParticleComboStep(1);
	dPiPlusWrapper = static_cast<DChargedTrackHypothesis*>(dStep1Wrapper->Get_FinalParticle(1));
	dPiMinusWrapper = static_cast<DChargedTrackHypothesis*>(dStep1Wrapper->Get_FinalParticle(2));

	//Step 2
	dStep2Wrapper = dComboWrapper->Get_ParticleComboStep(2);
	dDecayingPi0Wrapper = dStep2Wrapper->Get_InitialParticle();
	dPhoton1Wrapper = static_cast<DNeutralParticleHypothesis*>(dStep2Wrapper->Get_FinalParticle(0));
	dPhoton2Wrapper = static_cast<DNeutralParticleHypothesis*>(dStep2Wrapper->Get_FinalParticle(1));
}

#endif // DSelector_Eta3Pi_Tree_genEtaRegge_h
