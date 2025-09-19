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
			TK1_temp = (uint16_t)read_TK_temperature(TK2);
			
			// SPI communication
			PORTB &= ~(1<<SS_TK1);//Switch SS_TYPK_1 on (Low)
			
			SPDR = 0x00;
			deadend_counter = 0;
				
			while(!(SPSR & (1<<SPIF)))
				{
					if(++deadend_counter >= 200)
						{
							PORTA |= (1<<PA0);					//Enter Error state cause of failing SPI_Interrrupt_Flag  [Error-LED = On]
							
							PORTB |= (1<<SS_TK1);				//Switch SS_TYPK_1 off (High)
							break;;
						}
					else
					{
						PORTA &= ~(1<<PA0);					//Leave Error state cause set SPI_Interrrupt_Flag  [Error-LED = Off]
					}
				}
			brake_disc_temp = ((uint16_t)SPDR << 8);		//MSB
			
			SPDR = 0x00;
			while(!(SPSR & (1<<SPIF)))
			{
				if(++deadend_counter >= 200)
				{
					PORTA |= (1<<PA0);					//Enter Error state cause of failing SPI_Interrrupt_Flag  [Error-LED = On]
					
					PORTB |= (1<<SS_TK1);				//Switch SS_TYPK_1 off (High)
					break;;
				}
				else
				{
					PORTA &= ~(1<<PA0);					//Leave Error state cause set SPI_Interrrupt_Flag  [Error-LED = Off]
				}
			}
			brake_disc_temp = SPDR;							//LSB
			
			PORTB |= (1<<SS_TK1);							//Switch SS_TYPK_1 off (High)
			
			
			// CAN bus
			UAC0_databytes[0]	=	0x66													;	//lsb
			UAC0_databytes[1]	=	0x66													;	//msb
			UAC0_databytes[2]	=	(brake_disc_temp)	& 0xFF								;	//lsb
			UAC0_databytes[3]	=	(brake_disc_temp	>>	8)	& 0xFF						;	//msb
			UAC0_databytes[4]	=	0x66													;
			UAC0_databytes[5]	=	0x66													;
			UAC0_databytes[6]	=	0x66													;
			UAC0_databytes[7]	=	0x66													;
			
			can_tx(&can_UAC0_mob, UAC0_databytes);
			
			time_200ms = sys_time;				
		} // end of 200ms
	}
}