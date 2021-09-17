/*
 * First Try.c
 *
 * Created: 10/20/2020 7:10:32 PM
 * Author : Marcel
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "lcd.h"
#include "i2cmaster.h"

#include "i2cmaster.h"
#include "ds1621.h"

int main (void){
	i2c_init();
	LCD_init();
	ds1621_init();
	
	double temperature;
	double max;
	double min = 0;
	double tempfahr;
	
	temperature = get_temperature();
	max = temperature; min = temperature;
	
	
	DDRD = 0xFF;
	DDRC = 0xF0;
	PORTC = 0xF0;
	PORTD = 0x00;
	
	while (1){
		LCD_set_cursor(0,0);	
		PORTC = 0b00111111;
		temperature = get_temperature();
		tempfahr = temperature*1.8+32;

		if (temperature>max)
		{
			max = temperature;
		}
		else if (temperature<min)
		{
			min = temperature;
		}

		if (PINC == 0b00111110)
			{
				LCD_clear();
				printf("Temp = %.1f\337C", temperature);
				PORTD = 0b00010000;
				
			}
		if (PINC == 0b00111101)
			{
				LCD_clear();
				printf("Temp = %.1f\337F", tempfahr);
				PORTD = 0b00100000;
			}
		if (PINC == 0b00111011)
			{
				LCD_clear();
				LCD_set_cursor(0,0);
				printf("Min = %.1f\337C", min);
				LCD_set_cursor(0,1);
				printf("Min = %.1f\337F", (min*1.8+32));
				PORTD = 0b01000000;
			}
		if (PINC == 0b00110111)
			{
				LCD_clear();
				LCD_set_cursor(0,0);
				printf("Max = %.1f\337C", max);
				LCD_set_cursor(0,1);
				printf("Max = %.1f\337F", (max*1.8+32));
				PORTD = 0b10000000;
			}

	}
}
