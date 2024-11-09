/*
 * UAC_Test.c
 *
 * Created: 09.11.2024 16:23:23
 * Author : Egquus
 */ 

#include "main.h"

int main(void)
{
	port_config();
	sys_timer_config();
	SPI_MasterInit();
	
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
			PORTB &= ~(1<<SS_TK1);//Switch SS_TYPK_1 on (Low)
			SPDR = 0x22;
			/*while(!(SPSR & (1<<SPIF)));*/
			brake_disc_temp = SPDR;
			_delay_ms(100);
			PORTB |= (1<<SS_TK1);//Switch SS_TYPK_1 off (High)
		
			time_100ms = sys_time;
		
			sys_tick_heart();
		
		} // end of 100ms
	
		if (TIME_PASSED_200_MS)
		{
			time_200ms = sys_time;
		
		} // end of 200ms
	}
}