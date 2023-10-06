/*
 * servo check.c
 *
 * Created: 10/5/2023 2:45:07 PM
 * Author : Jordan
 */ 

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "uart_hal.h"
#include "timer1_hal.h"
#include "timer0_hal.h"



int main(void)
{
	uart_init(9600,0);

	pwm_init();
	sei();
	
	
	servo_set(0,180);
	int16_t i = 0;
	while (1)
	{
		
		for(i = 0; i <= 180; i++){
			servo_set(i,180);
			_delay_ms(40);
		}
		for(i = 180; i >= 0; i--){
			servo_set(i,180);
			_delay_ms(40);
		}
		
		

	}
}
