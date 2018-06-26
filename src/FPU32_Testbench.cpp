#include "FPU32_Testbench.h"
#include "FPU32.h"
#include "systemc.h"

void TestFloatingMultiplier32()
{
	// Basic testing
	sc_signal<sc_uint<32> > MultInA;
	sc_signal<sc_uint<32> > MultInB;
	sc_signal<sc_uint<32> > MultOut;
	sc_signal<sc_uint<1> > Overflow;
	sc_signal<sc_uint<1> > Underflow;
	FloatingMultiplier32 FMult32("FMULT32");

	// Connect ports
	FMult32.OpA(MultInA);
	FMult32.OpB(MultInB);
	FMult32.Result(MultOut);
	FMult32.Overflow(Overflow);
	FMult32.Underflow(Underflow);

	// VCD file
	sc_trace_file* TraceLogger = sc_create_vcd_trace_file("FMULT32 TEST");
	TraceLogger->set_time_unit(1, SC_NS);

	// Select signals to dump
	sc_trace(TraceLogger, MultInA, "OpA");
	sc_trace(TraceLogger, MultInB, "OpB");
	sc_trace(TraceLogger, MultOut, "Out");
	sc_trace(TraceLogger, Overflow, "Overflow");
	sc_trace(TraceLogger, Underflow, "Underflow");
	
	// Initialize values
	MultInA = 0x00000000;
	MultInB = 0x00000000;
	sc_start(1, SC_NS);
	
	// {====================}
	// {	NORMAL CASES	}
	// {====================}

	// Test 1 (Result=-171 or 0xC32B0000)
	MultInA = 0xC1900000;	// -18.0
	MultInB = 0x41180000;	//  9.5
	sc_start(1, SC_NS);

	// Test 2 (Result=46.875 or 0x423B8000)
	MultInA = 0x41480000;	//  12.5
	MultInB = 0x40700000;	//  3.75
	sc_start(1, SC_NS);
		
	// Test 3 (Result=3251.21875 or 0x454B3380)
	MultInA = 0x44FA1800;	//  2000.75
	MultInB = 0x3FD00000;	//  1.625
	sc_start(1, SC_NS);

	// Test 4 (Result=151.125 or 0x43172000)
	MultInA = 0xC2C98000;	// -100.75
	MultInB = 0xBFC00000;	// -1.5
	sc_start(1, SC_NS);

	// {====================}
	// {	SPECIAL CASES	}
	// {====================}

	// Test 1 Multiplication for Zero using OpA (Result = 0)
	MultInA = 0x00000000;	//  0.0
	MultInB = 0x41180000;	//  9.5
	sc_start(1, SC_NS);

	// Test 2 Multiplication for Zero using OpB (Result = 0)
	MultInA = 0x41180000;	//  9.5
	MultInB = 0x00000000;	//  0.0
	sc_start(1, SC_NS);

	// Test 3 Overflow (Result = +Infinity)
	MultInA = 0x7F480000;	//  2.552117 E38
	MultInB = 0x7F480000;	//  2.552117 E38
	sc_start(1, SC_NS);

	// Test 4 Overflow (Result = -Infinity)
	MultInA = 0x7F480000;	//  2.552117 E38
	MultInB = 0xFF480000;	// -2.552117 E38
	sc_start(1, SC_NS);

	// Test 5 Underflow (Result = Zero)
	MultInA = 0x00C00000;	//  1.7632415 E-38
	MultInB = 0x00C00000;	//  1.7632415 E-38
	sc_start(1, SC_NS);
	
	sc_close_vcd_trace_file(TraceLogger);
}

void TestFloatingAdder32()
{

}