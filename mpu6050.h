/*
 *            mpu6050.h
 *
 *   Created on:  March 24, 2017
 *  Last Edited:  March 24, 2017
 *       Author:  Nick Gorab
 *        
 *  This code is used for interfacing with the
 *  MPU6050 Gyro/Accelerometer used for this project.
 */

#ifndef MPU6050_H_
#define MPU6050_H_

#include "system.h"

/* Internal addresses for register access */
#define MPU6050_ADDRESS_LOW	 0x68
#define MPU6050_ADDRESS_HIGH 0x69	// Might be used to avoid noise spikes altering address
#define ACCEL_CONFIG		 0x1C
#define ACCEL_DATA 			 0x3B
#define PWR_MGMT_1           0x6B
#define MPU6050_ID			 0x75	// Should always read 0x68

/* Calibration constraints */
#define	ACCEL_DEADZONE		8
#define CALIB_BUF_SIZE	    100

/* Offset Register Addresses */
#define X_OFFSET_ADDRESS 	0X06
#define Y_OFFSET_ADDRESS 	0X08
#define Z_OFFSET_ADDRESS 	0X0A

/* Functions */
void MPU6050_Init(sensdat_t* sensdat);
void MPU6050_Get_Accel(sensdat_t* sensdat);
void MPU6050_Calibrate_Accel(sensdat_t* sensdat);
void MPU6050_Mean_Accel(sensdat_t* sensdat, int* x_offset, int* y_offset, int* z_offset);
void Set_X_Accel_Offset(int offset);
void Set_Y_Accel_Offset(int offset);
void Set_Z_Accel_Offset(int offset);
void MPU6050_Wake_Up(void);
char MPU6050_Get_ID(sensdat_t* sensdat);



#endif /* MPU6050_H_ */
