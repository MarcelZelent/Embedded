
#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include <util/delay.h>
#include "usart.h"
#define ADC_PIN0 0 //The channel we will use

uint16_t adc_result;

uint16_t adc_read(uint8_t);

int main(void)
{
	uart_init();
	io_redirect();
	
	DDRC = 0xF0; //PC0-PC3 are inputs of ADC
	PORTC = 0x00; //make sure no pullups - make my own connections
	DDRB = 0b00100000; // or 0x20 //PB5 is an output
	
	ADMUX = (1<<REFS0); // set the reference voltage as the AVcc - 5V of the Arduino
	ADCSRA = (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN); // set the prescaler to 128 and turn on the ADC module
	
	int alarm = 0;
	
    while (1) 
    {
		for (uint8_t channel = 0; channel<4; channel++)
		{
			adc_result = adc_read(channel); //read the result of conversion
			int voltage_base = (5000.0/1024); //this is the value of each interval
			adc_result = adc_result*voltage_base; //multiply the number of level by the interval
			printf("Channel:%d - %d\n", channel, adc_result);
			if (adc_result > 1000) alarm = 1;
		}
		if (alarm == 1)
			{
				printf("ALARM ON\n");
				PORTB |= (1<<PORTB5);
				alarm = 0;
			}
		else
		{
			PORTB &= ~(1<<PORTB5);
		}
	_delay_ms(1000);
	printf("\n");
	}
return 0;
}

uint16_t adc_read(uint8_t adc_channel)
{
	ADMUX &= 0xF0; //clear the channel that was previously used
	ADMUX |= adc_channel; // set the desired channel
	ADCSRA |= (1<<ADSC); //start the conversion by setting a flag into the register
	while ( (ADCSRA &  (1<<ADSC))); // wait until the conversion is done
return ADC;
}
