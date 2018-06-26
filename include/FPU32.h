#ifndef FPU32_H_
#define FPU32_H_

#include "systemc.h"

SC_MODULE(FloatingAdder32)
{
	sc_in< sc_uint<32> > OpA;
	sc_in< sc_uint<32> > OpB;
	sc_out< sc_uint<32> > Result;

	void doIt( void );

	SC_CTOR(FloatingAdder32)
	{
		SC_METHOD( doIt );
			sensitive << OpA;
			sensitive << OpB;
	}
};

SC_MODULE(FloatingMultiplier32)
{
	sc_in< sc_uint<32> > OpA;
	sc_in< sc_uint<32> > OpB;
	sc_out< sc_uint<32> > Result;
	sc_out< sc_uint<1> > Overflow;
	sc_out< sc_uint<1> > Underflow;

	void Multiply( void );

	SC_CTOR(FloatingMultiplier32)
	{
		SC_METHOD( Multiply );
			sensitive << OpA;
			sensitive << OpB;
	}
};

#endif