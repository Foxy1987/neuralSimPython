
#ifndef ELECTRODE
#define ELECTRODE

#include  <cmath>
#include <iostream>




using namespace std;

class Electrode {
    private:
        int mode;
        int del, dur; //pulse type
        double amp;
        double iapp;
        double freq, L;
        double fmin, fmax;
    public:
        Electrode(int m_in) {
            mode = m_in;
            iapp = 0; amp = 0; del =0; dur = 0;
        }
       
        ~Electrode(){}
        void set_dur(int dur_in) {
            dur = dur_in;
        }
        void set_del(int del_in) {
            del = del_in;
        }
        void set_amp(double amp_in) {
            amp = amp_in;
        }

        double i(double t){
            // switch on mode for the calculation of applied current
            switch(mode) {
                case 0:
                    if (t < del + dur && t >= del) {
                        iapp = amp;
                        
                    } else {
                        iapp = 0;
                        
                    }
                    break;
                case 1:
                    if (t < del + dur && t >=del) {
                        iapp = amp*(1+sin(2*M_PI*t/1000 + 3*(M_PI/2)));
                    } else {
                        iapp = 0;
                    }
                    break;
                case 2:
                    fmin=0.1; fmax = 4.0;
                    if (t > 30000.0) {
                        
                        L = log(fmax/fmin)/100000.0;
                        t = t-30000.0;
                        freq = (fmin*((exp(L*(t))-1)/(L*(t))));
                    } 
                    else {
                        freq = 0.1;
                    }
                    iapp = amp*sin(2*M_PI*freq*t/1000);
                    break;
                default:
                    iapp = 0;
                    //cout << "not implemented yet\n";
                }
            return iapp;
        }
        
        void state2double(double *st) {
            st[0] = iapp;
        }
};
#endif
