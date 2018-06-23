#include "DFTModule.h"
#include <math.h>

void DFTModule::DTF( void )
{
    if(Reset)
    {
        std::cout << "*Resetting DTF Module*" << std::endl << std::endl;
        _N = 0;
        _Counter = 0;
    }
    else if(f_Enable.read())
    {
        std::cout << "*Enabling DFT Module*" << std::endl << std::endl;
        Samples = new double[N];
        _N = N;
        _Counter = 0;
    }
    else if(f_RxSamples.read())
    {
        if(_Counter == 0) 
	    std::cout << "*Receiving Samples*" << std::endl;
        
	Samples[_Counter++] = Sample;

        if(_Counter == _N)
        {
            std::cout << "*" << _Counter << " samples read*" << std::endl;
	    _Counter = 0;
        }
    }
    else if(f_Calculate.read())
    {
        if(_Counter == 0)
	    std::cout << "*Calculating DFT*" << std::endl;

        double resReal = 0, resImg = 0;
    
	for(int n = 0; n < _N; n++)
	{
            resReal += Samples[n] * cos(-2.0*M_PI*n*_Counter / _N);
	    resImg  += Samples[n] * sin(-2.0*M_PI*n*_Counter / _N);
	}
        
	OutReal = resReal;
        OutImg = resImg;

        if(++Counter == _N)
        {
            std::cout << "*Processed " << _Counter << " samples*" << std::endl;
            _Counter = 0;
        }
    }
}

