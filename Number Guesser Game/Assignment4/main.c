/*
 * Assignment4.c
 *
 * Created: 9/25/2020 10:25:25 AM
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

	/*
	while (1)
	{
	int user1 = 0;
	int user2 = 0;
	int att = 0;
	printf("User 1: Type in a number between 0-1000\n");
	scanf("%d", &user1);
	while (user1<0 || user1>1000)
	{
		printf("Nice try, but the number has to be between 0-1000\n");
		scanf("%d", &user1);
	} 
	printf("User 2: Type in the predicted number\n");
	scanf("%d", &user2);
	att++;
		do 
		{
			while (user2<0 || user2>1000)
			{
				printf("Nice try, but the number has to be between 0-1000. Try again\n");
				scanf("%d", &user2);
				att++;
			}
			if(user1==user2)
			break;
			if(user1<user2)
			{
				printf("Try a lower number\n");
			}
			else 
			{
				printf("Try a higher number\n");
			}
			scanf("%d", &user2);
			att++;
			
		} while (user1!=user2);
	printf("Congrats! It only took you %d attempts\n", att);
	}


	int n;
	
	while(1)
	{
		unsigned long result = 1;
		printf("Type in the desired number\n");
		scanf("%d", &n);
		
		for(int i=1; i<n+1; i++)
		{
			result=result*i;
		}
	printf("The result is %lu\n", result);
	}

	*/
	int n = 0;
	while(1)
	{
	int i = 1;
	printf("Type in a number\n");
	scanf("%d", &n);
	
	for(i; i!=n; i++)
	{
		if (n%i==0)
		printf("%d, ", i);
	}
	if (i==n)
	{
		printf("%d\n", n);
	}
	}
	
	 return 0;
 }