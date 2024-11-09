/*
 * adc_functions.h
 *
 * Created: 06.04.2019 10:58:21
 *  Author: Ole Hannemann
 */ 


#ifndef ADC_FUNCTIONS_H_
#define ADC_FUNCTIONS_H_

#include <avr/io.h>
#include <avr/interrupt.h>

//function to config the adc interface
void adc_config();
//function thats restarting the next conversion
void adc_start_conversion();
//getter for adc vars
uint16_t adc_get_1();

ISR(ADC_vect);

#endif /* ADC_FUNCTIONS_H_ */