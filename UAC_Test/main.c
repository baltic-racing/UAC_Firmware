/*
 * UAC_Test.c
 *
 * Created: 09.11.2024 16:23:23
 * Author : Egquus
 */ 

#include "main.h"

//Typ K
uint16_t Typ_K1_temp = 0;	// brake disc temp
uint16_t Typ_K2_temp = 0;	// brake fluid temp
uint8_t channel = 0;

uint8_t switchi = 0;

extern uint8_t UAC0_databytes[8];

extern struct CAN_MOB can_UAC0_mob;

int main(void)
{
	port_config();
	sys_timer_config();
	//adc_config(); //CHECK WHICH ADC INPUTS CORRESPOND WITH WHAT DATABYTE WE SEND
	can_cfg();
	CAN_Init_Messages();
	SPI_MasterInit();
	
	struct CAN_MOB can_UAC0_mob;
	can_UAC0_mob.mob_id = 0x731;
	can_UAC0_mob.mob_idmask = 0xFFFF;//sent
	can_UAC0_mob.mob_number = 0;
	uint8_t UAC0_databytes[8] = {0};
	
	sei();

	while (1)
	{
		if(TIME_PASSED_1_MS)
		{
			time_1ms = sys_time;
			

		} // end of 1ms
	
		if(TIME_PASSED_10_MS)
		{
			//adc_start_conversion();
			
			time_10ms = sys_time;
		
		} // end of 10ms

		if (TIME_PASSED_100_MS)
		{
						
			sys_tick_heart();
				
			time_100ms = sys_time;						
		} // end of 100ms
	
		if (TIME_PASSED_200_MS)
		{
			if(switchi == 0)
			{
				Typ_K1_temp = read_brake_temp(0); 
				switchi = 1;
				
				if(SPDR == 0x04)
				{
					Temp_Disc_Fail;
					deadend_counter++;
				}
			}
			if(switchi == 1)
			{
				Typ_K2_temp = read_brake_temp(1);
				switchi = 0;
				
				if(SPDR == 0x04)
				{
					Temp_Fluid_Fail;
					deadend_counter++;
				}
			}
			
			// CAN bus
			UAC0_databytes[0]	=	(Typ_K2_temp	>>	8)	&	0xFF							;	//lsb
			UAC0_databytes[1]	=	(Typ_K2_temp)			&	0xFF							;	//msb
			UAC0_databytes[2]	=	(Typ_K1_temp	>>	8)	&	0xFF							;	//msb // & 0x3F da ersten 4 bit (links) nicht zur Temperatur gehören, da 12 bit Auflösung
			UAC0_databytes[3]	=	(Typ_K1_temp)			&	0xFF							;	//lsb
			UAC0_databytes[4]	=	0x66														;
			UAC0_databytes[5]	=	(deadend_counter)		&	0xFF							;
			UAC0_databytes[6]	=	(Temp_Fluid_Fail)		&	0xFF							;
			UAC0_databytes[7]	=	(Temp_Disc_Fail)		&	0xFF							;	// 1 = no typ K connected
			
			can_tx(&can_UAC0_mob, UAC0_databytes);
			
			time_200ms = sys_time;
		} // end of 200ms
	}
}