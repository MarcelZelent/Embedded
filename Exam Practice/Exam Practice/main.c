#define F_CPU 16000000UL
#define BAUDRATE 57600
#define BAUD_PRESCALER ((F_CPU/(16UL*BAUDRATE))-1)

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include "usart.h"
#include <avr/eeprom.h>
#include "ds1621.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "i2cmaster.h"
#include <util/delay.h>

void init_interrupt(void);
void USART_init(void);
unsigned char usart_receive(void);
void USART_send(unsigned char);

volatile unsigned int ms_counter=0;
volatile unsigned char locked;

unsigned int key;
uint8_t adrkey = 0;

int curr_temp, max_temp, min_temp;

volatile uint8_t pass;

int main(void)
{
	i2c_init();
	ds1621_init();
	USART_init();
	
	DDRB |= (1<<PORTB5);
	DDRD = 0xFF;
	DDRC = 0xF0;
	PORTC = 0x3F;
	PORTD = 0x00;
		
	curr_temp = get_temperature();
	max_temp=curr_temp;
	min_temp=curr_temp;
		
	init_interrupt();
	
	if(PINC == 0b00111110)
	{
		 USART_send(100);
		 key=usart_receive(); //this is blocking the code
		 eeprom_write_byte((uint8_t *)adrkey, key);
		 USART_send(key);
	}
	
	locked=1;
	pass = eeprom_read_byte((uint8_t *)adrkey);
	
	UCSR0B |= (1<<RXCIE0); //enable interrupts for RXIE
	while(1)
	{	
				if (locked==0)
				{
					PORTD = 0x00;
				}
				else{
					PORTD = 0xF0;
				}
				curr_temp=get_temperature();
				if(curr_temp>max_temp)	max_temp=curr_temp;
				if(curr_temp<min_temp)	min_temp=curr_temp;
			
	}
return(0);
}

void init_interrupt(void)
{
	TCCR2A |= (1<<WGM21);
	OCR2A = 0xF9;
	TCCR2B |= (1<<CS22);
	TIMSK2 |= (1<<OCIE2A);
	sei();
	
}

void USART_init()
{
	//setting up the prescaler
	UBRR0H = (uint8_t)(BAUD_PRESCALER>>8); //high byte
	UBRR0L = (uint8_t)(BAUD_PRESCALER); //low byte
	UCSR0B = (1<<RXEN0)|(1<<TXEN0); //enable RX and TX channels - sending and receiving data
	UCSR0C = ((1<<UCSZ00)|(1<<UCSZ01)); //setting the connection so that there's 1 stop bit, 8 bit data, no parity (the standard)
}

void USART_send(unsigned char data)
{
	while(!(UCSR0A & (1<<UDRE0))); //basically you need to send the data one bit at a time. This while loop waits until the port says it is ready to receive the next bit (wait for transmit buffer)
	UDR0 = data; //puts our new data into the tunnel (data to be sent)
}

unsigned char usart_receive(void)
{
	while(!(UCSR0A & (1<<RXC0))); //wait for new data
	return UDR0; //received data
}

ISR (TIMER2_COMPA_vect)
{
	ms_counter++;
	if (ms_counter==1000)
	{
		PORTB ^=(1<<PORTB5);
		ms_counter=0;
	}
}

ISR (USART_RX_vect) //this is for sending only
{
	volatile unsigned char received_data = UDR0;
	if (locked == 0)
	{
		if (received_data == 0) USART_send((uint8_t *)curr_temp);
		if (received_data == 1) USART_send((uint8_t *)max_temp);
		if (received_data == 2) USART_send((uint8_t *)min_temp);
	}
	else
	{
		if(received_data==pass)
		{
			locked=0;
			USART_send(0);
		}
		else
		{
			USART_send(255);
		}
	}
}
