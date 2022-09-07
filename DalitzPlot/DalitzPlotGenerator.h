#if !(defined DALITZPLOTGENERATOR)
#define DALITZPLOTGENERATOR

#include "IUAmpTools/PlotGenerator.h"

class FitResults;
class Kinematics;

class DalitzPlotGenerator : public PlotGenerator
{
    
public:
    
  DalitzPlotGenerator( const FitResults& results );

  enum { 
    k_x = 0, k_y, kdltz,
    kNumHists 
  };
    
private:
        
  void projectEvent( Kinematics* kin );

};

#endif
