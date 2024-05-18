#include <stdio.h>

#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_rtc.h"
#include "lpc17xx_uart.h"

#include "hardware.h"
#include "Delay_RIT.h"

void RTC_IRQHandler(void);

uint8_t  bufferUart[50];
uint8_t    lenBuf;
RTC_TIME_Type timeDate;


void RTC_IRQHandler()
{
  if(RTC_GetIntPending(LPC_RTC,RTC_INT_COUNTER_INCREASE))
	{
		RTC_ClearIntPending(LPC_RTC,RTC_INT_COUNTER_INCREASE);
		//1 Sec
	RTC_GetFullTime(LPC_RTC,&timeDate);

		lenBuf=sprintf(bufferUart,"%02u:%02u:%02u\n",timeDate.HOUR,timeDate.MIN,timeDate.SEC);
		
   	UART_Send(LPC_UART0,bufferUart,lenBuf,BLOCKING);

			
	
	}
}

int main()
{	
	HW_Init();

	HW_SetDateTime();
		
	while(1)
	{
		
		Delay_RIT_ms(1000);		
	}
	
}
