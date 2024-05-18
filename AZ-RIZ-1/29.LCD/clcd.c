#include "clcd.h"

#include <stdarg.h>
#include <stdio.h>

static void lcd_send(uint8_t value, uint8_t mode);
static void lcd_write_nibble(uint8_t nibble);

static uint8_t   lcd_displayparams;
static char      lcd_buffer[LCD_COL_COUNT + 1];

void lcd_command(uint8_t command)
{
  lcd_send(command, 0);
}

void lcd_write(uint8_t value)
{
  lcd_send(value, 1);
}

void lcd_send(uint8_t value, uint8_t mode)
{
  if (mode)
	{
		GPIO_SetValue(LCD_PORT,(1 << LCD_RS));    
  }
	else
	{
		GPIO_ClearValue(LCD_PORT,(1 << LCD_RS));    
  }
	
	GPIO_ClearValue(LCD_PORT,(1 << LCD_RW));

  lcd_write_nibble(value >> 4);
  lcd_write_nibble(value);
}

void lcd_write_nibble(uint8_t nibble)
{
		if(( nibble >> 0) & 0x01)			
			GPIO_SetValue(LCD_PORT,(1<<LCD_D0));
		else
			GPIO_ClearValue(LCD_PORT,(1<<LCD_D0));
		
		if(( nibble >> 1) & 0x01)			
			GPIO_SetValue(LCD_PORT,(1<<LCD_D1));
		else
			GPIO_ClearValue(LCD_PORT,(1<<LCD_D1));
		
		if(( nibble >> 2) & 0x01)			
			GPIO_SetValue(LCD_PORT,(1<<LCD_D2));
		else
			GPIO_ClearValue(LCD_PORT,(1<<LCD_D2));
		
		if(( nibble >> 3) & 0x01)			
			GPIO_SetValue(LCD_PORT,(1<<LCD_D3));
		else
			GPIO_ClearValue(LCD_PORT,(1<<LCD_D3));
		

	GPIO_ClearValue(LCD_PORT,(1<<LCD_EN));
	lcd_delay_us(300);

	GPIO_SetValue(LCD_PORT,(1<<LCD_EN));
  lcd_delay_us(300);

  GPIO_ClearValue(LCD_PORT,(1<<LCD_EN));
  
  lcd_delay_us(300);
}

void lcd_init(void)
{
  // Configure pins as output
	GPIO_SetDir(LCD_PORT,(1<<LCD_RS),1);
	GPIO_SetDir(LCD_PORT,(1<<LCD_RW),1);
	GPIO_SetDir(LCD_PORT,(1<<LCD_EN),1);
	GPIO_SetDir(LCD_PORT,(1<<LCD_D0),1);
	GPIO_SetDir(LCD_PORT,(1<<LCD_D1),1);
	GPIO_SetDir(LCD_PORT,(1<<LCD_D2),1);
	GPIO_SetDir(LCD_PORT,(1<<LCD_D3),1);
  
  // Wait for LCD to become ready (docs say 15ms+)
  lcd_delay_ms(50);

	GPIO_ClearValue(LCD_PORT,(1<<LCD_EN));
	GPIO_ClearValue(LCD_PORT,(1<<LCD_RS));
	GPIO_ClearValue(LCD_PORT,(1<<LCD_RW));
  
  lcd_delay_ms(5);

  lcd_write_nibble(0x03); // Switch to 4 bit mode
  lcd_delay_ms(5);

  lcd_write_nibble(0x03); // 2nd time
  lcd_delay_ms(5);

  lcd_write_nibble(0x03); // 3rd time
  lcd_delay_ms(5);

  lcd_write_nibble(0x02); // Set 4-bit mode 
  lcd_delay_ms(5);
 
  lcd_command(LCD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS);
  lcd_delay_ms(5);
	
			
  lcd_displayparams = LCD_CURSOROFF | LCD_BLINKOFF | LCD_DISPLAYON;
  lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
	lcd_delay_ms(5);
	
	lcd_command(LCD_CLEARDISPLAY);
  lcd_delay_ms(2);
}

void lcd_on(void)
{
  lcd_displayparams |= LCD_DISPLAYON;
  lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

void lcd_off(void)
{
  lcd_displayparams &= ~LCD_DISPLAYON;
  lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

void lcd_clear(void)
{
  lcd_command(LCD_CLEARDISPLAY);
  lcd_delay_ms(2);
}

void lcd_return_home(void)
{
  lcd_command(LCD_RETURNHOME);
  lcd_delay_ms(2);
}

void lcd_enable_blinking(void)
{
  lcd_displayparams |= LCD_BLINKON;
  lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

void lcd_disable_blinking(void)
{
  lcd_displayparams &= ~LCD_BLINKON;
  lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

void lcd_enable_cursor(void)
{
  lcd_displayparams |= LCD_CURSORON;
  lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

void lcd_disable_cursor(void)
{
  lcd_displayparams &= ~LCD_CURSORON;
  lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

void lcd_scroll_left(void)\
{
  lcd_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void lcd_scroll_right(void)
{
  lcd_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void lcd_set_left_to_right(void)
{
  lcd_displayparams |= LCD_ENTRYLEFT;
  lcd_command(LCD_ENTRYMODESET | lcd_displayparams);
}

void lcd_set_right_to_left(void)
{
  lcd_displayparams &= ~LCD_ENTRYLEFT;
  lcd_command(LCD_ENTRYMODESET | lcd_displayparams);
}

void lcd_enable_autoscroll(void)
{
  lcd_displayparams |= LCD_ENTRYSHIFTINCREMENT;
  lcd_command(LCD_ENTRYMODESET | lcd_displayparams);
}

void lcd_disable_autoscroll(void)
{
  lcd_displayparams &= ~LCD_ENTRYSHIFTINCREMENT;
  lcd_command(LCD_ENTRYMODESET | lcd_displayparams);
}

void lcd_create_char(uint8_t location, uint8_t *charmap)
{
  lcd_command(LCD_SETCGRAMADDR | ((location & 0x7) << 3));
  for (int i = 0; i < 8; i++) {
    lcd_write(charmap[i]);
  }
}

void lcd_set_cursor(uint8_t col, uint8_t row)
{
  static uint8_t offsets[] = { 0x00, 0x40, 0x14, 0x54 };

  if (row > 1) {
    row = 1;
  }

  lcd_command(LCD_SETDDRAMADDR | (col + offsets[row]));
}

void lcd_puts(char *string)
{
  for (char *it = string; *it; it++) {
    lcd_write(*it);
  }
}

void lcd_printf(char *format, ...)
{
  va_list args;

  va_start(args, format);
  vsnprintf(lcd_buffer, LCD_COL_COUNT + 1, format, args);
  va_end(args);

  lcd_puts(lcd_buffer);
}
