#ifndef DSelector_Eta3Pi_Thrown_genEtaRegge_h
#define DSelector_Eta3Pi_Thrown_genEtaRegge_h

#include <iostream>
#include "DSelector/DSelector.h"

#include "TH1I.h"
#include "TH2I.h"
#include "TH1F.h"

class DSelector_Eta3Pi_Thrown_genEtaRegge : public DSelector
{
	public:

		DSelector_Eta3Pi_Thrown_genEtaRegge(TTree* locTree = NULL) : DSelector(locTree){}
		virtual ~DSelector_Eta3Pi_Thrown_genEtaRegge(){}

		void Init(TTree *tree);
		Bool_t Process(Long64_t entry);

	private:

		void Finalize(void);

		// BEAM POLARIZATION INFORMATION
		UInt_t dPreviousRunNumber;
		bool dIsPolarizedFlag; //else is AMO
		bool dIsPARAFlag; //else is PERP or AMO
		TH1F* dHist_PiPlusPiMinusPi0Mass;
		TH1F* dHist_MandelstamT;
		TH2F* dHist_Dalitz;

	ClassDef(DSelector_Eta3Pi_Thrown_genEtaRegge, 0);
};

#endif // DSelector_Eta3Pi_Thrown_genEtaRegge_h
