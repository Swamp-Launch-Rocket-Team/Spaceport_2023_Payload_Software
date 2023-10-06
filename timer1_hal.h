/*
 * timer1_hal.h
 *
 * Created: 10/5/2023 2:47:00 PM
 *  Author: Jordan
 */ 


#ifndef TIMER1_HAL_H_
#define TIMER1_HAL_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define PWM_TOP (39999u)
#define SEVRO_MIN (1999u)
#define SEVRO_MAX  (4999u)


void pwm_init(void);
void pwm_sweep(void);
void servo_set(uint16_t deg,uint16_t max_deg);

#endif /* TIMER1_HAL_H_ */