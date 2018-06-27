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
	// Basic testing
	sc_signal<sc_uint<32> > AddInA;
	sc_signal<sc_uint<32> > AddInB;
	sc_signal<sc_uint<32> > AddOut;
	sc_signal<sc_uint<1> > Overflow;
	sc_signal<sc_uint<1> > Underflow;
	FloatingAdder32 FAdd32("FADD32");

	// Connect ports
	FAdd32.OpA(AddInA);
	FAdd32.OpB(AddInB);
	FAdd32.Result(AddOut);
	FAdd32.Overflow(Overflow);
	FAdd32.Underflow(Underflow);
	
	// VCD file
	sc_trace_file* TraceLogger = sc_create_vcd_trace_file("FADD32 TEST");
	TraceLogger->set_time_unit(1, SC_NS);
	
	// Select signals to dump
	sc_trace(TraceLogger, AddInA, "OpA");
	sc_trace(TraceLogger, AddInB, "OpB");
	sc_trace(TraceLogger, AddOut, "Out");
	sc_trace(TraceLogger, Overflow, "Overflow");
	sc_trace(TraceLogger, Underflow, "Underflow");
	
	// {====================}
	// {	NORMAL CASES	}
	// {====================}
	
	// Initialize values
	AddInA = 0x00000000;
	AddInB = 0x00000000;
	sc_start(1, SC_NS);

	// Test 1 (Result = 36 or 0x42100000)
	AddInA = 0x414C0000;	//  12.75
	AddInB = 0x41BA0000;	//  23.25
	sc_start(1, SC_NS);

	// Test 1 (Result = -15002.25 or 0xC66A6900)
	AddInA = 0xBFC00000;	//  -1.5
	AddInB = 0xC66A6300;	//  -15000.75
	sc_start(1, SC_NS);

	// {====================}
	// {	SPECIAL CASES	}
	// {====================}
	
	sc_close_vcd_trace_file(TraceLogger);
}

void TestFPU32()
{
	// Input signals
	sc_signal< sc_uint<32> > OpA;
	sc_signal< sc_uint<32> > OpB;
	sc_signal< sc_uint<2> > OpCode;

	// Output signals
	sc_signal< sc_uint<32> > Out;
	sc_signal< sc_uint<1> > Overflow;
	sc_signal< sc_uint<1> > Underflow;

	// DUT
	FPU32 Fpu32("FPU32");

	// Mapping
	Fpu32.OpA(OpA);
	Fpu32.OpB(OpB);
	Fpu32.Out(Out);
	Fpu32.OpCode(OpCode);
	Fpu32.Overflow(Overflow);
	Fpu32.Underflow(Underflow);
	
	// VCD file
	sc_trace_file* TraceLogger = sc_create_vcd_trace_file("FPU32 TEST");
	TraceLogger->set_time_unit(1, SC_NS);
	
	// Select signals to dump
	sc_trace(TraceLogger, OpA, "OpA");
	sc_trace(TraceLogger, OpB, "OpB");
	sc_trace(TraceLogger, Out, "Out");
	sc_trace(TraceLogger, OpCode, "OpCode");
	sc_trace(TraceLogger, Overflow, "Overflow");
	sc_trace(TraceLogger, Underflow, "Underflow");

	// Test #1 OpCode 2 [Unsupported Opcode, everything should be 0]
	OpCode = 2;
	OpA = 0xFFFFFFFF;
	OpB = 0xFFFFFFFF;
	sc_start(1, SC_NS);

	// Test #2 OpCode 3 [Unsupported OpCode, everything should be 0]
	OpCode = 3;
	OpA = 0xFFFFFFFF;
	OpB = 0xFFFFFFFF;
	sc_start(1, SC_NS);

	// Test #3 OpCode 0 [Values should be added, Result = 36 or 0xC32B0000]
	OpCode = 0;
	OpA = 0x414C0000;	//  12.75
	OpB = 0x41BA0000;	//  23.25
	sc_start(1, SC_NS);

	// Test #4 OpCode 1 [Values should be multiplied, Result = 46.875 or 0x423B8000]
	OpCode = 1;
	OpA = 0x41480000;	//  12.5
	OpB = 0x40700000;	//  3.75
	sc_start(1, SC_NS);

	// Test #5 OpCode 1 [Values should be multiplied, Result = -Inf 0xFF800000, and Overflow]
	OpCode = 1;
	OpA = 0x7F480000;	//  2.552117 E38
	OpB = 0xFF480000;	// -2.552117 E38
	sc_start(1, SC_NS);

	sc_close_vcd_trace_file(TraceLogger);
}