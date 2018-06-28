#include "tb.h"

void tb::source() {
	
	sc_int<16> tmp;
	
	//Reset
	input.write( 0 );
	input_valid.write(0);
	reset.write( 1 );
	wait();
	reset.write( 0 );
	wait();
	
	
	// Enviar estimulo al filtro
	for ( int i = 0; i < 64; i++)
	{
		if (i >23 && i < 29)
			tmp = 256;
		else
			tmp = 0;
		input_valid.write(1);
		input.write( tmp );
		do {
			wait();
		} while( !input_ready.read() );
		input_valid.write(0);
		
}
}

void tb::sink() {
	sc_int<16> indata;
	
	// Inicializacion puerto
	
	output_ready.write(o);
	
	
		for ( int i = 0; i < 64; i++)
	{
		output_ready.write(1);
		do{
			wait();
		} while ( !output_valid.read() );
		indata = output.read();
		output_ready.write(0);
		cout << i << " :\t" << indata.to_int() << end1:
	}
	// Final de simulacion
	sc_stop();