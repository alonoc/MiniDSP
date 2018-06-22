#ifndef IDFT_H_
#define IDFT_H_

#include "systemc.h"

SC_MODULE(IDFT)
{
	sc_in_clk clock;
	sc_in<bool> f_Enable;
	sc_in<bool> reset;
	sc_in< int > N;
	sc_in< double > InReal;
	sc_in< double > InImaginary;
	sc_in<bool> rxSamplesFlag;
	sc_in<bool> calculatingFlag;
	sc_out< double > OutReal;
	sc_out< double > OutImaginary;


	double *Samples;
	int _N;
	int gnral_counter;

	void doIt( void );

	SC_CTOR(IDFT)
	{
		SC_METHOD( doIt );
			sensitive << clock.pos();
			sensitive << reset;
	}

	~IDFT()
	{
		if(Samples){
			delete[] Samples;
		}
	}
};


#endif