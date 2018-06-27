#ifndef FPU32_H_
#define FPU32_H_

#include "systemc.h"
#include <memory>

SC_MODULE(FloatingAdder32)
{
	sc_in< sc_uint<32> > OpA;
	sc_in< sc_uint<32> > OpB;
	sc_out< sc_uint<32> > Result;
	sc_out< sc_uint<1> > Overflow;
	sc_out< sc_uint<1> > Underflow;
	
	void Add( void );

	SC_CTOR(FloatingAdder32)
	{
		SC_METHOD( Add );
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

SC_MODULE(MuxTwoPorts32Bits)
{
	sc_in< sc_uint<32> > InOne;
	sc_in< sc_uint<32> > InTwo;
	sc_in< sc_uint<2> > Select;
	sc_out< sc_uint<32> > Out;

	void Multiplex( void );

	SC_CTOR(MuxTwoPorts32Bits)
	{
		SC_METHOD( Multiplex );
			sensitive << InOne;
			sensitive << InTwo;
			sensitive << Select;
	}
};

SC_MODULE(MuxTwoPorts1Bit)
{
	sc_in< sc_uint<1> > InOne;
	sc_in< sc_uint<1> > InTwo;
	sc_in< sc_uint<2> > Select;
	sc_out< sc_uint<1> > Out;
	
	void Multiplex( void );
	
	SC_CTOR(MuxTwoPorts1Bit)
	{
		SC_METHOD( Multiplex );
			sensitive << InOne;
			sensitive << InTwo;
			sensitive << Select;
	}
};

SC_MODULE(FPU32)
{
public:
	sc_in< sc_uint<32> > OpA;
	sc_in< sc_uint<32> > OpB;
	sc_in< sc_uint<2> > OpCode;
	sc_out< sc_uint<32> > Out;
	sc_out< sc_uint<1> > Overflow;
	sc_out< sc_uint<1> > Underflow;

	void Execute( void );

	SC_CTOR(FPU32)
	{
		SC_METHOD( Execute );
			sensitive << OpA;
			sensitive << OpB;
			sensitive << OpCode;

		// Module Instanciation
		m_Multiplier 	= std::make_unique<FloatingMultiplier32>("FMULT32");
		m_Adder 		= std::make_unique<FloatingAdder32>("FADD32");
		m_MuxOut		= std::make_unique<MuxTwoPorts32Bits>("MUXOUT");
		m_MuxOverflow	= std::make_unique<MuxTwoPorts1Bit>("MUXOVERFLOW");
		m_MuxUnderflow	= std::make_unique<MuxTwoPorts1Bit>("MUXUNDERFLOW");

		// Module mapping
		m_Adder->OpA(OpA);
		m_Adder->OpB(OpB);
		m_Adder->Result(m_sAdderOut);
		m_Adder->Overflow(m_sAdderOverflow);
		m_Adder->Underflow(m_sAdderUnderflow);

		m_Multiplier->OpA(OpA);
		m_Multiplier->OpB(OpB);
		m_Multiplier->Result(m_sMultOut);
		m_Multiplier->Overflow(m_sMultOverflow);
		m_Multiplier->Underflow(m_sMultUnderflow);

		m_MuxOut->Select(OpCode);
		m_MuxOut->Out(Out);
		m_MuxOut->InOne(m_sAdderOut);
		m_MuxOut->InTwo(m_sMultOut);

		m_MuxOverflow->Select(OpCode);
		m_MuxOverflow->Out(Overflow);
		m_MuxOverflow->InOne(m_sAdderOverflow);
		m_MuxOverflow->InTwo(m_sMultOverflow);

		m_MuxUnderflow->Select(OpCode);
		m_MuxUnderflow->Out(Underflow);
		m_MuxUnderflow->InOne(m_sAdderUnderflow);
		m_MuxUnderflow->InTwo(m_sMultUnderflow);
	}

private:

	//Binding signals
	sc_signal< sc_uint<32> > 	m_sAdderOut;
	sc_signal< sc_uint<1> > 	m_sAdderOverflow;
	sc_signal< sc_uint<1> > 	m_sAdderUnderflow;
	sc_signal< sc_uint<32> > 	m_sMultOut;
	sc_signal< sc_uint<1> > 	m_sMultOverflow;
	sc_signal< sc_uint<1> > 	m_sMultUnderflow;

	// Modules
	std::unique_ptr<FloatingMultiplier32> 	m_Multiplier;	// Multiplier Floating Point Unit
	std::unique_ptr<FloatingAdder32> 		m_Adder;		// Adder Floating Point Unit
	std::unique_ptr<MuxTwoPorts32Bits>		m_MuxOut;		// Multiplexer for output
	std::unique_ptr<MuxTwoPorts1Bit>		m_MuxOverflow;	// Multiplexer for overflow flag
	std::unique_ptr<MuxTwoPorts1Bit>		m_MuxUnderflow;	// Multiplexer for underflow flag
};

#endif
