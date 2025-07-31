/*
 * sensor_functions.h
 *
 * Created: 17.07.2025 11:16:01
 *  Author: Egquus
 */ 

#ifndef SENSOR_FUNCTIONS_H_
#define SENSOR_FUNCTIONS_H_

/*	INCLUDES	*/

#include <avr/io.h>
#include <avr/interrupt.h>

typedef enum {
	TK1,
	TK2,
} TK_Channel;

uint16_t temp_calc(double ntc_adc);
float read_max6675(void);
float read_TK_temperature(TK_Channel channel);

#endif