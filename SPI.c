/*
 * SPI.c
 *
 * Created: 25.03.2021 17:23:26
 *  Author: lukas
 */  

#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint8_t SPIdata[4];
uint8_t SPIcount = 0;

void SPI_config(){
	DDRB  |= (1<<PB0) | (1<<PB1) | (1<<PB2);	// define SS, SCK & MOSI as Output
	DDRB  &= ~(1<<PB3);							// define MISO as input
	DDRB  |= (1<<PB4) | (1<<PB5);				// define CS as Output
	PORTB |= (1<<PB0);							//Switch SS off
	PORTB |= (1<<PB4) | (1<<PB5);				//Switch CS off
	
	SPCR = (1<<SPIE)|(1<<SPE)|(0<<DORD)|(1<<MSTR)|(0<<CPOL)|(1<<CPHA)|(1<<SPR1)|(1<<SPR0);
	SPSR &= ~(0<<SPI2X);
	//SPIE activates SPI Interrupt; SPE Enable SPI; DORD to MSB first see. MAX6675 Datasheet; MSTR set AT90CAN as Master;  
	//CPOL/CPHA set to read Data on falling edge & have clock to pulse from low to High; SPI2X/SPR1/SPR0 set clock divider to 64 = 250Khz max freq. for MAX6675 10Mhz (100ns on time)

}

void TYPK_read(){
	PORTB &= ~(1<<PB0);//Switch SS on
	PORTB &= ~(1<<PE4);//Switch CS_TYPK_1 on
	
	SPDR=0x11; //send empty byte to initiate data Transmission
	//Then we need to go into the interrupt when the transmission of the first byte is finished
}

uint16_t TYPK_getdata1(){
	//cli(); //disable Interrupts so that the SPI transfer will not corrupt our data in a way that we get the MSB of n and LSB of n-1
	uint16_t TYPkDATA1 =  ((SPIdata[0] << 8) | SPIdata[1]) >> 3;//PUT MSB shifted to left by 8 in place and or together with LSB then shift to right by three to get rid of Status bits
	//sei(); // enable Interrupts again
	return TYPkDATA1;	
}

uint16_t TYPK_getdata2(){
	//cli(); //disable Interrupts so that the SPI transfer will not corrupt our data in a way that we get the MSB of n and LSB of n-1
	uint16_t TYPkDATA2 =  ((SPIdata[2] << 8) | SPIdata[3]) >> 3;//PUT MSB shifted to left by 8 in place and or together with LSB then shift to right by three to get rid of Status bits
	//sei(); // enable Interrupts again
	return TYPkDATA2;										
}

	ISR(SPI_STC_vect){
		//Store the data that has been pushed into SPDR via the Slave
		SPIdata[SPIcount] = ~SPDR;
		SPIcount++; //incrementing the counter that indicates the number of bytes that has been transmitted
		//If the entire message has been transmitted terminate the SPI data transfer by pulling Chip Select low
		if (SPIcount==2){
			PORTB |= (1<<PE4);//Switch CS_TYPK_1 off
			PORTB &= ~(1<<PE5);//Switch CS_TYPK_2 on
		}
		if (SPIcount>=4){
			PORTB |= (1<<PB0);//Switch SS off, High=off, Low=on
			PORTB |= (1<<PE5);//Switch CS_TYPK_2 off

			SPIcount=0;//Resetting counter for next SPI transmission
		}
		else{
		//Initiate the next SPI transmission to get the remaining Bytes
		SPDR=0x11; //send empty byte to initiate data Transmission		
		}
	}