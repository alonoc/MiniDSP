#include <iostream>
#include "systemc.h"
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

	std::cout << "Hellow World, SystemC" << std::endl;
	return 0;	
}