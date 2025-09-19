/*
 * SPI_lib_light.c
 *
 * Created: 09.11.2024 16:34:57
 *  Author: Egquus
 */ 

#include "SPI_lib_light.h"

extern volatile uint16_t brake_disc_temp;

void SPI_MasterInit()
{
	//Set MOSI, SCK and all SS as output
	DDRB |= (1<<MOSI) | (1<<SCK);
	DDRB |= (1<<SS_TK1) | (1<<SS_TK2);
	
	//Set MISO as input
	DDRB &= ~(1<<MISO);
	
	// Set internal pull-up for MISO
	//PORT_ |= (1 << MISO);
	
	//enables SPI, Master-Mode, configure Clock_Select
	SPI_Control_Reg |= (1<<SPI_Enable) | (1<<SPI_Master) | (0<<SPI_Relation1) | (1<<SPI_Relation0);
	
	//configure Clock_Select [2 MHz]
	SPI_Status_Reg |= (1<<SPI_Double_Speed);
	
	//Set all SS High
	PORTB |= (1<<SS_TK1) | (1<<SS_TK2);
}

void SPI_SlaveInit()
{
	//Set MOSI, SCK, SS as input
	DDRB &= ~((1<<MOSI) | (1<<SCK));
	//PORTB |= 1<<SS_uC;
	
	//Set MISO as output
	DDRB |= (1<<MISO);
	
	//enables SPI
	SPI_Control_Reg |= (1<<SPI_Enable) | (1<<SPI_Interrupt_Enable);
	
	DDRA &= ~(1<<PA4);
}

char SPI_SlaveReceive()											// Use with care -> stop the uC
{																//
	/* Wait for reception complete */							//
	while(!(SPI_Status_Reg & (1<<SPI_Interrupft_Flag)));		//
	/* Return data register */									//
	return SPI_Data_Reg;										//
}																//

uint8_t SPI_transfer(uint8_t data){
	SPDR = data;
	while (!(SPSR & (1 << SPIF)));
	return SPDR;
}

//ISR(SPI_STC_vect)
//{
	//brake_disc_temp = SPDR;
	//DDRB &= ~(1<<SS_TK1);
//}