/*
 * uFAR.c
 *
 * Created: 04.04.2019 20:52:02
 * Author : Ole Hannemann
 */

#include "SPI.h"
#include "adc_functions.h"
#include "canlib.h"
#include "misc_functions.h"
#include <avr/interrupt.h>
#include <avr/io.h>

// Constants
#define ADC_MAX_VALUE 1023  // 10-bit ADC maximum value
#define ADC_VOLTAGE_REF 5.0 // Reference voltage for ADC
#define ZERO_G_VOLTAGE 2.5  // Voltage at 0g from the sensor
#define SENSITIVITY 0.02    // Sensor sensitivity in V/g
#define MAX_G 100.0         // Maximum g value for the sensor

extern unsigned long sys_time;
volatile unsigned long time_old = 0;
volatile uint8_t sys_time_10 = 0;
volatile uint8_t sys_time_50 = 0;
uint16_t adc_acceleration = 0;
uint16_t acceleration = 0;
uint8_t sign_bit = 0;

int
main(void)
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

    struct CAN_MOB can_UAC_mob1;
    can_UAC_mob1.mob_id = 0x702;
    can_UAC_mob1.mob_idmask = 0; // We are sending this CAN Message Object (MOB)
                                 // therefore we do not need an ID MASK
    can_UAC_mob1.mob_number = 1;
    uint8_t UAC_databytes1[2];

    struct CAN_MOB can_UAC_mob2;
    can_UAC_mob1.mob_id = 0x703;
    can_UAC_mob1.mob_idmask = 0; // We are sending this CAN Message Object (MOB)
                                 // therefore we do not need an ID MASK
    can_UAC_mob1.mob_number = 2;
    uint8_t UAC_databytes2[4];

    sei();

    while (1) {
        // 1ms loop
        if ((sys_time - time_old) >= 1) {
            time_old = sys_time;

            sys_time_10++;

            // 10ms loop
            if (sys_time_10 >= 10) {
                adc_start_conversion();
                uint16_t adcValue =
                  adc_get_1(); // Get the ADC value from the sensor
                float voltage =
                  (adcValue / (float)ADC_MAX_VALUE) * ADC_VOLTAGE_REF;
                float acceleration = (voltage - ZERO_G_VOLTAGE) / SENSITIVITY;

                // Convert acceleration to a range from -100.0 to +100.0 g
                int16_t scaledAcceleration = (int16_t)(acceleration * 100);

                // Prepare CAN message
                uint8_t UAC_databytes[2];
                if (scaledAcceleration < 0) {
                    scaledAcceleration = -scaledAcceleration;
                    UAC_databytes1[0] =
                      (uint8_t)(scaledAcceleration & 0xFF); // Lower byte
                    UAC_databytes1[1] =
                      (uint8_t)((scaledAcceleration >> 8) &
                                0x7F); // Higher byte with sign bit cleared
                    UAC_databytes1[1] |=
                      0x80; // Set the sign bit for negative values
                } else {
                    UAC_databytes1[0] =
                      (uint8_t)(scaledAcceleration & 0xFF); // Lower byte
                    UAC_databytes1[1] =
                      (uint8_t)((scaledAcceleration >> 8) &
                                0x7F); // Higher byte with sign bit cleared
                }
                UAC_databytes1[2] = 0;
                UAC_databytes1[3] = 0;
                UAC_databytes1[4] = 0;
                UAC_databytes1[5] = 0;
                UAC_databytes1[6] = 0;
                UAC_databytes1[7] = 0;

                can_tx(&can_UAC_mob1, UAC_databytes1); // send the CAN Message	100Hz
                sys_time_10 = 0;
                sys_time_50++;
            }
        }

        // 50ms loop
        if (sys_time_50 >= 10) { //10Hz
            sys_tick();
			//Platzhalter für Bremstemperaturen
				UAC_databytes2[0] = 0;
				UAC_databytes2[1] = 0;
				UAC_databytes2[2] = 0;
				UAC_databytes2[3] = 0;
	            UAC_databytes2[4] = 0;
				UAC_databytes2[5] = 0;
				UAC_databytes2[6] = 0;
				UAC_databytes2[7] = 0;
			can_tx(&can_UAC_mob2, UAC_databytes2); // send the CAN Message 10Hz
             sys_time_50 = 0;
        }
    }
}


