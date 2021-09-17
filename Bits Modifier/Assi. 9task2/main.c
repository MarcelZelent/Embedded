/*
 * Assi. 9task2.c
 *
 * Created: 11/6/2020 7:51:54 PM
 * Author : Marcel
 */ 

#define F_CPU 16000000UL

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"

int get_bit(int, int);
int set_bit(int, int);
int clear_bit(int, int);

void main(void){
	
	uart_init();
	io_redirect();

	char number, setbit, clearbit;
	int bitsarr[8];
	
	printf ("Hey write the first number, the bit you want to set and the bit you want to clear\n");
	
	
	number = 0;
	scanf("%d %d %d", &number, &setbit, &clearbit);
	
	for(int i; i<8; i++)
		bitsarr[i] = get_bit(number, i);		//puts the number into an array
		
	for(int i=7; i>=0; i--)
	printf("%d", bitsarr[i]);
	
	printf("\n");
			
	number = set_bit(number, setbit);		//sets the desired bit
	for(int i; i<8; i++)
	bitsarr[i] = get_bit(number, i);
	
	for(int i=7; i>=0; i--)
	printf("%d", bitsarr[i]);
	
	printf("\n");

	number = clear_bit(number, clearbit);			//clears the desired bit 
	for(int i; i<8; i++)
	bitsarr[i] = get_bit(number, i);
	
	for(int i=7; i>=0; i--)
	printf("%d", bitsarr[i]);
	
	printf("\n");

}

int get_bit(int number1, int bit1)
{
	int mask = 0b00000001 << bit1;
	
	if((mask & number1) == mask)
		return(1);
	else
		return(0);
}

int set_bit(int number1, int bit1)
{
	number1 = number1 | (0b00000001<<bit1);
	return number1;
}

int clear_bit(int number1, int bit1)
{
	number1 = number1 & ~(1<<bit1);
	return(number1);
}