#include "lpc17xx_gpio.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_pinsel.h"

#include "Delay_RIT.h"

const uint8_t segmentTable[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

void TIMER0_IRQHandler(void);

uint32_t countofPulse=0;
volatile uint8_t flagOfCount=0;

void TIMER0_IRQHandler()
{
	if(TIM_GetIntStatus(LPC_TIM0,TIM_MR0_INT))
	{
		TIM_ClearIntPending(LPC_TIM0,TIM_MR0_INT);
		//1S repeat
		 countofPulse=LPC_TIM2->TC;
		 LPC_TIM2->TC=0;		
		 flagOfCount=1;
	}	
}


int main()
{		
	uint32_t tempCount=0;
	PINSEL_CFG_Type   pinselCFg;
	TIM_COUNTERCFG_Type cntCfg;
	TIM_TIMERCFG_Type   timCfg;
	TIM_MATCHCFG_Type   matCfg;
	
	cntCfg.CounterOption=TIM_COUNTER_FALLING_MODE;
	cntCfg.CountInputSelect=TIM_COUNTER_INCAP0;
	
	TIM_Init(LPC_TIM2,TIM_COUNTER_FALLING_MODE,&cntCfg);
	
	TIM_Cmd(LPC_TIM2,ENABLE);

	timCfg.PrescaleOption=TIM_PRESCALE_USVAL;
	timCfg.PrescaleValue=1000;
	
	TIM_Init(LPC_TIM0,TIM_TIMER_MODE,&timCfg);
	
	matCfg.ExtMatchOutputType=DISABLE;
	matCfg.IntOnMatch=ENABLE;
	matCfg.MatchChannel=0;
	matCfg.MatchValue=1000;
	matCfg.ResetOnMatch=ENABLE;
	matCfg.StopOnMatch=0;
	
	TIM_ConfigMatch(LPC_TIM0,&matCfg);
	
	NVIC_EnableIRQ(TIMER0_IRQn);
	TIM_Cmd(LPC_TIM0,ENABLE);
	
	
	
	Delay_RIT_Init();
	/*---7 Segment Data---*/
	FIO_ByteSetDir(1,2,0xff,1);
	
	/*---7 Segment Digits---*/
	GPIO_SetDir(0,(1<<1),1);
	GPIO_SetDir(0,(1<<2),1);
	GPIO_SetDir(0,(1<<3),1);
	
	pinselCFg.Funcnum=PINSEL_FUNC_3;
	pinselCFg.OpenDrain=PINSEL_PINMODE_NORMAL;
	pinselCFg.Pinmode=PINSEL_PINMODE_TRISTATE;
	pinselCFg.Pinnum=4;
	pinselCFg.Portnum=0;
	PINSEL_ConfigPin(&pinselCFg);
		
while(1)
{
	if(flagOfCount==1)
	{
		flagOfCount=0;
		tempCount=countofPulse;
	}
	
	
	GPIO_ClearValue(0,(1<<3));
	GPIO_SetValue(0,(1<<1));
	FIO_ByteSetValue(1,2,segmentTable[tempCount/100]);
	FIO_ByteClearValue(1,2,~segmentTable[tempCount/100]);
	Delay_RIT_ms(6);
	GPIO_ClearValue(0,(1<<1));
	GPIO_SetValue(0,(1<<2));
	FIO_ByteSetValue(1,2,segmentTable[(tempCount%100)/10]);
	FIO_ByteClearValue(1,2,~segmentTable[(tempCount%100)/10]);
	Delay_RIT_ms(6);
	GPIO_ClearValue(0,(1<<2));
	GPIO_SetValue(0,(1<<3));
	FIO_ByteSetValue(1,2,segmentTable[(tempCount%100)%10]);
	FIO_ByteClearValue(1,2,~segmentTable[(tempCount%100)%10]);
	Delay_RIT_ms(6);
}
	
}
