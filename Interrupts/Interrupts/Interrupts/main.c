
#define F_CPU 16E6
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <stdlib.h>
#include "usart.h"
#include <avr/interrupt.h>

void init_interrupt();

//cli(); disable interrupts
//sei(); enable interrupts

volatile unsigned int ms_counter; //we need to use volatile so that the chip knows that this variable needs to change quickly and unexpectedly
void set_light(unsigned int);

/*int main(void)
{
	DDRD = 0xFF; 
	PORTD = 0x00; //lights
	
    init_interrupt();
    while (1) 
    {//do nothing
	}
return 0;
}

void init_interrupt(void)
{
	TCCR0A |= (1<<WGM01);
	OCR0A = 0xF9;
	//so far only setting up the timer
	TIMSK0 |= (1<<OCIE0A); //This tells the interrupt that the flag of the timer has been raised (a doorbell)
	sei(); //enable all interrupts
	TCCR0B |= (1<<CS01) | (1<<CS00); //start the timer
}

ISR (TIMER0_COMPA_vect)
{
	ms_counter++;
	if (ms_counter==5000)
	{
	PORTD ^= 0xF0; //toggle the leds (you need to use the ^ to get the toggle)
	ms_counter=0;
	}
}
//this is for toggling the leds after certain time
//this is for turning on the led periodically (6a)
*/
int main(void)
{
	DDRD = 0xFF;
	PORTD = 0x00; //lights
	DDRC = 0xF0;
	PORTC = 0x3F; //buttons
	
	init_interrupt();
	while (1)
	{//do nothing
		if (PINC == 0b00111110)
		{
			set_light(1000);
		}
	}
	return 0;
}

void init_interrupt(void)
{
	TCCR0A |= (1<<WGM01);
	OCR0A = 0xF9;
	//so far only setting up the timer
	TIMSK0 |= (1<<OCIE0A); //This tells the interrupt that the flag of the timer has been raised (a doorbell)
	sei(); //enable all interrupts
	TCCR0B |= (1<<CS01) | (1<<CS00); //start the timer
}

void set_light(unsigned int amount_of_ms)
{
	PORTD = 0xF0;
	ms_counter = amount_of_ms;
}

ISR (TIMER0_COMPA_vect)
{

	if (ms_counter==0)
	{
		PORTD = 0x00;
	}
	else ms_counter--;
}

//This is for turning the led when button is pressed (6b). Keep in mind that the longer you press the button, the longer the lights will be on; FIX=>use _ms_delay();

/*
int main(void)
{
	DDRD = 0xFF;
	PORTD = 0x00; //lights
	DDRC = 0xF0;
	PORTC = 0x3F; //buttons
	
	DDRD &= ~(1<<DDD2); //clear the pin PD2
	PORTD |= (1<<PORTD2); //set the pin PD2 as an external input
	
	init_interrupt();
	while (1)
	{//do nothing
	}
	return 0;
}

void init_interrupt(void)
{
	TCCR0A |= (1<<WGM01);
	OCR0A = 0xF9;
	//so far only setting up the timer
	TIMSK0 |= (1<<OCIE0A); //This tells the interrupt that the flag of the timer has been raised (a doorbell)
	EICRA |= (1<<ISC00); //any logic change
	EIMSK |= (1<<INT0); // subscribing to the event thats on the PD2 pin (this one has the INT0 - look at datasheet)
	sei(); //enable all interrupts
	TCCR0B |= (1<<CS01) | (1<<CS00); //start the timer
}

void set_light(unsigned int amount_of_ms) //this turns on the led and gets how long it should be pressed
{
	PORTD = 0xF0; 
	ms_counter = amount_of_ms;
}

ISR (TIMER0_COMPA_vect) //this turns off the led after the time runs out
{

	if (ms_counter==0)
	{
		PORTD = 0x00;
	}
	else ms_counter--;
}

ISR (INT0_vect)
{
	if (ms_counter==0) //this eliminates the problem with the led being turned on for 1 sec + however long the pin was on;  the led turns on only on the falling edges; only when it is not counting
	set_light(1000); 
}
//This turns on the led for 1 second when there is a logic change on the PD2 pin
*/