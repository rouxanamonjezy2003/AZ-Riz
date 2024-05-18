#include "hardware.h"
#include "clcd.h"

int main()
{	
	HW_Init();
  lcd_clear();
	lcd_puts("Salam");
  
while(1)
{
	uint16_t temp=HW_ADC_Read();
	lcd_set_cursor(0,1);
	lcd_printf("Temp is %u",temp);
	
	Delay_RIT_ms(2500);
}
	
}
