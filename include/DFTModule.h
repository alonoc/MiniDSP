#ifndef DFT_MODULE_H_
#define DFT_MODULE_H_

#include "systemc.h"

SC_MODULE(DFTModule)
{
    sc_in_clk clock;
    sc_in<bool> f_Enable;
    sc_in<bool> f_RxSamples;
    sc_in<bool> f_Calculate;
    sc_in<bool> Reset;
    sc_in<int> N;
    sc_in<double> Sample;
    sc_out<double> OutReal;
    sc_out<double> OutImg;

    double* _Samples;
    int _N;
    int _Counter;

    void DFT(void);

    SC_CTOR(DFTModule)
    {
        SC_METHOD(DFT);
        sensitive << clock.pos();
        sensitive << Reset;
    }

    ~DFTModule()
    {
        if(_Samples)
            delete[] _Samples;
    }
};

#endif
