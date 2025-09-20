/*
 * main.h
 *
 * Created: 09.11.2024 16:50:09
 *  Author: Egquus
 */ 

#ifndef MAIN_H_
#define MAIN_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc_functions.h"
#include "misc_Functions.h"
#include "SPI_lib_light.h"
#include "canlib.h"
#include "CAN_data.h"
#include "sensor_functions.h"

//	Variables
extern volatile unsigned long sys_time;

volatile unsigned long time_1ms = 0;
volatile unsigned long time_10ms = 0;
volatile unsigned long time_100ms = 0;
volatile unsigned long time_200ms = 0;
volatile unsigned long time_1000ms = 0;

volatile uint16_t brake_disc_temp_msb = 0;
volatile uint16_t brake_disc_temp_lsb = 0;
volatile uint16_t brake_disc_temp = 0;
volatile uint16_t brake_disc_temp_normal = 0;
volatile uint16_t brake_disc_temp_Grad_C = 0;

volatile uint8_t thermo_open = 0;

//	Macros

#define TIME_PASSED_1_MS	(sys_time - time_1ms) >= 1
#define TIME_PASSED_10_MS	(sys_time - time_10ms) >= 10
#define TIME_PASSED_100_MS	(sys_time - time_100ms) >= 100
#define TIME_PASSED_200_MS	(sys_time - time_200ms) >= 200
#define TIME_PASSED_1000_MS	(sys_time - time_1000ms) >= 1000

#endif /* MAIN_H_ */