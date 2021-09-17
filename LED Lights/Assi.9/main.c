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
	
	DDRD = 0xFF;
	DDRC = 0xF0;
	PORTC = 0x0F;
	
	int last_bit = 0;	
	
	PORTD = 0b00000000;

	
	while(1){
		
		if (PINC == 0b00111110)
			{
			last_bit = 1;
			PORTD = 0b10000000;
			}
		
		if (PINC == 0b00111101){
			last_bit = 2;
			PORTD = 0b10000000;}
		
		if (PINC == 0b00111011){
			last_bit = 3;
			PORTD = 0b10000000;}
		
		if (PINC == 0b00110111){
			last_bit = 4;
			PORTD = 0b10000000;
		}
		
		if (last_bit == 1)
			_delay_ms(500);
			
		if (last_bit == 2)
			_delay_ms(1000);
			
		if (last_bit == 3)
			_delay_ms(1500);
			
		if (last_bit == 4)
			_delay_ms(2000);
			
		if (last_bit==0)
			PORTD = 0x00;
			
		PORTD = PORTD>>1;
		
		if (PORTD == 0b00001000)
			PORTD = 0b10000000;
	LCD_set_cursor(0,0);
	printf("%d", last_bit);
	}
}