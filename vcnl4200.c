/*
 * 			vcnl4200.c
 *
 *   Created on:  March 26, 2018
 *  Last Edited:  March 26, 2018
 *       Author:  Nick Gorab
 *        Board:  MSP403FR5994
 * 
 *  This code is used to interface with the VCNL4200 distance sensor
 */

#include "system.h"

uint16_t VCNL4200_Get_PS_Data(sensdat_t* sensdat) {
	i2c_multiple_read(VCNL4200_ADDRESS, VCNL4200_PROXIMITY_REG, 2);
	sensdat->prox_data = ((sensdat->data_buffer[0] << 8) | sensdat->data_buffer[1]);

	return sensdat->prox_data;
}

uint16_t VCNL4200_Get_ALS_Data(sensdat_t* sensdat) {
    i2c_multiple_read(VCNL4200_ADDRESS, VCNL4200_AMBIENT_REG, 2);
    sensdat->prox_data = ((sensdat->data_buffer[0] << 8) | sensdat->data_buffer[1]);

    return sensdat->prox_data;
}

void VCNL4200_Read_CONF_REG(void) {
    char i = 0;

    while(i < 0x0F) {
    i2c_multiple_read(VCNL4200_ADDRESS, i, 2);
    i++;
    }
}

uint16_t VCNL4200_Get_ID(sensdat_t* sensdat) {
	i2c_multiple_read(VCNL4200_ADDRESS, VCNL4200_DeviceID_REG, 2);
	uint16_t vcnl4200_id = ((sensdat->data_buffer[0] << 8) | sensdat->data_buffer[1]);

	return vcnl4200_id;
}

void VCNL4200_Init(void) {
	char data[2] = {0x88, 0x13};
	i2c_multiple_write(VCNL4200_ADDRESS, VCNL4200_PS_THDL_REG, 2, data);
	data[0] = 0xE0;
	data[1] = 0x2E;
	i2c_multiple_write(VCNL4200_ADDRESS, VCNL4200_PS_THDH_REG, 2, data);

}

void VCNL4200_Start_PS(void) {
    char data[1] = {0};
    i2c_single_write(VCNL4200_ADDRESS, VCNL4200_PS_CONF1_CONF2_REG, data);
}

void VCNL4200_Start_CONF3_MS(void){	
    char data[2] = {0x00, 0x20};
    i2c_multiple_write(VCNL4200_ADDRESS, VCNL4200_PS_CONF3_MS_REG, 2, data);
}

void VCNL4200_Start_ALS(void) {
    char data[1] = {0};
    i2c_single_write(VCNL4200_ADDRESS, VCNL4200_ALS_CONF_REG, data);
}
