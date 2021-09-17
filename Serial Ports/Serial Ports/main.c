
#define F_CPU 16000000UL
#define BAUDRATE 57600
#define BAUD_PRESCALER ((F_CPU/(16UL*BAUDRATE))-1)

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void USART_init();
void USART_send(unsigned char);

int main(void)
{ 
	DDRC = 0xF0; //pc0-3 inputs
	PORTC = 0x3F;// pullups
	  
	USART_init();  
	  
    while (1) 
    {
		if(PINC == 0b00111110) //button 1 is pressed - send value 1
			{
				USART_send(1);
			}
		if(PINC == 0b00111101) //button 2 is pressed - send value 2
			{
				USART_send(2);
			}
		if(PINC == 0b00111011) //button 3 is pressed - send value 3
			{
				USART_send(3);
			}
		if(PINC == 0b00110111) //button 4 is pressed - send value 4
			{
				USART_send(4);
			}
		_delay_ms(300);
    }
return 0;
}
//This is the a part - sending a message over the serial port

int main(void)
{
	DDRC = 0xF0; //pc0-3 inputs
	PORTC = 0x3F;// pullups
	DDRD = 0xFF;
	PORTD = 0x00; //LEDS
	
	USART_init();
	UCSR0B|= (1<<RXCIE0); //enable interrupts for RXIE
	sei(); //turn on interrupts
	
	while (1)
	{
	// do nothing
	}
	return 0;
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

ISR (USART_RX_vect)
{
	volatile unsigned char received_data = UDR0;
	volatile unsigned char received_data_2 = UDR0; // this will actually be equal to 0, because the buffer is empty already
	if (received_data == 1) PORTD ^=0b00010000;
	if (received_data == 2) PORTD ^=0b00100000;
	if (received_data == 3) PORTD ^=0b01000000;
	if (received_data == 4) PORTD ^=0b10000000;
}