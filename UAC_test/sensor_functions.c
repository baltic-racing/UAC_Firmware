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


// Typ K temperature
uint16_t read_brake_temp(uint8_t Typ_K)
{
	volatile uint16_t brake_temp_msb = 0;
	volatile uint16_t brake_temp_lsb = 0;
	volatile uint16_t brake_temp = 0;
	volatile uint16_t brake_temp_normal = 0;
	volatile uint16_t brake_temp_Grad_C = 0;
	
	switch(Typ_K) {
		case 0: SS_Typ_K1_LOW(); break;
		case 1: SS_Typ_K2_LOW(); break;
	}

	brake_temp_msb = SPI_transfer(0x00);
	brake_temp_lsb = SPI_transfer(0x00);

	switch(Typ_K) {
		case 0: SS_Typ_K1_HIGH(); break;
		case 1: SS_Typ_K2_HIGH(); break;
	}
	
	brake_temp = (brake_temp_lsb | (brake_temp_msb << 8));	// Bit D0 bis D2 für Temperatur uninteressant
	brake_temp_normal = (brake_temp >> 3);						// shift sodass die 12 letzten Bits die Temperatur anzeigen

	brake_temp_Grad_C = brake_temp_normal * 25;					// 25 = 100/4 (*100 für 2 Dezimalstellen, /4 für auflösung)
	
	return brake_temp_Grad_C;
}