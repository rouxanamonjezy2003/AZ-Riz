#include "lpc17xx_gpio.h"
#include "lpc17xx_pwm.h"
#include "lpc17xx_pinsel.h"

#include "Delay_RIT.h"

int main()
{	
 PWM_TIMERCFG_Type pwmTimCfg;
 PWM_MATCHCFG_Type pwmMatch0;	
 PINSEL_CFG_Type  pwmPinsel;
	
	Delay_RIT_Init();
	
 
	pwmTimCfg.PrescaleOption=PWM_TIMER_PRESCALE_TICKVAL;
	pwmTimCfg.PrescaleValue=1;
	PWM_Init(LPC_PWM1,PWM_MODE_TIMER,&pwmTimCfg);

	pwmMatch0.IntOnMatch=DISABLE;
	pwmMatch0.MatchChannel=0;
	pwmMatch0.ResetOnMatch=ENABLE;
	pwmMatch0.StopOnMatch=DISABLE;
	
	PWM_ConfigMatch(LPC_PWM1,&pwmMatch0);
	
	PWM_MatchUpdate(LPC_PWM1,0,100,PWM_MATCH_UPDATE_NOW);
	
	PWM_ChannelCmd(LPC_PWM1,1,ENABLE);
	
	PWM_ResetCounter(LPC_PWM1);
	PWM_CounterCmd(LPC_PWM1,ENABLE);
	
	PWM_Cmd(LPC_PWM1,ENABLE);
	
	pwmPinsel.Funcnum=PINSEL_FUNC_1;
	pwmPinsel.OpenDrain=PINSEL_PINMODE_NORMAL;
	pwmPinsel.Pinmode=PINSEL_PINMODE_TRISTATE;
	pwmPinsel.Pinnum=0;
	pwmPinsel.Portnum=2;
	
	PINSEL_ConfigPin(&pwmPinsel);
	
	

	PWM_MatchUpdate(LPC_PWM1,1,100,PWM_MATCH_UPDATE_NOW);
	Delay_RIT_ms(5000);	
	
	while(1)
	{
		
		
	}
	
}
