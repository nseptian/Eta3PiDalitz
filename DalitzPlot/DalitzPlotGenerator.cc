
#include "DalitzPlot/DalitzPlotGenerator.h"
#include "IUAmpTools/Histogram1D.h"
#include "IUAmpTools/Kinematics.h"
#include "TLorentzVector.h"
#include "TVector3.h"

DalitzPlotGenerator::DalitzPlotGenerator( const FitResults& results ) :
PlotGenerator( results )
{
  bookHistogram( k_x, new Histogram1D(100,-1.0,1.0, "X", "" ) );
  bookHistogram( k_y, new Histogram1D(100,-1.0,1.0, "Y", "" ) );
  bookHistogram( kdltz, new Histogram2D(100,-1.0,1.0,100,-1.0,1.0, "dltz", "Dalitz Plot" ) );
}

void
DalitzPlotGenerator::projectEvent( Kinematics* kin ){
          
  TLorentzVector P1 = kin->particle(0);
  TLorentzVector P2 = kin->particle(1);
  TLorentzVector P3 = kin->particle(2);
  
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
   
  double X = TMath::Sqrt(3)*(T_plus - T_minus) / T_all;
  double Y = 3.0*T_zero/T_all - 1.0;
    
  fillHistogram(k_x, X );
  fillHistogram(k_y, Y );
  fillHistogram( kdltz, X,Y );
    
  /*
  fillHistogram( khm12, (P1+P2).M() );
  fillHistogram( khm13, (P1+P3).M() );
  fillHistogram( khm23, (P2+P3).M() );
  fillHistogram( kdltz, (P1+P2).M2(), (P2+P3).M2() );
   */
}
