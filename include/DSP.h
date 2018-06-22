#ifndef DSP_H_
#define DSP_H_

#include "systemc.h"

constexpr int MemorySize = 4096; 
constexpr int MemoryLogSize = 12;
constexpr int opCodeMask = 60;
constexpr int NMask = 0x00000000000FFFFF;

constexpr int fftOpCode = 1; 
constexpr int ifftOpCode = 2; 
constexpr int ALUOpCode = 3; 

SC_MODULE(DSP)
{
	sc_in< sc_uint<64> > instruction;
	sc_in_clk clock;
	sc_out<double> InRealBus;
	sc_out<double> inImaginaryBus;
	sc_out<double> outRealBus;
	sc_out<double> outImaginaryBus;

	//Status;
	bool sendingDataFlag = 0;
	bool recivingDataFlag = 0;
	bool startExecution = 0;
	bool hasFinished = 1;
	//Current instruction
	sc_uint<4> opCode;

	//Used in FFT and IFFT for sending data
	sc_uint<MemoryLogSize> currentDataIndex;
	sc_uint<MemoryLogSize> NForFourier;

	//Used memmorySoFar
	sc_uint<MemoryLogSize> usedMemorySoFar;

	//Data Memory of the DSP
	int memory[MemorySize];
	
	void decode(void );
	void sendingData(void);
	void recivingData(void);

	//Aux Methods
	void auxDecodeNType(void);
	void auxSendingData(void);
	void auxRecivingData(void);
	void fillMemory(void);

	SC_CTOR(DSP)
	{
		SC_METHOD( decode );
			sensitive << clock.pos();

		SC_METHOD( sendingData );
			sensitive << clock.pos();

		SC_METHOD( recivingData);
			sensitive << clock.pos();
	}


};


#endif 