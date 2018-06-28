#include <systemc.h>
#include "fir.h"
#include "tb.h"



SC_MODULE( SYSTEM ) {
	
	
	tb	*tb0;
	fir	*fir0;
	
	sc_signal< sc_int<16> >  input_sig;
	sc_signal< bool > 		 input_sig_valid
	sc_signal< bool > 		 input_sig_ready
	
	sc_signal< sc_int<16> >  output_sig;
	sc_signal< bool > 		 output_sig_valid
	sc_signal< bool > 		 output_sig_ready
	
	sc_signal< bool >  reset_sig;
	sc_clock  clk_sig;

	SC_CTOR( SYSTEM )
		: clk_sig ("clk_sig", 10, SC_NS)
		{
			tb0 = new tb("tb0");
			tb0->clk( clk_sig );
			tb0->reset( reset_sig );
			tb0->input( input_sig );
			tb0->out( output_sig );
			tb0->input_valid( input_sig_valid );
			tb0->input_ready( input_sig_ready );
			tb0->output_valid( output_sig_valid );
			tb0->output_ready( output_sig_ready );
			
			fir0 = new fir("fir0");
			fir0->clk( clk_sig );
			fir0->reset( reset_sig );
			fir0->input( input_sig );
			fir0->input( input_sig );
			fir0->input_valid( input_sig_valid );
			fir0->input_ready( input_sig_ready );
			fir0->output_valid( output_sig_valid );
			fir0->output_ready( output_sig_ready );
}

SYSTEM *top = NULL
	int sc_main( int argc, char* argv[] )
{
	top = new SYSTEM( "top" );
	sc_start();
	return 0;
}

//Destructor
~SYSTEM() {
	delete tb0;
	delete fir0;
}
};

