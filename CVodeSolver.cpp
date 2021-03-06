#include "CVodeSolver.h"
#include <iostream>
/* User has to pass in a function pointer defining f(t, y) to the
 * constructor of the CVodeSolver class */
CVodeSolver::CVodeSolver(int n, N_Vector &yi, N_Vector &abstol, NeuronModel *model, RhsFn rhsfn ) {
    
    
   // this->yi = N_VMake_Serial(n, NV_DATA_S(yi));
    //this->abstol = N_VMake_Serial(n, NV_DATA_S(abstol));
//    N_VPrint_Serial(yi);
//    N_VPrint_Serial(abstol);
    
    rhsFn = rhsfn;
    
    reltol = RTOL;
    cvode_mem = CVodeCreate(CV_BDF, CV_NEWTON);
    
    CVodeSetUserData(cvode_mem, static_cast<void *>(model));
    CVodeInit(cvode_mem, rhsFn, T0, yi);
    
    CVodeSVtolerances(cvode_mem, reltol, abstol);

    CVDense(cvode_mem, n);
    
}

CVodeSolver::~CVodeSolver() {
        // cout << "Deallocated CVode memory" << endl; The main
        //program deallocates this memory since yi and abstol can be
        //used by other solvers.
    //N_VDestroy_Serial(yi);
    //N_VDestroy_Serial(abstol);
    CVodeFree(&cvode_mem);
}

/* make the call to perform the integration of the IVP */
/* 
   flag = CVode(cvode mem, tout, yout, &tret, itask);
   itask can be either CV_NORMAL or CV_ONE_STEP 
*/
int CVodeSolver::fadvance(realtype tout, realtype *t, N_Vector &yi, int mode) {
    return CVode(cvode_mem, tout, yi, t, mode);

}

int CVodeSolver::setStopTime(realtype t) {
    return CVodeSetStopTime(cvode_mem, t);
}

int CVodeSolver::rootInit(int n, gFn g) {
    return CVodeRootInit(cvode_mem, n, g);
}

int CVodeSolver::getRootInfo(int *rootsfound) {
    return CVodeGetRootInfo(cvode_mem, rootsfound);
}

int CVodeSolver::reinit(realtype t1, N_Vector &yi) {
        /*An optional function CVodeGetDky is available to obtain
          additional output values. This function should only be called after a
          successful return from CVode as it provides interpolated values either
          of y or of its derivatives (up to the current order of the integration
          method) interpolated to any value of t in the last internal step taken
          by cvode.
        */
    int flag;
    flag = CVodeGetDky(cvode_mem, t1, 0, yi);
    return CVodeReInit(cvode_mem, t1, yi); 
}
int CVodeSolver::get_dt(realtype *dt) {
    return CVodeGetCurrentStep(cvode_mem, dt);
}
