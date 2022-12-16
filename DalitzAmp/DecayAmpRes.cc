#include <cassert>
#include <iostream>
#include <string>
#include <complex>
#include <cstdlib>

#include "TLorentzVector.h"
#include "TVector3.h"
#include "TMath.h"
#include "IUAmpTools/Kinematics.h"
#include "DalitzAmp/DecayAmpRes.h"

DecayAmpRes::DecayAmpRes( const vector< string >& args ) :
UserAmplitude< DecayAmpRes >(args)
{

  // std::cout<<"Test: "<<args.size()<< endl;

  assert( args.size() == 10 );

  Norm = AmpParameter(args[0]);
  par_A = AmpParameter(args[1]);
  par_B = AmpParameter(args[2]);
  par_C = AmpParameter(args[3]);
  par_D = AmpParameter(args[4]);
  par_E = AmpParameter(args[5]);
  par_F = AmpParameter(args[6]);
  par_G = AmpParameter(args[7]);
  par_H = AmpParameter(args[8]);
  par_L = AmpParameter(args[9]);
  
  
  // need to register any free parameters so the framework knows about them
  registerParameter( Norm );
  registerParameter( par_A );
  registerParameter( par_B );
  registerParameter( par_C );
  registerParameter( par_D );
  registerParameter( par_E );
  registerParameter( par_F );
  registerParameter( par_G );
  registerParameter( par_H );
  registerParameter( par_L );
  
}


void
DecayAmpRes::calcUserVars( GDouble** pKin, GDouble* userVars ) const {
  
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
DecayAmpRes::calcAmplitude( GDouble** pKin, GDouble* userVars ) const {

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

  double amp_sq = GetConvolutedAmpSq(X,Y);
  // cout << "ConvolutedAmpSq = " << GetConvolutedAmpSq(X,Y) << ", AmpSq = " << GetAmpSq(X,Y) << endl; 
  double eta_width = Norm*TMath::Sqrt(amp_sq);
  
  /*
  return  complex<GDouble>(1.0,0.0) /
    complex<GDouble>(eta_mass_sq - eta_mass_true*eta_mass_true, eta_mass_true*eta_width);
  */
  return  complex<GDouble>(eta_width,0.0);
}

DecayAmpRes::~DecayAmpRes(){
}

double DecayAmpRes::GetResFuncX(GDouble X, GDouble Y) const{
  double par[8];
  par[0]=  4.56178e-02; 
  par[1]= -5.31662e-05;  
  par[2]=  6.30150e-03;  
  par[3]=  4.12495e-04;  
  par[4]= -2.95304e-02; 
  par[5]=  5.18685e-04;  
  par[6]= -2.21068e-02; 
  par[7]=  1.29343e-04; 
  return par[0] + par[1]*X + par[2]*Y + par[3]*X*Y + par[4]*TMath::Power(X,2) + par[5]*TMath::Power(Y,2) + par[6]*Y*TMath::Power(X,2) + par[7]*X*TMath::Power(Y,2);
}

double DecayAmpRes::GetResFuncY(GDouble X, GDouble Y) const{
  double par[8];
  par[0]=  4.57198e-02; 
  par[1]= -2.02205e-04; 
  par[2]= -1.59946e-03; 
  par[3]=  1.96253e-05; 
  par[4]=  1.75347e-03; 
  par[5]= -3.13544e-02; 
  par[6]= -1.53797e-02; 
  par[7]=  1.07954e-03; 
  return par[0] + par[1]*X + par[2]*Y + par[3]*X*Y + par[4]*TMath::Power(X,2) + par[5]*TMath::Power(Y,2) + par[6]*Y*TMath::Power(X,2) + par[7]*X*TMath::Power(Y,2);
}

double DecayAmpRes::GetAmpSq(GDouble X, GDouble Y) const{
  return (1.0 + par_A*Y + par_B*Y*Y + par_C*X + par_D*X*X + par_E*X*Y + par_F*Y*Y*Y + par_G*X*X*Y + par_H*X*Y*Y + par_L*X*X*X);
}

double DecayAmpRes::GetIntegrandFunc(GDouble X, GDouble Y, GDouble XX, GDouble YY) const{
  // if ((XX < 0.05) && (XX > -0.05) && (YY < 0.05) && (YY > -0.05)){
    // cout << X << "|" << Y << "|" << XX << "|" << YY << "|" << "GaussFcn = " << Get2DNormalizedGaussianFcn(X,Y,XX,YY) << endl;
  // }
  return GetAmpSq(XX,YY)*Get2DNormalizedGaussianFcn(X,Y,X-XX,Y-YY);
}

double DecayAmpRes::Get2DNormalizedGaussianFcn(GDouble X, GDouble Y, GDouble XXX, GDouble YYY) const{
  GDouble sigmaX = GetResFuncX(X,Y);
  GDouble sigmaY = GetResFuncY(X,Y);
  // cout << "SigmaX = " << sigmaX << endl;
  // cout << "SigmaY = " << sigmaY << endl;
  GDouble zX = (XXX/sigmaX);
  GDouble zY = (YYY/sigmaY);
  return TMath::Exp(-0.5*(zX*zX+zY*zY))/(sigmaX*sigmaY*2*TMath::Pi());
}

double DecayAmpRes::GetConvolutedAmpSq(GDouble X, GDouble Y) const{
  //Do convolution between squared decay amplitude and resolution function of X and Y by numerical integration
  GDouble sigmaX = GetResFuncX(X,Y);
  GDouble sigmaY = GetResFuncY(X,Y);
  // const Int_t NX = 20;
  // const Int_t NY = 20;
  GDouble h = 0.05;
  GDouble Xlower_bound = -5*sigmaX-1;
  GDouble Xupper_bound = 5*sigmaX+1;
  GDouble Ylower_bound = -5*sigmaY-1;
  GDouble Yupper_bound = 5*sigmaY+1;
  Int_t NX = TMath::FloorNint((Xupper_bound-Xlower_bound)/h);
  Int_t NY = TMath::FloorNint((Yupper_bound-Ylower_bound)/h);
  // cout << "Nx = " << NX << ", Ny = " << NY << endl;
  GDouble convolutedAmpSq = 0.;
  
  for (Int_t i = 0; i < NX; i++)
  {
    for (Int_t j = 0; j < NY; j++)
    {
      convolutedAmpSq +=  (GetIntegrandFunc(X,Y,(Xlower_bound+i*h),(Ylower_bound+j*h)) + GetIntegrandFunc(X,Y,(Xlower_bound+(i+1)*h),(Ylower_bound+j*h)) + GetIntegrandFunc(X,Y,(Xlower_bound+i*h),(Ylower_bound+(j+1)*h)) + GetIntegrandFunc(X,Y,(Xlower_bound+(i+1)*h),(Ylower_bound+(j+1)*h)));
      // cout << X << "|" << Y << "|" << i << "|" << j << "|" << convolutedAmpSq << endl;
      // cout << X << "|" << Y << "|" << X-(Xlower_bound+i*h) << "|" << Y-(Ylower_bound+j*h) << endl;
    }
  }
  convolutedAmpSq *= (h*h/4);
  return convolutedAmpSq;
}

#ifdef GPU_ACCELERATION
void
DecayAmpRes:::launchGPUKernel( dim3 dimGrid, dim3 dimBlock, GPU_AMP_PROTO ) const {
  
  DecayAmpRes_exec( dimGrid,  dimBlock, GPU_AMP_ARGS, Norm, par_A, par_B, par_C, par_D, par_E, par_F, par_G, par_H, par_L);

}
#endif //GPU_ACCELERATION