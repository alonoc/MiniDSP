#include "ALU.h"

void ALU::operate()
{
    {
        ZERO = false;
        data1 = OPa.read();
        data2 = OPb.read();
        
        switch(OP.read())
        {
                
            case 0:
                result = data1 + data2;
                break;
                
            case 1:
                result = data1 - data2;
                break;
                
            case 2:
                result = data1 + 1;
                break;
                
            case 3:
                result = data1 - 1;
                break;
                
            case 4:
                result = data1 && data2;
                break;
                
            case 5:
                result = data1 || data2;
                break;
                
                            
        }
        RESULT = result;
       // RESULT.write(result.range(3, 0));
        
              
        if(result == 0)
            ZERO.write(true);
    }
}
