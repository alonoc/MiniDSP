#include "FPU32.h"

//PLEASE DELETE THIS INCLUDE
#include <iostream>

void FloatingAdder32::doIt( void )
{

}

void FloatingMultiplier32::doIt( void )
{
	const sc_uint<32> c_ZeroPos = 0x00000000;
	const sc_uint<32> c_ZeroNeg = 0x80000000;
	sc_uint<24> MantissaOpA;
	sc_uint<24> MantissaOpB;
	sc_uint<48> MantissaMul;
	sc_uint<8> ExpOpA;
	sc_uint<8> ExpOpB;
	sc_uint<8> Bias;
	sc_uint<9> ExpAddition;
	sc_uint<32> TempResult;

	// Reset flags
	Overflow.write(0);
	Underflow.write(0);

	if(	c_ZeroPos == OpA  || c_ZeroNeg == OpA ||
	 	c_ZeroPos == OpB  || c_ZeroNeg == OpB)
	{
	 	TempResult = 0;
	}
	else
	{
		// Let's calculate the sign of the multiplication with a XOR
		TempResult[31] = OpA.read()[31] ^ OpB.read()[31];

		// Let's calculate the mantissa of the multiplication
		MantissaOpA.range(22,0) = OpA.read().range(22,0);
		MantissaOpA[23] = 1;
		MantissaOpB.range(22,0) = OpB.read().range(22,0);
		MantissaOpB[23] = 1;

		// A multiplication of 24 bits with 24 bits requires 48 bits
		MantissaMul = MantissaOpA * MantissaOpB;

		std::cout << "Mantissa OpA = " << MantissaOpA << std::endl;
		std::cout << "Mantissa OpB = " << MantissaOpB << std::endl;
		std::cout << "Mantissa after mul = " << MantissaMul << std::endl;
		
		// Now let's truncate the result
		TempResult.range(22,0) = MantissaMul.range(45,23);
		
		// Let's calculate the exponent of the multiplication
		ExpOpA = OpA.read().range(30,23);
		ExpOpB = OpB.read().range(30,23);
		Bias = 127;
		ExpAddition = ExpOpA + ExpOpB - Bias;

		TempResult.range(30,23) = ExpAddition.range(7,0);
	}

	// Finally just write the result in the port
	Result.write(TempResult);
}