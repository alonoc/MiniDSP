#include "FPU32.h"

void FloatingAdder32::Add( void )
{
	sc_uint<24> MantissaOpA = 0;
	sc_uint<24> MantissaOpB = 0;
	sc_uint<25> MantissaAdd = 0;
	sc_uint<8> ExpOpA = 0;
	sc_uint<8> ExpOpB = 0;
	sc_uint<8> ExpDiff = 0;
	sc_uint<9> ExpResult = 0;
	sc_uint<32> TempResult = 0;

	// Reset flags
	Overflow.write(0);
	Underflow.write(0);

	//Assuming addition of equal sign just take the sign of the first operator
	TempResult[31] = OpA.read()[31];
	
	// Read the mantissas
	MantissaOpA.range(22,0) = OpA.read().range(22,0);
	MantissaOpB.range(22,0) = OpB.read().range(22,0);
	MantissaOpA[23] = 1;
	MantissaOpB[23] = 1;
	
	// Read exponents
	ExpOpA = OpA.read().range(30,23);
	ExpOpB = OpB.read().range(30,23);
	
	// If OpA is greater than OpB
	// Then shift the Mantissa of OpB until its exponent matches the Exponent of OpA
	if(ExpOpA > ExpOpB)
	{
	 	ExpResult.range(7,0) = ExpOpA;
	 	ExpDiff = ExpOpA - ExpOpB;
	 	MantissaOpB >>= ExpDiff;
	}
	// If OpB is greater than OpA
	// Then shift the Mantissa of OpA until its exponent matches the Exponent of OpB
	else if(ExpOpA != ExpOpB)
	{
		ExpResult.range(7,0) = ExpOpB;
	 	ExpDiff = ExpOpB - ExpOpA;
	 	MantissaOpA >>= ExpDiff;
	}

	// Add the mantissas
	MantissaAdd = MantissaOpA + MantissaOpB;

	if(1 == MantissaAdd[24])
	{
	 	MantissaAdd >>= 1;
	 	ExpResult += 1;
	}
	
	if(ExpResult > 254)
	{
 		// Then there's overflow
	 	// 1. Turn on the overflow flag
	 	// 2. Return +/- Inf (the exponent is set to all 1s and the mantissa is set to 0, to indicate +/- infinity)
	 	Overflow.write(1);
	 	TempResult.range(30,23) = 255;
	 	TempResult.range(22,0) = 0;
	}
	else
	{
		// Just write the result
		TempResult.range(30,23) = ExpResult.range(7,0);
		TempResult.range(22,0) = MantissaAdd.range(22,0);
	}
	
	// Finally just write the result in the port
	Result.write(TempResult);
}

void FloatingMultiplier32::Multiply( void )
{
	const sc_uint<8> c_Bias = 127;
	sc_uint<24> MantissaOpA = 0;
	sc_uint<24> MantissaOpB = 0;
	sc_uint<48> MantissaMul = 0;
	sc_uint<8> ExpOpA = 0;
	sc_uint<8> ExpOpB = 0;
	sc_uint<9> ExpResult = 0;
	sc_uint<32> TempResult = 0;

	// Reset flags
	Overflow.write(0);
	Underflow.write(0);

	// Let's calculate the sign of the multiplication with a XOR
	TempResult[31] = OpA.read()[31] ^ OpB.read()[31];
	
	// If one of the operands is 0 then return 0
	if(	0 == OpA.read().range(30,0) || 0 == OpB.read().range(30,0))
	{
		TempResult.range(30,0) = 0;
	}
	else
	{
		// 	// Let's calculate the mantissa of the multiplication
		MantissaOpA.range(22,0) = OpA.read().range(22,0);
		MantissaOpB.range(22,0) = OpB.read().range(22,0);

		// Add hidden bit
		MantissaOpA[23] = 1;
		MantissaOpB[23] = 1;

		// A multiplication of 24 bits with 24 bits requires 48 bits
		MantissaMul = MantissaOpA * MantissaOpB;

		// If this happens, the mantissa should be normalized
		if(1 == MantissaMul[47])
		{
			MantissaMul >>= 1;
			ExpResult = 1;
		}

		// Now let's truncate the result
		TempResult.range(22,0) = MantissaMul.range(45,23);

		// Let's calculate the exponent of the multiplication
		ExpOpA = OpA.read().range(30,23);
		ExpOpB = OpB.read().range(30,23);
		ExpResult += (ExpOpA + ExpOpB - c_Bias);

		if(0 == ExpResult[8])
		{
			// Then write the exponent
		 	TempResult.range(30,23) = ExpResult.range(7,0);
		}
		else
		{
		 	if(0 == ExpResult[7])
		 	{
		 		// Then there's overflow
			 	// 1. Turn on the overflow flag
			 	// 2. Return +/- Inf (the exponent is set to all 1s and the mantissa is set to 0, to indicate +/- infinity)
			 	Overflow.write(1);
			 	TempResult.range(30,23) = 255;
			 	TempResult.range(22,0) = 0;
		 	}
		 	else
		 	{
		 		// Then there's underflow
		 		// 1. Turn on the overflow flag
		 		// 2. Return 0 because there's a loss of precision, which is guaranteed to be closely approximated by zero
		 		Underflow.write(1);
		 		TempResult.range(30,23) = 0;
			 	TempResult.range(22,0) = 0;
		 	}

		}
	}
	
	// Finally just write the result in the port
	Result.write(TempResult);
}