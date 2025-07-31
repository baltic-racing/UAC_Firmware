/*
 * SPI_lib_light.h
 *
 * Created: 09.11.2024 16:35:07
 *  Author: Egquus
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef SPI_LIB_LIGHT_H_
#define SPI_LIB_LIGHT_H_

#define MOSI PB2							//Start: individual configuration
#define MISO PB3
#define SCK PB1					
#define SS_TK1 PB4
#define SS_TK2 PB5							//End

#define SPI_Control_Reg SPCR
	#define SPI_Interrupt_Enable SPIE
	#define SPI_Enable SPE
	#define SPI_Data_Order DORD
	#define SPI_Master MSTR
	#define SPI_Clock_Polarity CPOL
	#define SPI_Clock_Phase CPHA
	#define SPI_Relation0 SPR0				// Attention, this also affects the clock frequency [see Table SPI_1]
	#define SPI_Relation1 SPR1				// Attention, this also affects the clock frequency [see Table SPI_1]

#define SPI_Status_Reg SPSR
	#define SPI_Interrupft_Flag SPIF
	#define SPI_Collision_Flag WCOL
	#define SPI_Double_Speed SPI2X			// Attention, this also affects the clock frequency [see Table SPI_1]
	
#define SPI_Data_Reg SPDR


#define SS_TK1_LOW() (PORTB &= ~(1 << SS_TK1))
#define SS_TK1_HIGH() (PORTB |= (1 << SS_TK1))
#define SS_TK2_LOW() (PORTB &= ~(1 << SS_TK2))
#define SS_TK2_HIGH() (PORTB |= (1 << SS_TK2))
//#define LEFT 0x01
//#define RIGHT 0x03

void SPI_MasterInit();
void SPI_SlaveInit(void);
char SPI_SlaveReceive(void);

//ISR(SPI_STC_vect);


#endif /* SPI_LIB_LIGHT_H_ */

//SPCR -> SPI Control Register | SPIE | SPE | DORD | MSTR | CPOL | CPHA | SPR1 | SPR0
//SPIE ->
/*  _________________________________________________________
	H	SPI2X	H	SPR1	H	SPR0	H	SCK_Frequency	H
	H___________H___________H___________H___________________H
	H	0		H	0		H	0		H	f_clkio/4		H
	H	0		H	0		H	1		H	f_clkio/16		H
	H	0		H	1		H	0		H	f_clkio/64		H
	H	0		H	1		H	1		H	f_clkio/128		H
	H	1		H	0		H	0		H	f_clkio/2		H
	H	1		H	0		H	1		H	f_clkio/8		H
	H	1		H	1		H	0		H	f_clkio/32		H
	H	1		H	1		H	1		H	f_clkio/64		H
	_________________________________________________________
	Table SPI_1												*/