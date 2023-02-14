
#include <stdio.h>

#include "GPUManager/GPUCustomTypes.h"
#include "GPUManager/CUDA-Complex.cuh"


__global__ void
DecayAmp_kernel( GPU_AMP_PROTO, GDouble par_A, GDouble par_B, GDouble par_C, GDouble par_D, GDouble par_E, GDouble par_F, GDouble par_G, GDouble par_H, GDouble par_L){

  int iEvent = GPU_THIS_EVENT;

  /****
  *  Equivalently one could switch needsUserVarsOnly to false
  *  and uncomment the following lines to get the same
  *  result.  The role of user-data is to optimize this function
  *  call in the instance it is repeated multiple times throughout
  *  a fit.

  GDouble p1[4] = GPU_P4(daught1-1);
  GDouble p2[4] = GPU_P4(daught2-1);

  GDouble m = G_SQRT(SQ(p1[0]+p2[0]) - SQ(p1[1]+p2[1])
                                     - SQ(p1[2]+p2[2])
                                     - SQ(p1[3]+p2[3]));

  WCUComplex bwBot = { SQ( m ) - SQ( mass0 ), mass0 * width0 };
  */


  // here we need to be careful to index the user-defined
  // data with the proper integer corresponding to the
  // enumeration in the C++ header file

  GDouble par_A = GPU_UVARS(0);
  GDouble par_B = GPU_UVARS(1);
  GDouble par_C = GPU_UVARS(2);
  GDouble par_D = GPU_UVARS(3);
  GDouble par_E = GPU_UVARS(4);
  GDouble par_F = GPU_UVARS(5);
  GDouble par_G = GPU_UVARS(6);
  GDouble par_H = GPU_UVARS(7);
  GDouble par_L = GPU_UVARS(8);
    
  pcDevAmp[iEvent] = (1.0 + par_A*Y + par_B*Y*Y + par_C*X + par_D*X*X + par_E*X*Y + par_F*Y*Y*Y + par_G*X*X*Y + par_H*X*Y*Y + par_L*X*X*X);
}


void
DecayAmp_exec( dim3 dimGrid, dim3 dimBlock, GPU_AMP_PROTO,GDouble par_A, GDouble par_B, GDouble par_C, GDouble par_D, GDouble par_E, GDouble par_F, GDouble par_G, GDouble par_H, GDouble par_L)
{  

  DecayAmp_kernel<<< dimGrid, dimBlock >>>
    ( GPU_AMP_ARGS,par_A, par_B, par_C, par_D, par_E, par_F, par_G, par_H, par_L);

}
