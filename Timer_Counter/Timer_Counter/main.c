/*
 * Timer_Counter.c
 *
 * Created: 19-03-2021 21:23:11
 * Author : Marcel
 */ 

#define F_CPU 16E6
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/eeprom.h>

#include "i2cmaster.h"
#include "ds1621.h"
#include "usart.h"

void delay_ms (unsigned int miliseconds);//miliseconds
void delay_hs (unsigned int hundred_miliseconds);//one hundred of miliseconds 

int main(void)
{	
	DDRD = 0xFF;//all outputs
	PORTD = 0x00;//all LEDS off
	
	TCCR0A |= (1<<WGM01); //CTC MODE
	OCR0A = 249; //desired number of "counters" state is 249, which with 64 prescaler equals 0.001 seconds (1ms)
	
    while (1) 
    {

		PORTD = 0xFF; //turn on all LEDS
		delay_ms(500);
		PORTD = 0x00; //turn off all LEDS
		delay_ms(500);
    }
	
return 0;
}

void delay_ms (unsigned int ms)
{
	TCCR0B = (1<<CS01)|(1<<CS00); //this gives us a prescaler of 64 and also starts the timer
	
	for (int i=0; i<ms; i++) //wait for 1ms "ms" times
	{
		while ((TIFR0 & (1<<OCF0A)) == 0) //this waits for the timer to hit the desired state (the flag is low)
		{
		}
	//now the desired time has passed
	TIFR0 = (1<<OCF0A); //resets the flag (need to use this, it is read-only)
	}

	
}

void delay_hs (unsigned int hs)
{
	for (int i=0; i<hs; i++)
	{
		delay_ms(100);
	}
}