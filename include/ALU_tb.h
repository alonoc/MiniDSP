#include "systemc.h" 
SC_MODULE(tb) {
	sc_out < sc_uint<4> > OPa,OPb;
	sc_out < sc_uint<4> > OP;
	sc_in<bool> Clk;

	void tbGen()   {

		OPa.write(0111);     
		OPb.write(0001);
		OP.write(0);

		wait(10,SC_NS);

		OPa.write(0111);     
		OPb.write(0001);
		OP.write(1);

		wait(10,SC_NS);

		OPa.write(0111);     
		OPb.write(0001);
		OP.write(2);

		wait(10,SC_NS);

		OPa.write(0111);     
		OPb.write(0001);
		OP.write(3);

		wait(10,SC_NS);

		OPa.write(0111);     
		OPb.write(0001);
		OP.write(4);

		wait(10,SC_NS);

		OPa.write(0111);     
		OPb.write(0001);
		OP.write(5);

		wait(10,SC_NS);


		sc_stop();   
	}   

	SC_CTOR(tb)   {
		SC_THREAD(tbGen);
	} 
};
