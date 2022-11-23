#if !defined(DECAYAMPRES)
#define DECAYAMPRES

#include "IUAmpTools/Amplitude.h"
#include "IUAmpTools/UserAmplitude.h"
#include "IUAmpTools/AmpParameter.h"
#include "GPUManager/GPUCustomTypes.h"

#include <utility>
#include <string>
#include <complex>
#include <vector>

#ifdef GPU_ACCELERATION

void DecayAmpRes_exec( dim3 dimGrid, dim3 dimBlock, GPU_AMP_PROTO,
		           GDouble norm, GDouble A, GDouble B, GDouble C, GDouble D, GDouble E, GDouble F, GDouble G, GDouble H, GDouble L
                          );

#endif // GPU_ACCELERATION

using std::complex;
using namespace std;

class Kinematics;

class DecayAmpRes : public UserAmplitude< DecayAmpRes >{

public:
  
  DecayAmpRes() : UserAmplitude< DecayAmpRes >() { }

  DecayAmpRes( const vector< string >& args );

  ~DecayAmpRes();

  string name() const { return "DecayAmpRes"; }

  complex< GDouble > calcAmplitude( GDouble** pKin, GDouble* userVars ) const;

  double GetResFuncX(GDouble X,GDouble Y) const;
  double GetResFuncY(GDouble X,GDouble Y) const;
  double GetAmpSq(GDouble X,GDouble Y) const;
  double GetIntegrandFunc(GDouble X, GDouble Y, GDouble XX, GDouble YY) const;
  double GetConvolutedAmpSq(GDouble X, GDouble Y) const;
  
  // **********************
  // The following lines are optional and can be used to precalcualte
  // user-defined data that the amplitudes depend on.
  
  // Use this for indexing a user-defined data array and notifying
  // the framework of the number of user-defined variables.
  enum UserVars {x_ind,y_ind,mass_ind,kNumUserVars };
  unsigned int numUserVars() const { return kNumUserVars; }
  
  // This function needs to be defined -- see comments and discussion
  // in the .cc file.
  void calcUserVars( GDouble** pKin, GDouble* userVars ) const;
  
  // This is an optional addition if the calcAmplitude routine
  // can run with only the user-defined data and not the original
  // four-vectors.  It is used to optimize memory usage in GPU
  // based fits.
  //bool needsUserVarsOnly() const { return true; }
  bool needsUserVarsOnly() const { return false; }
  // **  end of optional lines **
  
#ifdef GPU_ACCELERATION

  void launchGPUKernel( dim3 dimGrid, dim3 dimBlock, GPU_AMP_PROTO ) const;

  bool isGPUEnabled() const { return true; }

#endif // GPU_ACCELERATION
  
private:
	
  AmpParameter Norm;
  AmpParameter par_A;
  AmpParameter par_B;
  AmpParameter par_C;
  AmpParameter par_D;
  AmpParameter par_E;
  AmpParameter par_F;
  AmpParameter par_G;
  AmpParameter par_H;
  AmpParameter par_L;
};

#endif
