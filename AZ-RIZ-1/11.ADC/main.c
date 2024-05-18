#include "lpc17xx_gpio.h"
#include "lpc17xx_adc.h"
#include "lpc17xx_pinsel.h"


const uint8_t segmentTable[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

void DelayRefresh()
{
   for(uint32_t count=0;count<UINT16_MAX+40000;count++);  //6.4mS	    
}

int main()
{	
	uint8_t j=0;
	uint16_t volatge,buff,temp,total;
	float    calcVolt;
	PINSEL_CFG_Type adcpinsel;
	
	adcpinsel.Funcnum=PINSEL_FUNC_1;
	adcpinsel.OpenDrain=PINSEL_PINMODE_NORMAL;
	adcpinsel.Pinmode=PINSEL_PINMODE_TRISTATE;
	adcpinsel.Pinnum=23;
	adcpinsel.Portnum=0;
	
	PINSEL_ConfigPin(&adcpinsel);
	
	ADC_Init(LPC_ADC,200000);
	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_0,ENABLE);
	

	/*---7 Segment Data---*/
	FIO_ByteSetDir(1,2,0xff,1);
	
	/*---7 Segment Digits---*/
	GPIO_SetDir(0,(1<<1),1);
	GPIO_SetDir(0,(1<<2),1);
	GPIO_SetDir(0,(1<<3),1);
	
	GPIO_SetDir(1,(1<<26),1);

while(1)
{		
  ADC_StartCmd(LPC_ADC,ADC_START_NOW);
	while(ADC_ChannelGetStatus(LPC_ADC,ADC_CHANNEL_0,ADC_DATA_DONE)==0)
	{
		
	}
	j++;
	volatge=ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_0);
	/*
	 3        4096
	 x        volt
   x=volt*3/4096	
	*/
	
	calcVolt=(volatge*3.0F)/4096;
  buff=(calcVolt*100);
	total = total+buff;
if (j==50){
   temp = total/50;
	j=0;
	total=0;
	buff=0;
}
	if(temp>24)
	{
		GPIO_SetValue(1,(1<<26));		
	}
	else
	{
		GPIO_ClearValue(1,(1<<26));
	}
	
  GPIO_ClearValue(0,(1<<2));
	GPIO_SetValue(0,(1<<1));
	FIO_ByteSetValue(1,2,segmentTable[temp/10]);
	FIO_ByteClearValue(1,2,~segmentTable[temp/10]);
	DelayRefresh();
	GPIO_ClearValue(0,(1<<1));
	GPIO_SetValue(0,(1<<2));
	FIO_ByteSetValue(1,2,segmentTable[temp%10]);
	FIO_ByteClearValue(1,2,~segmentTable[temp%10]);
	DelayRefresh();

}
}
