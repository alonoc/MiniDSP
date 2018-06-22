#include "DSP.h"


void DSP::fillMemory(void){
	double samp_real[] = {35,-8.2426,-3,0.2426,-5,0.2426,-3,-8.2426};
	for(int i = 0; i < 8; i++){
		memory[i] = samp_real[i];
	}
}



void DSP::decode(void){
	if(hasFinished == 1){
		sc_uint<64> inst = instruction.read();
	
		//Get the Instruction
		opCode = inst >> opCodeMask;

		if(opCode == fftOpCode){
			auxDecodeNType();
		}
		else if(opCode == ifftOpCode){
			auxDecodeNType();
		}
		else if(opCode == ALUOpCode){
			cout << "Soy una ALU\n";
		}
	}
}

void DSP::auxDecodeNType(void){
	sc_uint<64> inst = instruction.read();
	NForFourier = inst & NMask;
	hasFinished = 0;
	currentDataIndex = 0;
	initSendingData = 1;
	if(opCode == fftOpCode){
		cout<< "Soy fft y el N es "<<(inst & NMask)<<"\n";
	}
	else if(opCode == ifftOpCode){
		cout<< "Soy ifft y el N es "<<(inst & NMask)<<"\n";
	}
}

void DSP::sendingData(void){
	if(initSendingData == 1){
		initSendingData = 2;
	}
	else if(initSendingData == 2){
		initSendingData = 3;
	}
	else if(initSendingData == 3){
		initSendingData = 0;
		rxSamplesFlag = 1;
	}
	else if(rxSamplesFlag == 1 )
	{
		if(opCode == fftOpCode){
			auxSendingData();
		}
		else if(opCode == ifftOpCode){
			auxSendingData();
		}
		else if(opCode == ALUOpCode){

		}

		
	}
	
}

void DSP::auxSendingData(void){
	if(NForFourier > currentDataIndex && currentDataIndex < MemorySize)
	{
		cout<<"Mandando datos \n";
		InRealBus = memory[currentDataIndex];
		inImaginaryBus = 0;
		currentDataIndex =currentDataIndex+1;
	}
	else
	{
		rxSamplesFlag = 0;
		initRecivingData = 1;
		currentDataIndex = 0;
	}
}

void DSP::enableStateMachine(void){
	if(opCode == lastOpCode){
		flag_en_IFFT = 0;
	}
	else{
		if(opCode == fftOpCode){
			
		}
		else if(opCode == ifftOpCode){
			cout<<"Sending Enable\n";
			flag_en_IFFT = 1;
			lastOpCode = opCode;
		}
		else if(opCode == ALUOpCode){

		}
	}
}

void DSP::recivingData(void){
	if(calculatingFlag == 1 || initRecivingData == 1){
		if(opCode == fftOpCode){
			auxRecivingData();
		}
		else if(opCode == ifftOpCode){
			auxRecivingData();
		}
		else if(opCode == ALUOpCode){

		}

		if(initRecivingData == 1){
			initRecivingData = 0;
			calculatingFlag = 1;
		}
	}
}

void DSP::auxRecivingData(void){
	if(NForFourier > currentDataIndex && currentDataIndex < MemorySize)
	{
		cout<<"Reciviendo datos \n";
		memory[currentDataIndex] = outRealBus.read();
		currentDataIndex =currentDataIndex+1;
	}
	else
	{
		calculatingFlag = 0;
		currentDataIndex = 0;
		hasFinished = 1;
	}
}



