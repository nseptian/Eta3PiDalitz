#include <cassert>
#include <iostream>
#include <string>
#include <complex>
#include <cstdlib>

#include "TLorentzVector.h"
#include "TVector3.h"
#include "IUAmpTools/Kinematics.h"
#include "DalitzAmp/DecayAmpReduced.h"

DecayAmpReduced::DecayAmpReduced( const vector< string >& args ) :
UserAmplitude< DecayAmpReduced >(args)
{

  // std::cout<<"Test: "<<args.size()<< endl;

  assert( args.size() == 7 );

  Norm = AmpParameter(args[0]);
  par_A = AmpParameter(args[1]);
  par_B = AmpParameter(args[2]);
  par_C = AmpParameter(args[3]);
  par_D = AmpParameter(args[4]);
  par_E = AmpParameter(args[5]);
  par_F = AmpParameter(args[6]);
  
  
  // need to register any free parameters so the framework knows about them
  registerParameter( Norm );
  registerParameter( par_A );
  registerParameter( par_B );
  registerParameter( par_C );
  registerParameter( par_D );
  registerParameter( par_E );
  registerParameter( par_F );
  
}


void
DecayAmpReduced::calcUserVars( GDouble** pKin, GDouble* userVars ) const {
  
  // This method can be used to calculate more CPU-intensive quantities
  // that the amplitudes themselves depend on.  IMPORTANT:  it is called
  // ONCE per fit so do NOT calculate quantities that depend on
  // free parameters in the fit.
  //
  // These quantities are always calculated on the CPU and, in the
  // case of GPU accelerated fits, they will be propagated to the GPU
  // for use in the amplitude kernel.  It is recommended to index
  // them with an enum.  Unfortunately there is no straightforward
  // way to propagate this indexing to the GPU, so the GPU kernel
  // will need to fetch the quantities using integers.
  
  TLorentzVector P1(pKin[0][1], pKin[0][2],
                    pKin[0][3], pKin[0][0]);
    
  TLorentzVector P2(pKin[1][1], pKin[1][2],
                      pKin[1][3], pKin[1][0]);
  
  TLorentzVector P3(pKin[2][1], pKin[2][2],
                        pKin[2][3], pKin[2][0]);
    
    
  TLorentzVector eta = P1 + P2 + P3;
  TVector3 eta_boost = eta.BoostVector();
    
  TLorentzVector boosted_p1 = P1;
  TLorentzVector boosted_p2 = P2;
  TLorentzVector boosted_p3 = P3;

  boosted_p1.Boost(eta_boost*(-1));
  boosted_p2.Boost(eta_boost*(-1));
  boosted_p3.Boost(eta_boost*(-1));
    
  double T_plus = boosted_p1.E() - boosted_p1.M();
  double T_minus = boosted_p2.E() - boosted_p2.M();
  double T_zero = boosted_p3.E() - boosted_p3.M();
  
  double T_all = T_plus + T_minus + T_zero;
 
  userVars[x_ind] = TMath::Sqrt(3)*(T_plus - T_minus) / T_all;
  userVars[y_ind] = 3.0*T_zero/T_all - 1.0;
  userVars[mass_ind] = eta.M2();

}


complex< GDouble >
DecayAmpReduced::calcAmplitude( GDouble** pKin, GDouble* userVars ) const {

  /****
   *  There are two possibilities here:
   *  (1) fetch mass squared from the userVars array, in which case
   *  the needUserVarsOnly flag can be set to true; or
   *  (2) calculate mass squared from scratch and set the needUserVarsOnly
   *  flag to false.
   *
   *  The role of user data is to optimize this function
   *  call in the instance it is repeated multiple times throughout
   *  a fit, which only happens if there are free parameters
   *  in this amplitude calculation (as there are in this one).
  */

  // (1) fetch the cached copy of mass squared
  
  //GDouble mass2 = userVars[kMass2];

  // or (2) calculate mass squared from scratch
    
  GDouble X = userVars[x_ind];
  GDouble Y = userVars[y_ind];
  GDouble eta_mass_sq = userVars[mass_ind];
  GDouble eta_mass_true = 0.5478;  
 
  /*
  TLorentzVector P1(pKin[0][1], pKin[0][2],
                    pKin[0][3], pKin[0][0]);

  TLorentzVector P2(pKin[1][1], pKin[1][2],
		    pKin[1][3], pKin[1][0]);

  TLorentzVector P3(pKin[2][1], pKin[2][2],
		    pKin[2][3], pKin[2][0]);


  TLorentzVector eta = P1 + P2 + P3;
  TVector3 eta_boost = eta.BoostVector();

  TLorentzVector boosted_p1 = P1;
  TLorentzVector boosted_p2 = P2;
  TLorentzVector boosted_p3 = P3;

  boosted_p1.Boost(eta_boost*(-1));
  boosted_p2.Boost(eta_boost*(-1));
  boosted_p3.Boost(eta_boost*(-1));

  double T_plus = boosted_p1.E() - boosted_p1.M();
  double T_minus = boosted_p2.E() - boosted_p2.M();
  double T_zero = boosted_p3.E() - boosted_p3.M();

  double T_all = T_plus + T_minus + T_zero;

  GDouble X = TMath::Sqrt(3)*(T_plus - T_minus) / T_all;
  GDouble Y = 3.0*T_zero/T_all - 1.0;
  */

  double amp_sq = (1.0 + par_A*Y + par_B*Y*Y + par_C*X + par_D*X*X + par_E*X*Y + par_F*Y*Y*Y );
  double eta_width = Norm*TMath::Sqrt(amp_sq);
  
  /*
  return  complex<GDouble>(1.0,0.0) /
    complex<GDouble>(eta_mass_sq - eta_mass_true*eta_mass_true, eta_mass_true*eta_width);
  */
  return  complex<GDouble>(eta_width,0.0);
}

DecayAmpReduced::~DecayAmpReduced(){
}


#ifdef GPU_ACCELERATION
void
DecayAmp:::launchGPUKernel( dim3 dimGrid, dim3 dimBlock, GPU_AMP_PROTO ) const {
  
  DecayAmpReduced_exec( dimGrid,  dimBlock, GPU_AMP_ARGS, Norm, par_A, par_B, par_C, par_D, par_E, par_F);

}
#endif //GPU_ACCELERATION
