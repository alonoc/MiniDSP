#include <iostream>
#include <iomanip>
#include "systemc.h"
#include "IDFT.h"
#include "DFTModule.h"
#include "FPU32.h"
#include "DSP.h"

int sc_main(int argc, char* argv[])
{

	//General Signals
	sc_signal<bool> clk;
	sc_signal<sc_uint<64>> inst;

	//FFT and IFFT
	sc_signal<bool> flag_en_IDFT;
	sc_signal<bool> flag_reset_IDFT;
	sc_signal<bool> flag_en_DFT;
	sc_signal<bool> flag_reset_DFT;
	sc_signal<int> DFT_N;
	sc_signal<bool> rxIDFT;
	sc_signal<bool> rxDFT;
	sc_signal<bool> calculatingFlagIDFT;
	sc_signal<bool> calculatingFlagDFT;
	sc_signal<double> InRealBus;
	sc_signal<double> inImaginaryBus;
	sc_signal<double> outRealBus;
	sc_signal<double> outImaginaryBus;

	DSP dsp("DSP");
	dsp.flag_en_IDFT(flag_en_IDFT);
	dsp.flag_reset_IDFT(flag_reset_IDFT);
	dsp.NForFourier(DFT_N);
	dsp.clock(clk);
	dsp.instruction(inst);
	dsp.InRealBus(InRealBus);
	dsp.inImaginaryBus(inImaginaryBus);
	dsp.outImaginaryBus(outImaginaryBus);
	dsp.outRealBus(outRealBus);
	dsp.rxSamplesFlag(rxIDFT);
	dsp.calculatingFlag(calculatingFlagIDFT);
	dsp.fillMemory();

	IDFT pIDFT("IDFFT");
	pIDFT.clock(clk);
	pIDFT.f_Enable(flag_en_IDFT);
	pIDFT.reset(flag_reset_IDFT);
	pIDFT.N(DFT_N);
	pIDFT.InReal(InRealBus);
	pIDFT.InImaginary(inImaginaryBus);
	pIDFT.OutReal(outRealBus);
	pIDFT.OutImaginary(outImaginaryBus);
	pIDFT.rxSamplesFlag(rxIDFT);
	pIDFT.calculatingFlag(calculatingFlagIDFT);

	DFTModule pDFTMod("DFT");
	pDFTMod.clock(clk);
	pDFTMod.f_Enable(flag_en_DFT);
	pDFTMod.f_RxSamples(rxDFT);
	pDFTMod.f_Calculate(calculatingFlagDFT);
	pDFTMod.Reset(flag_reset_DFT);
	pDFTMod.N(DFT_N);
	pDFTMod.Sample(InRealBus);
	pDFTMod.OutReal(outRealBus);
	pDFTMod.OutImaginary(outImaginaryBus);


	// Initialice before tracing
	clk = 0;
	rxIDFT = 0;
	rxDFT = 0;
	calculatingFlagIDFT = 0;
	calculatingFlagDFT = 0;
	flag_reset_IDFT = 0;
	flag_reset_DFT = 0;

	// Starting trace
	sc_trace_file *wf = sc_create_vcd_trace_file("IDFTing");
	wf->set_time_unit(1, SC_NS);

	sc_trace(wf, clk, "Clock");
	sc_trace(wf, flag_en_IFFT, "Flag");
	sc_trace(wf, InRealBus, "RealData");
	sc_trace(wf, outRealBus, "OutRealData");
	sc_trace(wf, rxIFFT, "SendingData");
	sc_trace(wf, calculatingFlagIFFT, "CalculatingData");

	sc_start(1, SC_NS);
	inst = 0x2000000000000005;
	
	for (int i=0;i<20;i++) {
	    clk = 0; 
	    sc_start(1,SC_NS);
	    clk = 1; 
	    sc_start(1,SC_NS);
	}	

	std::cout << "Hellow World, SystemC" << std::endl;
	
	// // Finishing trace
	sc_close_vcd_trace_file(wf);

	// std::cout << "Hellow World, SystemC" << std::endl;
	return 0;	
}
