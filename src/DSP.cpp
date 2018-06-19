#include "DSP.h"


void DSP::fillMemory(void){
	for(int i = 0; i < MemorySize; i++){
		memory[i] = i;
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

void DSP::sendingData(void){
	cout<<"A mandar data carajo";
	if(sendingDataFlag == 1)
	{
		if(opCode == fftOpCode){
		}
		else if(opCode == ifftOpCode){
			if(NForFourier > currentDataIndex && currentDataIndex < MemorySize){
			InRealBus = memory[currentDataIndex];
			inImaginaryBus = 0;
			currentDataIndex +=currentDataIndex;
			cout<<"Voy a mandar "<<InRealBus<<"\n";
		}
		}
		else if(opCode == ALUOpCode){

		}
	}
}
	


void DSP::auxDecodeNType(void){
	sc_uint<64> inst = instruction.read();
	NForFourier = inst & NMask;
	hasFinished = 0;
	currentDataIndex = 0;
	sendingDataFlag = 1;
	if(opCode == fftOpCode){
		cout<< "Soy fft y el N es "<<NForFourier<<"\n";
	}
	else if(opCode == ifftOpCode){
		cout<< "Soy ifft y el N es "<<NForFourier<<"\n";
	}
}