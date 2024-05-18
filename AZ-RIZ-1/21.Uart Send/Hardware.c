#include "Hardware.h"

#include "lpc17xx_gpio.h"
#include "lpc17xx_adc.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_uart.h"

#include "Delay_RIT.h"

uint16_t volatge,realTemp=0;
float    calcVolt;
uint32_t total=0;
uint8_t countOfSum=0;

volatile uint8_t flagOfTemp=0;

void ADC_IRQHandler()
{
	if(ADC_ChannelGetStatus(LPC_ADC,ADC_CHANNEL_0,ADC_DATA_DONE)==0)
	{
		uint16_t temp;
		volatge=ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_0);
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
	PINSEL_CFG_Type uartPinSel;
	UART_CFG_Type   uartConfig;

	NVIC_SetPriorityGrouping(0x07);
	
	adcpinsel.Funcnum=PINSEL_FUNC_1;
	adcpinsel.OpenDrain=PINSEL_PINMODE_NORMAL;
	adcpinsel.Pinmode=PINSEL_PINMODE_TRISTATE;
	adcpinsel.Pinnum=23;
	adcpinsel.Portnum=0;
	
	PINSEL_ConfigPin(&adcpinsel);
	
	ADC_Init(LPC_ADC,200000);
	ADC_IntConfig(LPC_ADC,ADC_ADINTEN0,ENABLE);
	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_0,ENABLE);

	NVIC_SetPriority(ADC_IRQn,NVIC_EncodePriority(0x07,0,1));
  NVIC_EnableIRQ(ADC_IRQn);	
	
	Delay_RIT_Init();
	
	uartPinSel.Funcnum=PINSEL_FUNC_1;
	uartPinSel.OpenDrain=PINSEL_PINMODE_NORMAL;
	uartPinSel.Pinmode=PINSEL_PINMODE_TRISTATE;
	uartPinSel.Pinnum=2;
	uartPinSel.Portnum=0;
	
	PINSEL_ConfigPin(&uartPinSel);
	uartPinSel.Pinnum=3;
	PINSEL_ConfigPin(&uartPinSel);
	
  uartConfig.Baud_rate=115200;
  uartConfig.Databits=UART_DATABIT_8;
	uartConfig.Parity=UART_PARITY_NONE;
	uartConfig.Stopbits=UART_STOPBIT_1;
	
	UART_Init(LPC_UART0,&uartConfig);
	UART_TxCmd(LPC_UART0,ENABLE);
	
}
