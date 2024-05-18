#include "Hardware.h"

#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_rtc.h"

#include "Delay_RIT.h"

void HW_Init()
{
	PINSEL_CFG_Type uartPinSel;
	UART_CFG_Type   uartConfig;

	NVIC_SetPriorityGrouping(0x07);
	
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
	
	RTC_Init(LPC_RTC);
	
	RTC_ResetClockTickCounter(LPC_RTC);
	RTC_Cmd(LPC_RTC,ENABLE);
	RTC_CalibCounterCmd(LPC_RTC,DISABLE);
	
	RTC_CntIncrIntConfig(LPC_RTC,RTC_TIMETYPE_SECOND,ENABLE);
	
	NVIC_EnableIRQ(RTC_IRQn);
}

void HW_SetDateTime()
{
	RTC_SetTime(LPC_RTC,RTC_TIMETYPE_SECOND,0);
	RTC_SetTime(LPC_RTC,RTC_TIMETYPE_MINUTE,0);
	RTC_SetTime(LPC_RTC,RTC_TIMETYPE_HOUR,0);
	
	RTC_SetTime(LPC_RTC,RTC_TIMETYPE_DAYOFMONTH,1);
	RTC_SetTime(LPC_RTC,RTC_TIMETYPE_MONTH,1);
	RTC_SetTime(LPC_RTC,RTC_TIMETYPE_YEAR,2018);
}
