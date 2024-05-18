#include "lpc17xx_gpio.h"

const uint8_t LedArray[]={0x01,0x03,0x07,0x0E,0x1C,0x38,0x70,0xE0};

void Delay()
{
   for(uint16_t count=0;count<UINT16_MAX;count++)
	    for(uint8_t count2=0;count2<5;count2++)
	     {
				 
			 }
}

int main()
{	
	FIO_ByteSetDir(0,2,0xFF,1);

	
while(1)
{	
	for(uint8_t count=0;count<3;count++)
	{
	  FIO_ByteSetValue(0,2,LedArray[count]);
		FIO_ByteClearValue(0,2,~LedArray[count]);
		Delay();
	}	
}
	
}
