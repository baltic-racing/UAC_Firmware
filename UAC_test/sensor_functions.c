/*
 * sensor_functions.c
 *
 * Created: 17.07.2025 11:14:52
 *  Author: Egquus
 */ 


#include "sensor_functions.h"
#include "SPI_lib_light.h"
#include <avr/io.h>
#include <math.h>
#include <util/delay.h>

// Typ K temperature
float read_TK_temperature(TK_Channel channel) {
	uint8_t high_byte = 0;
	uint8_t low_byte = 0;
	uint16_t raw_value = 0;
	float temperature = 0.0;

	switch(channel) {
		case TK1: SS_TK1_LOW(); break;
		case TK2: SS_TK2_LOW(); break;
	}


	switch(channel) {
		case TK1: SS_TK1_HIGH(); break;
		case TK2: SS_TK2_HIGH(); break;
	}

	raw_value = (high_byte << 8) | low_byte;
	temperature = ((raw_value >> 3) & 0x7FF) * 0.25;

	return temperature * 100;
}