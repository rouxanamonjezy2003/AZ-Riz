#include "Hardware.h"

#include "lpc17xx_gpio.h"
#include "lpc17xx_adc.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_uart.h"

#include "Delay_RIT.h"
#include "clcd.h"

uint16_t volatge,realTemp=0;
float    calcVolt;
uint32_t total=0;
uint8_t countOfSum=0;

volatile uint8_t flagOfTemp=0;

void ADC_IRQHandler()
{
	if(ADC_ChannelGetStatus(LPC_ADC,ADC_CHANNEL_3,ADC_DATA_DONE)==0)
	{
		uint16_t temp;
		volatge=ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_3);
	/*
	 3        4096
	 x        volt
   x=volt*3/4096	
	*/
	
	calcVolt=(volatge*3.0F)/4096;
  temp=(calcVolt*100);
	
	total+=temp;
	countOfSum++;
		if(countOfSum==100)
		{
			countOfSum=0;
			realTemp=total/100;
			total=0;
			flagOfTemp=1;
		
		}
		else
		 ADC_StartCmd(LPC_ADC,ADC_START_NOW);		
	}
}

uint16_t HW_ADC_Read()
{
	flagOfTemp=0;
	ADC_StartCmd(LPC_ADC,ADC_START_NOW);		
	
	while(flagOfTemp==0);
	
	return realTemp;
}

void HW_Init()
{
	PINSEL_CFG_Type adcpinsel;

	NVIC_SetPriorityGrouping(0x07);
	
	adcpinsel.Funcnum=PINSEL_FUNC_1;
	adcpinsel.OpenDrain=PINSEL_PINMODE_NORMAL;
	adcpinsel.Pinmode=PINSEL_PINMODE_TRISTATE;
	adcpinsel.Pinnum=26;
	adcpinsel.Portnum=0;
	
	PINSEL_ConfigPin(&adcpinsel);
	
	ADC_Init(LPC_ADC,200000);
	ADC_IntConfig(LPC_ADC,ADC_ADINTEN3,ENABLE);
	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_3,ENABLE);

	NVIC_SetPriority(ADC_IRQn,NVIC_EncodePriority(0x07,0,1));
  NVIC_EnableIRQ(ADC_IRQn);	
	
	Delay_RIT_Init();

	lcd_init();
}
