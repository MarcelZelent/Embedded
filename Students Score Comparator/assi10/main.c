

#define F_CPU 16000000UL

#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"

typedef	struct
{
	char name[10];
	int age;
	int score
	}student_t;


int main(void)
{
	uart_init();
	io_redirect();
	

	srand(0);
	char name[10];
	int age;
	int score;
	int av_score=0;
	int av_age=0;
	
 
		for(int i; i<7; i++){
		printf("Give the student's name and age\n");
		scanf("%s %d", name, &age);
		score = rand()%100;
		av_score = av_score + score;
		av_age = av_age + age;
		student_t student1;
		student1.name[10] = name;
		student1.age = age;
		student1.score = score;

		}

	printf("The average age is %d and the average score is %d", av_age/7, av_score/7);

	return 0;
}

