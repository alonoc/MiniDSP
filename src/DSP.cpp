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

void DSP::sendingData(void){
	if(sendingDataFlag == 1 && currentReadingSamples == 1)
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

void DSP::enableStateMachine(void){
	if(opCode == lastOpCode){
		flag_en_IFFT = 0;
		currentCalculatingModule = calculatingFlag.read();
		currentReadingSamples = readingSamplesFlag.read();
	}
	else{
		if(opCode == fftOpCode){
			
		}
		else if(opCode == ifftOpCode){
			flag_en_IFFT = 1;
			currentCalculatingModule = calculatingFlag.read();
			currentReadingSamples = readingSamplesFlag.read();
		}
		else if(opCode == ALUOpCode){

		}
	}
}

void DSP::recivingData(void){
	if(recivingDataFlag == 1 && currentCalculatingModule == 1){
		if(opCode == fftOpCode){
			auxRecivingData();
		}
		else if(opCode == ifftOpCode){
			auxRecivingData();
		}
		else if(opCode == ALUOpCode){

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
		sendingDataFlag = 0;
		recivingDataFlag = 0;
		currentDataIndex = 0;
		hasFinished = 1;
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
		sendingDataFlag = 0;
		recivingDataFlag = 1;
		currentDataIndex = 0;
	}
}

void DSP::auxDecodeNType(void){
	sc_uint<64> inst = instruction.read();
	NForFourier = inst & NMask;
	hasFinished = 0;
	currentDataIndex = 0;
	sendingDataFlag = 1;
	if(opCode == fftOpCode){
		cout<< "Soy fft y el N es "<<(inst & NMask)<<"\n";
	}
	else if(opCode == ifftOpCode){
		cout<< "Soy ifft y el N es "<<(inst & NMask)<<"\n";
	}
}