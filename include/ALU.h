#include "systemc.h"

SC_MODULE(ALU)
{
	sc_in < sc_uint<3> > OP;
	sc_in < sc_uint<4> > OPa,OPb;
	sc_out < bool > ZERO;
	sc_out < sc_uint<4> >RESULT;

    sc_uint<4> data1, data2;
    sc_uint<5> result;

    void operate();
    
	SC_CTOR(ALU){
		SC_METHOD(operate);
			sensitive << OPa << OPb << OP;
	}

};
