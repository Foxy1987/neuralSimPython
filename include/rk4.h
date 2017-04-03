#ifndef _RK4_H_INCLUDED
#define _RK4_H_INCLUDED

#include <nvector/nvector_serial.h>  /* serial N_Vector types, fcts., macros */
#include <sundials/sundials_types.h> /* definition of type realtype */
#include <math.h>
#include <cfloat>
#include "neuronmodel.h"

/* #define min(X,Y) (X<Y ? X:Y) */
/* #define max(X,Y) (X>Y ? X:Y) */

#define Ith(v,i)    NV_Ith_S(v,i-1)       /* Ith numbers components 1..NEQ */

class RK4 {
 private:
  double a[4][4];
  double b[4];
 
  double *Y[4];
  double *F[4];
  double aF;

  int N;
  int i, j, k;
  
public:
  RK4(int);
  ~RK4();

  void integrate(double, N_Vector, N_Vector, NeuronModel &, double);
  int dim() { return N; }
};

#endif
