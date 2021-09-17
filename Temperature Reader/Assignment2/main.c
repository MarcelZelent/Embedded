/*
 * Assignment2.c
 *
 * Created: 9/18/2020 10:31:33 AM
 * Author : Marcel
 */ 

 #define F_CPU 16000000UL

 #include <stdio.h>
 #include <avr/io.h>
 #include <util/delay.h>
 #include "usart.h"

 int main(void) 
 {

	 uart_init();
	 io_redirect(); 
	 
	 int input = 0;
	 unsigned char temp;
	 double tempf;
	 /*
	 {
		 
		 printf("Input a number in decimal\n");
		 scanf("%d", &input);
		 printf("You wrote %x in hexadecimal\n", input);
		 
		 printf("Input a number in hexadecimal\n");
		 scanf("%x", &input);
		 printf("You wrote %d in decimal\n", input);
	 }*/
	 {
		 _delay_ms(1000);
		 printf("OK now it's time for some temperature conversion \n");
		 _delay_ms(1000);
		 printf("What's the temperature in Celsius\n");
		 scanf("%d", &temp);
		 temp = temp*9/5+32;
		 tempf = temp;
		 printf("Here's your temperature in Fahrenheit:\n %d\n", temp);
		 printf("The result in different formats:\n d=%d\n u=%u\n f=%f\n e=%e\n c=%c\n o=%o\n x=%x\n",temp,temp,tempf,tempf,temp,temp,temp);
	 }
		 
	 
	 return 0;
 }

