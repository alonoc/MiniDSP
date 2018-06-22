#include <iostream>
#include <iomanip>
#include "systemc.h"
#include "IDFT.h"
#include "FPU32.h"
#include "DSP.h"

int sc_main(int argc, char* argv[])
{

	//General Signals
	sc_signal<bool> clk;
	sc_signal<sc_uint<64>> inst;

	//FFT and IFFT
	sc_signal<bool> flag_en_IFFT;
	sc_signal<bool> flag_reset_IFFT;
	sc_signal<bool> flag_en_FFT;
	sc_signal<bool> flag_reset_FFT;
	sc_signal<int> FFT_N;
	sc_signal<bool> sendingDataIFFT;
	sc_signal<bool> calculatingFlagIFFT;
	sc_signal<double> InRealBus;
	sc_signal<double> inImaginaryBus;
	sc_signal<double> outRealBus;
	sc_signal<double> outImaginaryBus;

	DSP dsp("DSP");
	dsp.flag_en_IFFT(flag_en_IFFT);
	dsp.flag_reset_IFFT(flag_reset_IFFT);
	dsp.NForFourier(FFT_N);
	dsp.clock(clk);
	dsp.instruction(inst);
	dsp.InRealBus(InRealBus);
	dsp.inImaginaryBus(inImaginaryBus);
	dsp.outImaginaryBus(outImaginaryBus);
	dsp.outRealBus(outRealBus);
	dsp.readingSamplesFlag(sendingDataIFFT);
	dsp.calculatingFlag(calculatingFlagIFFT);
	dsp.fillMemory();

	IDFT pIDFT("IDFFT");
	pIDFT.clock(clk);
	pIDFT.f_Enable(flag_en_IFFT);
	pIDFT.reset(flag_reset_IFFT);
	pIDFT.N(FFT_N);
	pIDFT.InReal(InRealBus);
	pIDFT.InImaginary(inImaginaryBus);
	pIDFT.OutReal(outImaginaryBus);
	pIDFT.OutImaginary(outRealBus);
	pIDFT.readingSamplesFlag(sendingDataIFFT);
	pIDFT.calculatingFlag(calculatingFlagIFFT);


	// Initialice before tracing
	clk = 0;

	// Starting trace
	sc_trace_file *wf = sc_create_vcd_trace_file("IDFTing");
	wf->set_time_unit(1, SC_NS);

	sc_trace(wf, clk, "Clock");
	sc_trace(wf, flag_en_IFFT, "Flag");
	sc_start(1, SC_NS);

	inst = 0x2000000000000005;
	
	for (int i=0;i<15;i++) {
	    clk = 0; 
	    sc_start(1,SC_NS);
	    clk = 1; 
	    sc_start(1,SC_NS);
    }

	std::cout << "Hellow World, SystemC" << std::endl;
	// // Passing the input
	// for(int i = 0; i < FFT_N; i++){
	// 	clk = 0;
	// 	sc_start(1, SC_NS);
	// 	clk = 1;
	// 	sc_start(1, SC_NS);
	// }
	
	// // Reading the output
	// for(int i = 0; i < FFT_N; i++){
	// 	clk = 0;
	// 	sc_start(1, SC_NS);
	// 	clk = 1;
	// 	sc_start(1, SC_NS);
	// 	std::cout << "Result: "  << InRealBus << " + " << outImaginaryBus << "j \n";
	// }

	// // Finishing trace
	// sc_close_vcd_trace_file(wf);

	// std::cout << "Hellow World, SystemC" << std::endl;
	return 0;	
}