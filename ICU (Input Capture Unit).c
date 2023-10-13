#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/io.h>
#include "std_macros.h"
#include "LCD.h"

int main(void)
{
	unsigned short a,b,c;
	unsigned short high,period,frequency;
	unsigned char duty_cycle;
	LCD_init();
	TCCR1A=0;
	
	/* Clear the flag bit (Important) */
	SET_BIT(TIFR,ICF1);
	
	/* Detect first rising edge */
	TCCR1B=0xC1;
	while(READ_BIT(TIFR,ICF1) == 0);
	SET_BIT(TIFR,ICF1);
	a=ICR1;
	
	/* Detect first falling edge */
	TCCR1B=0x81;
	while(READ_BIT(TIFR,ICF1) == 0);
	SET_BIT(TIFR,ICF1);
	b=ICR1;
	
	/* Detect second rising edge */
	TCCR1B=0xC1;
	while(READ_BIT(TIFR,ICF1) == 0);
	SET_BIT(TIFR,ICF1);
	c=ICR1;
	
	/* Stop the timer (Not Important) */
	TCCR1B=0;
	TCNT1=0;
	
	/* Check for timer 1 overflow */
	if(a<b && b<c)
	{
		high = b-a;
		period = c-a;
		duty_cycle = ((float)high/period)*100;
		frequency = F_CPU/period;
		LCD_send_string("Duty Cycle:");
		LCD_send_char(duty_cycle/10+48);
		LCD_send_char(duty_cycle%10+48);
		LCD_send_char('%');
		LCD_move_cursor(2,1);
		LCD_send_string("Freq:");
		if(frequency >= 10000)
		{
			LCD_send_char(frequency/10000+48);
			LCD_send_char((frequency/1000)%10+48);
			LCD_send_char((frequency/100)%10+48);
			LCD_send_char((frequency/10)%10+48);
			LCD_send_char(frequency%10+48);
			LCD_send_string("Hz");
		}
		else if(frequency >= 1000)
		{
			LCD_send_char(frequency/1000+48);
			LCD_send_char((frequency/100)%10+48);
			LCD_send_char((frequency/10)%10+48);
			LCD_send_char(frequency%10+48);
			LCD_send_string("Hz");
		}
		else
		{
			LCD_send_char(frequency/100+48);
			LCD_send_char((frequency/10)%10+48);
			LCD_send_char(frequency%10+48);
			LCD_send_string("Hz");
		}
	}
	else
	{
		LCD_send_string("Frequency is");
		LCD_move_cursor(2,1);
		LCD_send_string("out of range");
	}
    while(1)
    {
		
    }
}