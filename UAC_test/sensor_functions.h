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

uint16_t read_brake_temp(uint8_t channel);

#endif