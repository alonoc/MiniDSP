#include <iostream>
#include "systemc.h"

SC_MODULE(fir)
{
    sc_in<bool>  			clk;
    sc_in<bool>  			reset;
    
	sc_in< sc_int<16> > 	input;
	sc_in<bool> 			input_valid;
	sc_out<bool> 			input_ready;
	
    sc_out< sc_int<16> >  	output;
	sc_out<bool> 			output_valid;
	sc_in<bool> 			output_ready
	
	// coeficientes para el FIR

const sc_uint<8> coefs[5]; 

    
    void fir_main();
	
	void initialize_coefs()
    {
        coefs[0] = 0.125;
        coefs[1] = 0.25;
        coefs[2] = -0.75;
        coefs[3] = 1.25;
        coefs[4] = 1.0;
                
    }
    
    SC_CTOR(fir)
    {
        SC_THREAD(fir_main);
		reset_signal_is (reset,true);
		sensitive << clk.pos();
        initialize_coefs();
       
    }
};
