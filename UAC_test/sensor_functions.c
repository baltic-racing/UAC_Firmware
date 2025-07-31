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


float beta = 3450.0f;				// optimized between 0-100°C
float R_25C = 10000.0f;				// restistance at 25°C
float T_25C = 298.15f;
float VCC = 5.0f;
float ADC_max = 1023.0f;
float R_i = 3300.0f;



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

	high_byte = SPI_transfer(0x00);
	low_byte  = SPI_transfer(0x00);

	switch(channel) {
		case TK1: SS_TK1_HIGH(); break;
		case TK2: SS_TK2_HIGH(); break;
	}

	raw_value = (high_byte << 8) | low_byte;
	temperature = ((raw_value >> 3) & 0x7FF) * 0.25;

	return temperature * 100;
}