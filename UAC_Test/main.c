/*
 * UAC_Test.c
 *
 * Created: 09.11.2024 16:23:23
 * Author : Egquus
 */ 

#include "main.h"

//Typ K
uint16_t TK1_temp = 0;
uint16_t TK2_temp = 0;

uint8_t deadend_counter = 0;
uint8_t zweitakt = 0;

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
			
			// CAN bus
			UAC0_databytes[0]	=	0x66													;	//lsb
			UAC0_databytes[1]	=	0x66													;	//msb
			UAC0_databytes[2]	=	(brake_disc_temp_Grad_C	>>	8)	& 0x3F					;	//msb // & 0x3F da ersten 4 bit nicht zur Temperatur gehören
			UAC0_databytes[3]	=	(brake_disc_temp_Grad_C)	& 0xFF						;	//lsb
			UAC0_databytes[4]	=	0x66													;
			UAC0_databytes[5]	=	0x66													;
			UAC0_databytes[6]	=	0x66													;
			UAC0_databytes[7]	=	thermo_open												;	// 1 = no typ K connected
			
			can_tx(&can_UAC0_mob, UAC0_databytes);
			
			time_200ms = sys_time;				
		} // end of 200ms	
			
			
			
		if (TIME_PASSED_1000_MS)
		{
		// SPI communication
				PORTB &= ~(1<<SS_TK1);						//Switch SS_TYPK_1 on (Low)
				
				//start msb-read
					
				SPDR = 0x00;
				deadend_counter = 0;
						
				while(!(SPSR & (1<<SPIF)))					//wait for SPI_Interrupt_Flag
				{
					deadend_counter++ ;
						
					if(deadend_counter >= 200)				//for case that SPIF isnt send 
					{
						PORTA |= (1<<PA0);					//Enter Error state cause of failing SPI_Interrrupt_Flag  [Error-LED = On]
						break;								// quit while-loop
					}
					else
					{
						PORTA &= ~(1<<PA0);					//Leave Error state cause set SPI_Interrrupt_Flag  [Error-LED = Off]
					}
				}
					
				brake_disc_temp_msb = SPDR;					//msb
				
				//start lsb-read
					
				SPDR = 0x00;
				deadend_counter = 0;
					
				while(!(SPSR & (1<<SPIF)))					//wait for SPI_Interrupt_Flag
				{
					deadend_counter++ ;
						
					if(deadend_counter >= 200)				//for case that SPIF isnt send
					{
						PORTA |= (1<<PA0);					//Enter Error state cause of failing SPI_Interrrupt_Flag  [Error-LED = On]
						break;
					}
					else
					{
						PORTA &= ~(1<<PA0);					//Leave Error state cause set SPI_Interrrupt_Flag  [Error-LED = Off]
					}
				}
					
				brake_disc_temp_lsb = SPDR;					//lsb
									
				PORTB |= (1<<SS_TK1);													//Switch SS_TYPK_1 off (High)
					
				brake_disc_temp = (brake_disc_temp_lsb | (brake_disc_temp_msb << 8));	// Bit D0 bis D2 für Temperatur uninteressant
				brake_disc_temp_normal = (brake_disc_temp >> 3);						// shift sodass die 12 letzten Bits die Temperatur anzeigen
					
				thermo_open = ((brake_disc_temp_lsb >> 2) & 0x1) ;						// check if TYP K is connected
				
				brake_disc_temp_Grad_C = brake_disc_temp_normal * 25;					// 25 = 100/4 (*100 für 2 Dezimalstellen, /4 für auflösung)
				
					
				
			time_1000ms = sys_time;
		} // end of 1000ms
	}
}