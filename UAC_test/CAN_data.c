/*
 * CAN_data.c
 *
 * Created: 31.03.2025 14:41:09
 *  Author: pasca
 */ 


#include "CAN_data.h"

uint8_t UAC0_databytes[8];
uint8_t UAC1_databytes[8];

struct CAN_MOB can_UAC0_mob;
struct CAN_MOB can_UAC1_mob;

CAN_Init_Messages(){
	
	can_UAC0_mob.mob_id = 0x730;
	can_UAC0_mob.mob_idmask = 0xFFFF;//sent
	can_UAC0_mob.mob_number = 0;
	
	can_UAC1_mob.mob_id = 0x731;
	can_UAC1_mob.mob_idmask = 0xFFFF;//sent
	can_UAC1_mob.mob_number = 1;	
}