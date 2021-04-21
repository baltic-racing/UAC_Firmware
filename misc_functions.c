/*
 * misc_functions.c
 *
 * Created: 05.04.2019 18:51:39
 *  Author: Ole Hannemann
 * last change: 28.02.2021
 *  Author: Lukas Deeken
 */ 
#include "misc_functions.h"
volatile unsigned long sys_time = 0;

void port_config(){
	DDRA = 0 | (1<<PA0) | (1<<PA1);							//Heart & Error
	DDRB = 0 | (1<<PB6);									//SPI, Setup is done in SPI.C & 3,3V Enbale	
	DDRC = 0;												//empty
	DDRD = 0;												//Can so all Input Setup is done in CANlib.C
	DDRE = 0;												//empty
	DDRF = 0;												//mark analog inputs on Port F as input		
	PORTB = 0| (1<<PB6);									//switch 3,3V enable off as we currently use nothing with 3,3V																		
}

void sys_tick(){
	PORTA ^= 1<<PA1; //toggling the Status of PA1 to make the Heart LED blink every SYStick
}

void sys_timer_config(){
	//8 bit Timer 0 configuration
	//ctc mode and 64 as prescaler for 16Mhz
	TCCR0A = 0 | (1<<WGM01) | (1<<CS01) | (1<<CS00);
	TIMSK0 = 0 | (1<<OCF0A);	//compare interrupt enable
	OCR0A = 250-1;				// compare value for 1ms;
}

//ISR for Timer 0 compare interrupt
ISR(TIMER0_COMP_vect){
	sys_time++; //system time generation
	//for every time the timer equals 249 an interrupt is generated resulting in increasing the SYStime
}
