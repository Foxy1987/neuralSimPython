#include "rk4.h"
#include <iostream>


RK4::RK4(int dim) {

  N= dim;

  // initialize upper triangle of butcher table to 0.0 and then fill in appropriate entries
  for (int i= 0; i < 4; i++) {
      Y[i]= new double[N];
      F[i]= new double[N];
      for (int j=i; j < 4; j++) {
          a[i][j]= 0.0;
      }
  }


  a[0][0]=0.0;
  a[1][0]=0.5;
  a[2][0]=0.0;
  a[2][1]=0.5;
  a[3][0]=0.0;
  a[3][1]=0.0;
  a[3][2]=1.0;
   
  b[0]= 1.0/6.0;
  b[1]= 1.0/3.0;
  b[2]= 1.0/3.0;
  b[3]= 1.0/6.0;

}


RK4::~RK4(){
   
    for (int i= 0; i < 4; i++) {
        delete [] Y[i];
        delete [] F[i];
    }
}

void RK4::integrate(double t, N_Vector y, N_Vector yn_, NeuronModel &model, double dt) {
    realtype *yi, *yn;
    N_Vector Yi, Fi;
    yi = NV_DATA_S(y);
    yn = NV_DATA_S(yn_);
  

   
   // calculate iterative terms rk4_Y[__i] and rk4_F[__i] (to fourth order)
    // stage Yi
    for (i= 0; i < 4; i++) {
        // equation
        for (k= 0; k < N; k++){
          aF= 0.0;
          // coefficient
          for (j= 0; j < i; j++) {
              aF += a[i][j]*F[j][k];
  //            cout << a[i][j] << "\t";
          }
    //      cout << "\n";
          Y[i][k]= yi[k]+dt*aF;
        }
        Yi = N_VMake_Serial(N, Y[i]);
        Fi = N_VMake_Serial(N, F[i]);
      
        model.derivative(t, Yi, Fi, NULL); 
        N_VDestroy_Serial(Yi);
        N_VDestroy_Serial(Fi);
    }

    //cout << a[3][2] << " " << b[1] << " " << b[2] << "\n";
       // sum up rk4_Y[__i] and rk4_F[__i] to build 4th order scheme -> yn
   
   for (k= 0; k < N; k++) {
     aF= 0.0;
     for (j= 0; j < 4; j++) {
       aF+= b[j]*F[j][k];
    //   cout << F[j][k] << "\t";
     }
    // cout << "\n";
      yn[k]= yi[k]+dt*aF;
      
    }
}
