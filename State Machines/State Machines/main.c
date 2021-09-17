/*
 * State Machines.c
 *
 * Created: 06-03-2021 16:19:22
 * Author : Marcel
 */ 
/* equations
q0_next=(!q1&&x)||(q0&&!x)||q2
q1_next=(!q2&&q0&&x)||(q1&&!x)||(q2&&q1)
q2_next=(q2&&!q0)||(!q0&&!x&&y)
z1=!q0||!q1||q2
z2=(q2&&q1)||(q2&&!q0)
*/

#define F_CPU 16000000UL
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"

unsigned char x, y, z1, z2; // inputs and outputs
unsigned char q0, q1, q2, q0_next, q1_next, q2_next;

void read_xy_values(void); //checking which button is pressed
void show_output(void); //showing current state + state variables on the screen
void state_transition(void); //advancing to the new state by implementing transition equations

int main(void)
{
	uart_init();
	io_redirect();
	
	q0 = 0;
	q1 = 0;
	q2 = 0;
	q0_next = 0;
	q1_next = 0;
	q2_next = 0;
	x = 0;
	y = 0;
	z1 = 0;
	z2 = 0;
	
	DDRD = 0xFF;
	DDRC = 0xF0;
	PORTC = 0x3F;
	PORTD = 0x00;
	
	while(1)
	{
		read_xy_values();
		state_transition();
		show_output();
		_delay_ms(1000);
	}
}

void read_xy_values(void)
{
	/* better way
	x=(PORTC&(1<<2))==0
	y=(PORTC&(1<<3))==0
	
	x=!(PORTC&(1<<2)) the chad way
	*/
	
	if(PINC == 0b00111011)
	{
		x=1;
		PORTD = 0b01000000;
	} else {
		x=0;
		PORTD = 0b00000000;
	 }
	

	if(PINC == 0b00110111)
	{
		y=1;
		PORTD = 0b10000000;
	} else{
		y=0;
		PORTD = 0b00000000;
	}
	if (PINC == 0b00110011)
	{
		x=1;
		y=1;
		PORTD = 0b11000000;
	}
}

void show_output(void)
{
	printf("x=%d, y=%d q2q1q0=%d%d%d state %c\n", x, y, q2,q1,q0, 65+q2*4+q1*2+q0);
}

void state_transition(void)
{
	q0_next=(!q1&&x)||(q0&&!x)||q2;
	q1_next=(!q2&&q0&&x)||(q1&&!x)||(q2&&q1);
	q2_next=(q2&&!q0)||(!q0&&!x&&y);
	z1=!q0||!q1||q2;
	z2=(q2&&q1)||(q2&&!q0);
	
	q0=q0_next;
	q1=q1_next;
	q2=q2_next;
}