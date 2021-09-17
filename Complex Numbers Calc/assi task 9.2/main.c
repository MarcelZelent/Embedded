#define F_CPU 16000000UL

#include <stdlib.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "usart.h"

typedef struct{
	int real_part;
	int ima_part;
	}complex_t;

complex_t create_number(void);
void adding(complex_t, complex_t);
void product(complex_t, complex_t);
void display(complex_t, int state);

int main(void){
	
	uart_init();
	io_redirect();
	
	complex_t number[2];
	
		for (int i; i<2; i++)
		{
			number[i] = create_number();
			display(number[i], 1);
		}
	adding(number[0], number[1]);
	product(number[0], number[1]);
	
	
}

complex_t create_number(void){
	complex_t temp;
	
	printf("Write the real part\n");
	scanf("%d", &temp.real_part);
	
	printf("Write the imaginary part\n");
	scanf("%d", &temp.ima_part);
	
	return temp;
}

void adding(complex_t first, complex_t second){
	complex_t sum;
	sum.real_part = first.real_part + second.real_part;
	sum.ima_part = first.ima_part + second.ima_part;
	display(sum, 0);
}
 
void display(complex_t num, int state){
	if (state == 1)
	{
		if (num.ima_part>0)
		printf("The number: %d+%di\n", num.real_part, num.ima_part);
		else
		printf("The number: %d%di\n", num.real_part, num.ima_part);
	} 
	else
	{
		if (num.ima_part>0)
		printf("The result: %d+%di\n", num.real_part, num.ima_part);
		else
		printf("The result: %d%di\n", num.real_part, num.ima_part);
	}
	if (num.ima_part>0)
	printf("The number: %d+%di\n", num.real_part, num.ima_part);
	else
	printf("The number: %d%di\n", num.real_part, num.ima_part);
}

void product(complex_t first, complex_t second){
	complex_t prod;
	prod.real_part = first.real_part*second.real_part + first.ima_part*second.ima_part;
	prod.ima_part = first.ima_part*second.ima_part;
	display(prod, 0);
}