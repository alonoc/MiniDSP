#include <iostream>
#include <iomanip>
#include "systemc.h"
<<<<<<< HEAD
#include "DSP.h"

int sc_main(int argc, char* argv[])
{
	sc_signal<bool>   clock;
	sc_signal<sc_uint<64>> inst;
	sc_signal<bool> hasFinished;
	sc_signal<double> InRealBus;
	sc_signal<double> inImaginaryBus;

	DSP dsp("DSP");
	dsp.clock(clock);
	dsp.instruction(inst);
	dsp.InRealBus(InRealBus);
	dsp.inImaginaryBus(inImaginaryBus);
	dsp.fillMemory();

	hasFinished = 0;
	clock = 0;
	inst = 0x100000000000000F;
	hasFinished = 1;
	for (int i=0;i<10;i++) {
	    clock = 0; 
	    sc_start(1,SC_NS);
	    clock = 1; 
	    sc_start(1,SC_NS);
    }
=======
#include "IDFT.h"
#include "FPU32.h"

int sc_main(int argc, char* argv[])
{

	// Create Module instance
	sc_signal<bool> clk;
	sc_signal<bool> flag_en;
	sc_signal<bool> flag_reset;
	sc_signal<int> N;
	sc_signal< double > InReal;
	sc_signal< double > InImaginary;
	sc_signal< double > OutReal;
	sc_signal< double > OutImaginary;

	IDFT pIDFT("idft");
	pIDFT.clock(clk);
	pIDFT.f_Enable(flag_en);
	pIDFT.reset(flag_reset);
	pIDFT.N(N);
	pIDFT.InReal(InReal);
	pIDFT.InImaginary(InImaginary);
	pIDFT.OutReal(OutReal);
	pIDFT.OutImaginary(OutImaginary);
	
	// Define the inputs (get from the samples 1,3,8,7,5,6,1,4)
	double samp_real[] = {35,-8.2426,-3,0.2426,-5,0.2426,-3,-8.2426};
	double samp_im[] = {0,-7,2,7,0,-7,-2,7};
	N = 8;

	// Initialice before tracing
	clk = 0;
	flag_reset = 0;

	// Starting trace
	sc_trace_file *wf = sc_create_vcd_trace_file("IDFTing");
	wf->set_time_unit(1, SC_NS);

	sc_trace(wf, clk, "Clock");
	sc_trace(wf, flag_en, "Flag");
	sc_start(1, SC_NS);
	
	// Enabling the Module
	clk = 0;
	sc_start(1, SC_NS);
	flag_en = 1;
	clk = 1;
	sc_start(1, SC_NS);
	flag_en = 0;

	// Passing the input
	for(int i = 0; i < N; i++){
		InReal = samp_real[i];
		InImaginary = samp_im[i];
		clk = 0;
		sc_start(1, SC_NS);
		clk = 1;
		sc_start(1, SC_NS);
	}
	
	// Reading the output
	for(int i = 0; i < N; i++){
		clk = 0;
		sc_start(1, SC_NS);
		clk = 1;
		sc_start(1, SC_NS);
		std::cout << "Result: "  << OutReal << " + " << OutImaginary << "j" << std::endl;
	}

	// Finishing trace
	sc_close_vcd_trace_file(wf);
>>>>>>> f553b740920e44ce97ce0f1960b46b898749cd33

	std::cout << "Hellow World, SystemC" << std::endl;
	return 0;	
}