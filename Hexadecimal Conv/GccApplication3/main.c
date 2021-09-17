/*
 * GccApplication3.c
 *
 * Created: 9/17/2020 10:57:25 PM
 * Author : Marcel
 */ 

 #define F_CPU 16000000UL

 #include <stdio.h>
 #include <avr/io.h>
 #include <util/delay.h>
 #include "usart.h"

 int main(void) {

	 uart_init(); // open the communication to the microcontroller
	 io_redirect(); // redirect input and output to the uart
	 
	 int input = 0;

	 while(1) {
		 
		 printf("Input a number\n");
		 scanf("%d" "%x", &input);
		 printf("Decimal %d, Hexa %X\n", input);
		 
	 }
	 
	 return 0;
 }
