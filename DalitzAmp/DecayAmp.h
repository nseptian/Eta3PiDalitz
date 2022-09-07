#if !defined(DECAYAMP)
#define DECAYAMP

#include "IUAmpTools/Amplitude.h"
#include "IUAmpTools/UserAmplitude.h"
#include "IUAmpTools/AmpParameter.h"
#include "GPUManager/GPUCustomTypes.h"

#include <utility>
#include <string>
#include <complex>
#include <vector>

#ifdef GPU_ACCELERATION

void DecayAmp_exec( dim3 dimGrid, dim3 dimBlock, GPU_AMP_PROTO,
		           GDouble norm, GDouble A, GDouble B, GDouble C, GDouble D, GDouble E, GDouble F, GDouble G, GDouble H, GDouble L
                          );

#endif // GPU_ACCELERATION

using std::complex;
using namespace std;

class Kinematics;

class DecayAmp : public UserAmplitude< DecayAmp >{

public:
  
  DecayAmp() : UserAmplitude< DecayAmp >() { }

  DecayAmp( const vector< string >& args );

  ~DecayAmp();

  string name() const { return "DecayAmp"; }

  complex< GDouble > calcAmplitude( GDouble** pKin, GDouble* userVars ) const;
  
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
