#include "Delay_RIT.h"
#include "lpc17xx_rit.h"

volatile uint8_t ritFlag=0;

void RIT_IRQHandler(void);

void RIT_IRQHandler()
{
	RIT_GetIntStatus(LPC_RIT);	
	ritFlag=1;
}

void Delay_RIT_Init()
{
	RIT_Init(LPC_RIT);
	NVIC_SetPriority(RIT_IRQn,NVIC_EncodePriority(0x07,0,1));
	NVIC_EnableIRQ(RIT_IRQn);	
	RIT_Cmd(LPC_RIT,DISABLE);
}

void Delay_RIT_ms(uint32_t timMs)
{
 ritFlag=0;
 RIT_TimerConfig(LPC_RIT,timMs);	
 RIT_Cmd(LPC_RIT,ENABLE);
 while(ritFlag==0);
 RIT_Cmd(LPC_RIT,DISABLE);
}
