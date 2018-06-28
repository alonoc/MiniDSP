#include <systemc.h>


SC_MODULE(tb) 
{
	sc_in<bool>  			clk;
	sc_out<bool> 			reset;
	sc_out< sc_int<16> > 	input;
	sc_out<bool> 			input_valid;
	sc_in<bool> 			input_ready;
	
	sc_in< sc_int<16> > 	output;
	sc_in<bool> 			output_valid;
	sc_out<bool> 			output_ready;
	
	void source();
	void sink();

	SC_CTOR(tb){
	SC_CTHREAD(source, clk.pos());
	SC_CTHREAD(sink, clk.pos()); 
	 
}