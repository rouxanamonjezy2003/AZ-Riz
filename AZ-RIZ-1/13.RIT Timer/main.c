#include "lpc17xx_gpio.h"
#include "lpc17xx_rit.h"

const uint8_t segmentTable[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

void DelayRefresh()
{
   for(uint32_t count=0;count<UINT16_MAX+40000;count++);  //6.4mS	    
}


void RIT_IRQHandler(void);
volatile uint8_t ritFlag=0;

void RIT_IRQHandler()
{
	RIT_GetIntStatus(LPC_RIT);	
	ritFlag=1;
}

void Delay_RIT_Init()
{
	RIT_Init(LPC_RIT);
	NVIC_EnableIRQ(RIT_IRQn);	
}

void Delay_RIT_ms(uint32_t timMs)
{
 ritFlag=0;
 RIT_TimerConfig(LPC_RIT,timMs);
 RIT_Cmd(LPC_RIT,ENABLE);
 while(ritFlag==0);
 RIT_Cmd(LPC_RIT,DISABLE);
}


int main()
{	
	Delay_RIT_Init();
	/*---7 Segment Data---*/
	FIO_ByteSetDir(1,2,0xff,1);
	
	/*---7 Segment Digits---*/
	GPIO_SetDir(0,(1<<1),1);
	GPIO_SetDir(0,(1<<2),1);
	GPIO_SetDir(0,(1<<3),1);
	
	GPIO_SetDir(1,(1<<26),1);

while(1)
{	
  GPIO_ClearValue(0,(1<<3));
	GPIO_SetValue(0,(1<<1));
	FIO_ByteSetValue(1,2,segmentTable[0]);
	FIO_ByteClearValue(1,2,~segmentTable[0]);
	Delay_RIT_ms(20);
	GPIO_ClearValue(0,(1<<1));
	GPIO_SetValue(0,(1<<2));
	FIO_ByteSetValue(1,2,segmentTable[1]);
	FIO_ByteClearValue(1,2,~segmentTable[1]);
	Delay_RIT_ms(20);
	GPIO_ClearValue(0,(1<<2));
	GPIO_SetValue(0,(1<<3));
	FIO_ByteSetValue(1,2,segmentTable[2]);
	FIO_ByteClearValue(1,2,~segmentTable[2]);
	Delay_RIT_ms(20);
}
	
}
