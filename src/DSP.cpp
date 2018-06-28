#include "DSP.h"


void DSP::fillMemory(void){
	double samp_real[] = {2,-8.2426,-3,0.2426,-5,0.2426,-3,-8.2426};
	
	for(int i = 0; i < 4000; i++){
		if(i < 5){
			memory[i] = samp_real[i];
		}
		else{
			memory[i] = i;
		}
	}
	memory[10] = 0x00000000414C0000;
	memory[11] = 0x0000000041BA0000;
	memory[12] = 0x00000000C1900000;
	memory[13] = 0x0000000041180000;
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
		else if(opCode == FPUOpCode){
			cout<<"Soy una FPU \n";
			OpCodeAlu = inst >> OpCodeAluMask;
			long opaDir = inst & opaDirMask;
			long opbDir = inst & opbDirMask;
			opbDir >>=  16;
			opaDir >>=  36;
			OpA = memory[opaDir];
			OpB = memory[opbDir];
		}
		else if(opCode == ALUOpCode){
			cout<<"Soy una ALU \n";
			OpCodeAlu = inst >> OpCodeAluMask;
			long opaDir = inst & opaDirMask;
			long opbDir = inst & opbDirMask;
			opbDir >>=  16;
			opaDir >>=  36;
			OpA = memory[opaDir];
			OpB = memory[opbDir];
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
		if(opCode == fftOpCode){
			rxSamplesFlag_DFT = 1;
		}
		else if(opCode == ifftOpCode){
			rxSamplesFlag = 1;
		}
	}
	else if(rxSamplesFlag == 1 || rxSamplesFlag_DFT == 1)
	{
		if(opCode == fftOpCode){
			auxSendingData();
		}
		else if(opCode == ifftOpCode){
			auxSendingData();
		}
		else if(opCode == FPUOpCode){

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
		rxSamplesFlag_DFT = 0;
		initRecivingData = 1;
		currentDataIndex = 0;
	}
}

void DSP::enableStateMachine(void){
	if(opCode == lastOpCode){
		flag_en_IDFT = 0;
		flag_en_DFT = 0;
	}
	else{
		if(opCode == fftOpCode){
			cout<<"Sending Enable to DFT\n";
			flag_en_DFT = 1;
			lastOpCode = opCode;
		}
		else if(opCode == ifftOpCode){
			cout<<"Sending Enable to IDFT\n";
			flag_en_IDFT = 1;
			lastOpCode = opCode;
		}
		else if(opCode == FPUOpCode){

		}
	}
}

void DSP::muxInputData(void){
	if(opCode == fftOpCode){
		outRealBus = outRealBus_DFT.read();
		outImaginaryBus = outImaginaryBus_DFT.read();
	}
	else if(opCode == ifftOpCode){
		outRealBus = outRealBus_IDFT.read();
		outImaginaryBus = outImaginaryBus_IDFT.read();
	}
	else{
		outRealBus = 0;
		outImaginaryBus = 0;
	}
}


void DSP::recivingData(void){
	if(calculatingFlag == 1 || initRecivingData == 1 || calculatingFlag_DFT == 1){
		if(opCode == fftOpCode){
			auxRecivingData();
		}
		else if(opCode == ifftOpCode){
			auxRecivingData();
		}
		else if(opCode == FPUOpCode){

		}

		if(initRecivingData == 1){
			initRecivingData = 0;
			if(opCode == ifftOpCode){
				calculatingFlag = 1;
			}
			else if(opCode == fftOpCode){
				calculatingFlag_DFT = 1;
			}
		}
	}
}

void DSP::auxRecivingData(void){
	if(NForFourier > currentDataIndex && currentDataIndex < MemorySize)
	{
		cout<<"Reciviendo datos \n";
		memory[currentDataIndex] = outRealBus;
		currentDataIndex =currentDataIndex+1;
	}
	else
	{
		calculatingFlag = 0;
		currentDataIndex = 0;
		calculatingFlag_DFT = 0;
		hasFinished = 1;
	}
}



