/*
 * 			vcnl4200.h
 *
 *   Created on:  March 26, 2018
 *  Last Edited:  March 26, 2018
 *       Author:  Nick Gorab
 *        Board:  MSP403FR5994
 * 
 *  This code is used to interface with the VCNL4200 distance sensor
 */

#include "system.h"

#define VCNL4200_ADDRESS	0x51


/* Device specific registers */
#define VCNL4200_ALS_CONF_REG 0x00
#define VCNL4200_ALS_THDH_REG 0x01 //Ambient Light Sensor Threshold Data High
#define VCNL4200_ALS_THDL_REG 0x02 //Ambient Light Sensor Threshold Data Low
#define VCNL4200_PS_CONF1_CONF2_REG 0x03
#define VCNL4200_PS_CONF3_MS_REG 0x04 //Conf3 and Mode Select
#define VCNL4200_PS_CANC_REG 0x05
#define VCNL4200_PS_THDL_REG 0x06 //Proximity Sensor Threshold Data Low
#define VCNL4200_PS_THDH_REG 0x07 //Proximity Sensor Threshold Data High
#define VCNL4200_PROXIMITY_REG 0x08
#define VCNL4200_AMBIENT_REG 0x09
#define VCNL4200_WHITE_REG 0x0A
#define VCNL4200_INT_FLAG_REG 0x0D
#define VCNL4200_DeviceID_REG 0x0E


uint16_t VCNL4200_Get_PS_Data(sensdat_t* sensdat);
uint16_t VCNL4200_Get_ALS_Data(sensdat_t* sensdat);
uint16_t VCNL4200_Get_ID(sensdat_t* sensdat);
void VCNL4200_Start_PS(void);
void VCNL4200_Start_ALS(void);
void VCNL4200_Init();
void VCNL4200_Read_CONF_REG(void);
void VCNL4200_Start_CONF3_MS(void);
