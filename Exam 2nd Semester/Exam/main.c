
#define F_CPU 16000000UL
#define BAUDRATE 19200
#define BAUD_PRESCALER ((F_CPU/(16UL*BAUDRATE))-1)
#define ADC_PIN0

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include "usart.h"
#include <avr/eeprom.h>
#include "ds1621.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include "i2cmaster.h"
#include <util/delay.h>

void init_interrupt(void);
int sensor(int);
void USART_init();
void USART_send(unsigned char);

uint16_t adc_result;
uint16_t adc_read(uint8_t);

uint8_t adrindex = 20;
uint16_t adrsuccess = 0;

int result;
int index[5];

volatile unsigned int ms_counter;
unsigned int limb;
volatile int twosec=0;
int success=0;

int main(void)
{
	i2c_init();
	LCD_init();
	init_interrupt();
	USART_init();
	
	DDRC = 0xF0; 
	PORTC = 0x00;
	
	ADMUX = (1<<REFS0); // set the reference voltage as the AVcc - 5V of the Arduino
	ADCSRA = (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN); // set the prescaler to 128 and turn on the ADC module
	
	srand((unsigned int) limb);
	
	PORTB =(0<<PORTB5);
	success=eeprom_read_byte((uint16_t *) adrsuccess);
	if(success==1) PORTB =(1<<PORTB5);
	
	int i=0;
	
    while (1) 
    {
		LCD_set_cursor(0,0);
		printf("%d", limb);
		if(twosec==1)
		{
			sensor(limb);
			LCD_set_cursor(0,2);
			printf("Result %d", result);
			if(result==1)
			{
				if(i<5)
				{
					index[i]=limb;
					LCD_set_cursor(i,3);
					printf("%d", index[i]);
					i++;
				}
				if(i==4)
				{
					success=1;
					eeprom_write_byte((uint16_t *) adrsuccess, success);
					  for (int c = 0; c < 5; c++)
					  {
						  eeprom_write_byte((uint8_t *) adrindex+c*10, index[c]);
					  }

				}
			}
			else
			{
				for(i;i>=0;i--)
				{
					index[i]=0;
					i=0;
					success=0;
				}
			}
		twosec=0;
		}
		
    }
}

void init_interrupt(void)
{
	TCCR1A |= (1<<WGM11);
	OCR1A = 0xF9;
	TCCR1B |= (1<<CS01)|(1<<CS00);
	TIMSK1 |= (1<<OCIE1A);
	sei();
	
}

ISR (TIMER1_COMPA_vect) //1000 means 2 seconds in real life
{
	ms_counter++;
	if (ms_counter==1000)
	{
		limb=(rand() % 4);
		ms_counter=0;
		twosec=1;
	}
}

uint16_t adc_read(uint8_t adc_channel)
{
	ADMUX &= 0xF0; //clear the channel that was previously used
	ADMUX |= adc_channel; // set the desired channel
	ADCSRA |= (1<<ADSC); //start the conversion by setting a flag into the register
	while ( (ADCSRA &  (1<<ADSC))); // wait until the conversion is done
	return ADC;
}

int sensor(int nlimb)
{
		result=0; //0=>unsuccesful
		adc_result = adc_read(nlimb); //read the result of conversion
		int voltage_base = (5000.0/1024); //this is the value of each interval
		adc_result = adc_result*voltage_base; //multiply the number of level by the interval
		if (adc_result > 1000) result=1;
		LCD_set_cursor(0,1);
		printf("Voltage %d", adc_result);
return result;
}

void USART_init()
{
	//setting up the prescaler
	UBRR0H = (uint8_t)(BAUD_PRESCALER>>8); //high byte
	UBRR0L = (uint8_t)(BAUD_PRESCALER); //low byte
	UCSR0B = (1<<RXEN0)|(1<<TXEN0); //enable RX and TX channels - sending and receiving data
	UCSR0C = ((1<<UCSZ00)|(1<<UCSZ01)); //setting the connection so that there's 1 stop bit, 8 bit data, no parity (the standard)
	UCSR0B|= (1<<RXCIE0); //enable interrupts for RXIE
}

void USART_send(unsigned char data)
{
	while(!(UCSR0A & (1<<UDRE0))); //basically you need to send the data one bit at a time. This while loop waits until the port says it is ready to receive the next bit (wait for transmit buffer)
	UDR0 = data; //puts our new data into the tunnel (data to be sent)
}

ISR (USART_RX_vect)
{
	volatile unsigned char received_data = UDR0;
	if (received_data == 1) USART_send(success);
	if (received_data == 2)
	{
		if(success==1)
		{
		 for (int c = 0; c < 5; c++)
		 {
			USART_send(eeprom_read_byte((uint8_t *) adrindex+c*10));
		 }
		}
		else
		{
			for (int k = 0; k < 5; k++)
			{
				USART_send(255);
			}
		}
	}
}