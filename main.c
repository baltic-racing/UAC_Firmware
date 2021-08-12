/*
 * uFAR.c
 *
 * Created: 04.04.2019 20:52:02
 * Author : Ole Hannemann
 */ 

#include <avr/io.h>
#include "canlib.h"
#include "misc_functions.h"
#include "adc_functions.h"
#include "SPI.h"
#include <avr/interrupt.h>

extern unsigned long sys_time;
volatile unsigned long time_old = 0;
volatile uint8_t sys_time_10 = 0;
volatile uint8_t sys_time_50 = 0;
volatile uint8_t sys_time_500 = 0;

int main(void)
{
	can_cfg();
	sys_timer_config();
	port_config();
	adc_config();
	SPI_config();
	
	// Struct for the CAN Message that contains the sampled Sensor Data
	// UAC-FL 0x700 0x701
	// UAC-FR 0x702 0x703
	// UAC-RL 0x704 0x705
	// UAC-RR 0x706 0x707
	
	struct CAN_MOB can_SH_mob1;
	can_SH_mob1.mob_id = 0x702;
	can_SH_mob1.mob_idmask = 0; //We are sending this CAN Message Object (MOB) therefore we do not need an ID MASK
	can_SH_mob1.mob_number = 1;
	uint8_t SH_databytes1[2];
	
	struct CAN_MOB can_SH_mob2;
	can_SH_mob1.mob_id = 0x703;
	can_SH_mob1.mob_idmask = 0; //We are sending this CAN Message Object (MOB) therefore we do not need an ID MASK
	can_SH_mob1.mob_number = 2;
	uint8_t SH_databytes2[4];
	
	sei();
	
    while (1) {
		//1ms loop
		if((sys_time - time_old) >= 1){ 
			time_old = sys_time;
			//adc_start_conversion();
			sys_time_10++;
				
		//10ms loop
		if (sys_time_10 >= 10){  
			
			SH_databytes1[0] = adc_get_1() & 0xff;
			SH_databytes1[1] = adc_get_1() >> 8;
			can_tx(&can_SH_mob1, SH_databytes1); //send the CAN Message	100Hz	

			sys_time_10 = 0;
			sys_time_50++;
			}
		}
		
		//50ms loop
		if(sys_time_50 >= 5){
			
			sys_time_50 = 0;
			sys_time_500++;
			}
			
		//500ms loop	
		if(sys_time_500 >= 20){
			sys_tick();
			TYPK_read();
			SH_databytes2[0] = TYPK_getdata1()& 0xff;
			SH_databytes2[1] = TYPK_getdata1()>> 8;
			SH_databytes2[2] = TYPK_getdata2() & 0xff;
			SH_databytes2[3] = TYPK_getdata2()>> 8;
			can_tx(&can_SH_mob2, SH_databytes2); //send the CAN Message only with 2 Hz as TYPK is not fast
			
			sys_time_500 = 0;
			}
		}
	}

