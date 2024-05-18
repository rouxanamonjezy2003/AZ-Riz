#include "lpc17xx_gpio.h"

void Delay()
{
   for(uint16_t count=0;count<UINT16_MAX;count++)
	    for(uint8_t count2=0;count2<10;count2++)
	     {
				 
			 }
}

int main()
{	
	GPIO_SetDir(1,(1<<16),1);

while(1)
{	
	GPIO_SetValue(1,(1<<16));
	Delay();
	GPIO_ClearValue(1,(1<<16));	
	Delay();
}
}