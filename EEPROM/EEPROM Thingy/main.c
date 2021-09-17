/*
 * EEPROM Thingy.c
 *
 * Created: 12-03-2021 21:45:34
 * Author : Marcel
 */ 

#define F_CPU 16e6
#include <avr/io.h>
#include <avr/eeprom.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

#include "i2cmaster.h"
#include "ds1621.h"
#include "usart.h"

int main(void)
{
	DDRD = 0xFF;
	DDRC = 0xF0;
	PORTC = 0x3f;
	PORTD = 0x00;
	
	i2c_init();
	ds1621_init();
	uart_init();
	io_redirect();
	
	float current_temp, maxtemp, mintemp;
	
	uint16_t *addrmax = malloc (1);
	*addrmax = 0;
	uint16_t *addrmin = malloc (1);
	*addrmin = 1;
	
	maxtemp = eeprom_read_float((float *)addrmax);
	mintemp = eeprom_read_float((float *)addrmin);
	
    while (1) 
    {
		current_temp = get_temperature();
		if (current_temp>maxtemp){
			maxtemp = current_temp;
			eeprom_write_float((float *)addrmax, maxtemp);
			printf("MAX saved\n");
		}
		else if (current_temp<mintemp)
		{
			mintemp = current_temp;
			eeprom_write_float((float *)addrmin, mintemp);
			printf("MIN saved\n");
		}
		printf("Current %.1f, max is %.1f, min is %.1f\n", current_temp, maxtemp, mintemp);
		_delay_ms(1000);
		
		if (PINC == 0b00110111)
		{
			printf("Max and Min DESTROYED\n");
			maxtemp = current_temp;
			eeprom_write_float((float *)addrmax, maxtemp);
			mintemp = current_temp;
			eeprom_write_float((float *)addrmin, mintemp);
			printf("Max and Min SAVED\n");
		}
    }
}

