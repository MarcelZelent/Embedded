/*
 * Ass.8.c
 *
 * Created: 10/28/2020 2:53:16 PM
 * Author : Marcel
 */ 

#define F_CPU 16000000UL

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"

void swap(int*, int*);
int num1, num2;
int array[10];
int swaps;

int main(void)
{
		uart_init();
		io_redirect();
		
   while(1)
    {
		printf("Put in the numbers\n");
		for(int i=0; i<10; i++)
		{
			scanf("%d", &array[i]);
		}

		do 
		{
			swaps=0;
			for(int i=0; i<9; i++)
			{
				if (array[i]>array[i+1])
				{
					swap(&array[i],&array[i+1]);
					swaps++;
				}
			}
			for(int p=0; p<10; p++)
				printf("%d", array[p]);
		printf("\n");
		} while (swaps!=0);
	}
}

swap(int *small, int *large)
{
	int tmp;

		tmp = *small;
		*small = *large;
		*large = tmp;


}
