//https://xantorohara.github.io/led-matrix-editor/#ffffffff0f0f0f0f
//http://dotmatrixtool.com/

#include "lpc17xx_gpio.h"


const uint8_t segmentTable[7][8]={
{
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
},
{
0x00, 0x7e, 0x04, 0x08, 0x10, 0x08, 0x04, 0x7e
},  
{
0x00, 0x00, 0x82, 0xfe, 0x82, 0x00, 0x00, 0x00
},
{
0x00, 0xfe, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00
},
{
0x00, 0x78, 0x14, 0x12, 0x12, 0x14, 0x78, 0x00
},
{
0xff, 0x81, 0x81, 0x42, 0x3c, 0x00, 0x00, 0x00
},
{
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
}
};


void Delay()
{
   for(uint16_t count=0;count<40000;count++)
	    //for(uint8_t count2=0;count2<0;count2++)
	     {		 
			 }
}

int main()
{	
	uint8_t myNum=0,myNum1=0,countOfDelay=0,countOfDelay1=0;
	
	FIO_ByteSetDir(1,2,0xFF,1);
	GPIO_SetDir(2,(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7)|(1<<8),1);
	
	
		
while(1)
{		
	GPIO_ClearValue(2,(1<<8));
	GPIO_SetValue(2,(1<<1));
	FIO_ByteSetValue(1,2,segmentTable[myNum][myNum1]);
	FIO_ByteClearValue(1,2,~segmentTable[myNum][myNum1]);
	Delay();
	GPIO_ClearValue(2,(1<<1));
	GPIO_SetValue(2,(1<<2));
	FIO_ByteSetValue(1,2,segmentTable[myNum][myNum1+1]);
	FIO_ByteClearValue(1,2,~segmentTable[myNum][myNum1+1]);
	Delay();
	GPIO_ClearValue(2,(1<<2));
	GPIO_SetValue(2,(1<<3));
	FIO_ByteSetValue(1,2,segmentTable[myNum][myNum1+2]);
	FIO_ByteClearValue(1,2,~segmentTable[myNum][myNum1+2]);
	Delay();
		GPIO_ClearValue(2,(1<<3));
	GPIO_SetValue(2,(1<<4));
	FIO_ByteSetValue(1,2,segmentTable[myNum][myNum1+3]);
	FIO_ByteClearValue(1,2,~segmentTable[myNum][myNum1+3]);
	Delay();
		GPIO_ClearValue(2,(1<<4));
	GPIO_SetValue(2,(1<<5));
	FIO_ByteSetValue(1,2,segmentTable[myNum][myNum1+4]);
	FIO_ByteClearValue(1,2,~segmentTable[myNum][myNum1+4]);
	Delay();
		GPIO_ClearValue(2,(1<<5));
	GPIO_SetValue(2,(1<<6));
	FIO_ByteSetValue(1,2,segmentTable[myNum][myNum1+5]);
	FIO_ByteClearValue(1,2,~segmentTable[myNum][myNum1+5]);
	Delay();
		GPIO_ClearValue(2,(1<<6));
	GPIO_SetValue(2,(1<<7));
	FIO_ByteSetValue(1,2,segmentTable[myNum][myNum1+6]);
	FIO_ByteClearValue(1,2,~segmentTable[myNum][myNum1+6]);
	Delay();
		GPIO_ClearValue(2,(1<<7));
	GPIO_SetValue(2,(1<<8));
	FIO_ByteSetValue(1,2,segmentTable[myNum][myNum1+7]);
	FIO_ByteClearValue(1,2,~segmentTable[myNum][myNum1+7]);
	Delay();
	
	countOfDelay++;
	countOfDelay1++;
	if(countOfDelay==24)
	{
		countOfDelay=0;
		myNum++;
		if(myNum==6){
			myNum=0;
	  	//myNum1++;
		}
	}
		if(countOfDelay1==3)
	{
		countOfDelay1=0;
		myNum1++;
		 if(myNum1==8){
	  	myNum1=0;
		}
	}

}
	
}
