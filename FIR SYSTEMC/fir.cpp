#include <systemc.h>
#include "fir.h"


// FIR thread principal

void fir::fir_main()
{

	sc_int<16> 			taps[5];

//Reset
for(int i = 0; i < 5; i++){
	taps[i] = 0;
}

input_ready.write(0);
output_valid.write(0);
output.write( 0 );
wait();

// FIR FILTRO

while( true ) 
	
{
	sc_int<16> in_val;
	sc_int<16> out_val;
	
	input_ready.write(1);
	do{wait();} while ( !input_valid.read() );
	in_val = input.read();
	input_ready.write(0);
	
	
 
 for(int i = 4; i > 0; i--){
 taps[i] = taps[i-1];
 }
 taps[0] = in_val;
 
 
 //Multiplicacion y acumulacion
 
 for(int i = 0; i < 5; i++){
 out_val += coef[i] * taps[i];
 }
 output_valid.write(1);
 output.write( out_val );
 do{wait();} while ( !output_ready.read() );
 output_valid.write(0);
}
} 

