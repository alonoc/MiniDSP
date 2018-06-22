#include "IDFT.h"
#include <math.h>

//PLEASE DELETE THIS INCLUDE
#include <iostream>

void IDFT::doIt( void )
{
	if(reset == 1){
		std::cout << "Reseting...." << std::endl << std::endl;
		_N = 0;
		gnral_counter = 0;
	}
	else if(readingSamplesFlag.read() == 1)
	{
		std::cout << "ReadingSamples...." << std::endl;
		Samples[gnral_counter] = InReal;
		Samples[gnral_counter+_N] = InImaginary;
		gnral_counter++;
		std::cout << "Sample read: " << gnral_counter << std::endl;

		if(gnral_counter == _N)
		{
			gnral_counter = 0;
			std::cout << std::endl;
		}
	}
	else if(calculatingFlag.read() == 1)
	{
		std::cout << "Calcuting IRDFT...." << std::endl;

		sc_uint<16> n, k;
		double result1, result2;
	
		result1 = 0;
		result2 = 0;
		for(n = 0; n < _N; n++)
		{	
			double mult1 = Samples[n]*cos(2.0*M_PI*gnral_counter*n/_N);
			double mult2 = Samples[n+_N]*sin(2.0*M_PI*gnral_counter*n/_N);
			result1 += mult1;
			result1 -= mult2;

			mult1 = Samples[n]*sin(2.0*M_PI*gnral_counter*n/_N);
			mult2 = Samples[n+_N]*cos(2.0*M_PI*gnral_counter*n/_N);
			result1 += mult1;
			result1 += mult2;
		}
		OutReal = result1/_N;
		OutImaginary = result2/_N;

		gnral_counter++;
		std::cout << "Result Calculated: " << gnral_counter << std::endl;
		if(gnral_counter == _N)
		{
			gnral_counter = 0;
		}
		
	}
	else if(f_Enable.read() == 1){
		std::cout << "Enabling...." << std::endl << std::endl;
		Samples = (double*) calloc(N*2, sizeof(double));
		_N = N;
		gnral_counter = 0;
	}	
}

