/*
 *            mpu6050.c
 *
 *   Created on:  March 24, 2017
 *  Last Edited:  March 24, 2017
 *       Author:  Nick Gorab
 *        
 *  This code is used for interfacing with the
 *  MPU6050 Gyro/Accelerometer used for this project.
 */


#include "mpu6050.h"



void MPU6050_Wake_Up(void) {
    char data[1] = {0};
    i2c_single_write(MPU6050_ADDRESS_LOW, PWR_MGMT_1, data);
}

char MPU6050_Get_ID(sensdat_t* sensdat) {
	i2c_single_read(MPU6050_ADDRESS_LOW, MPU6050_ID);

	return sensdat->data_buffer[0];
}

/* Returns the accelerometer data */
void MPU6050_Get_Accel(sensdat_t* sensdat) {
	i2c_multiple_read(MPU6050_ADDRESS_LOW, ACCEL_DATA, 6);
	char msb, lsb;

	/* Determining X acceleration */
	lsb = sensdat->data_buffer[1];
	msb = sensdat->data_buffer[0];
	sensdat->x_accel = ((msb << 8) | lsb);

	/* Determining y acceleration */
	lsb = sensdat->data_buffer[3];
	msb = sensdat->data_buffer[2];
	sensdat->y_accel = ((msb << 8) | lsb);

	/* Determining z acceleration */
	lsb = sensdat->data_buffer[5];
	msb = sensdat->data_buffer[4];
	sensdat->z_accel = ((msb << 8) | lsb);
}



void MPU6050_Calibrate_Accel(sensdat_t* sensdat) {
    P1OUT |= BIT1;		// Turns on status LED for calibration

    /* Varibales used to store the mean accelerometer data */
  	int x_mean = 0; 
  	int y_mean = 0;
  	int z_mean = 0;

    MPU6050_Mean_Accel(&sensor_data, &x_mean, &y_mean, &z_mean);

    int x_offset = -x_mean/8;
    int y_offset = -y_mean/8;
    int z_offset = (16384-z_mean)/8;

    while(1) {
    	int calibrated = 0; 

    	/* Sets the new offsets */
    	Set_X_Accel_Offset(x_offset);
    	Set_Y_Accel_Offset(y_offset);
    	Set_Z_Accel_Offset(z_offset);

    	MPU6050_Mean_Accel(&sensor_data, &x_mean, &y_mean, &z_mean);

    	/* Checks to see if the means are within the limits */
    	if(abs(x_mean)<=ACCEL_DEADZONE) {
    		calibrated++;
    	} else {
    		x_offset = x_offset - x_mean/ACCEL_DEADZONE;
    	}

    	if(abs(y_mean)<=ACCEL_DEADZONE) {
    		calibrated++;
    	} else {
    		y_offset = y_offset - y_mean/ACCEL_DEADZONE;
    	}

    	if(abs(16384-z_mean)<=ACCEL_DEADZONE) {
    		calibrated++;
    	} else {
    		z_offset = z_offset + (16384-z_mean)/ACCEL_DEADZONE;
    	}

    	/* If all of the axis pass calibration */
    	if(calibrated == 3) {
    		break;
    	}
    }
}


void MPU6050_Mean_Accel(sensdat_t* sensdat, int* x_mean, int* y_mean, int* z_mean) {

   uint32_t x_accel_buffer = 0;
   uint32_t y_accel_buffer = 0;
   uint32_t z_accel_buffer = 0;

   int i = 0;
   while(i < (CALIB_BUF_SIZE+1)) {
       MPU6050_Get_Accel(&sensor_data);
       x_accel_buffer += sensdat->x_accel;
       y_accel_buffer += sensdat->y_accel;
       z_accel_buffer += sensdat->z_accel;
       i++;
       __delay_cycles(2);
   	}

  	*x_mean = x_accel_buffer/CALIB_BUF_SIZE;
   	*y_mean = y_accel_buffer/CALIB_BUF_SIZE;
   	*z_mean = z_accel_buffer/CALIB_BUF_SIZE;

}

/* Sets the offset of the x-axis */
void Set_X_Accel_Offset(int offset) {
	int MSB = offset >> 8;
	int LSB = offset & 0xFF;

	char offset_data[2] = {MSB, LSB};

	i2c_multiple_write(MPU6050_ADDRESS_LOW, X_OFFSET_ADDRESS, 2, offset_data);

}

/* Sets the offset for the y-axis */
void Set_Y_Accel_Offset(int offset) {
	int MSB = offset >> 8;
	int LSB = offset & 0xFF;

	char offset_data[2] = {MSB, LSB};

	i2c_multiple_write(MPU6050_ADDRESS_LOW, Y_OFFSET_ADDRESS, 2, offset_data);
}

/* Sets the offset for the z-axis */
void Set_Z_Accel_Offset(int offset) {
	int MSB = offset >> 8;
	int LSB = offset & 0xFF;

	char offset_data[2] = {MSB, LSB};

	i2c_multiple_write(MPU6050_ADDRESS_LOW, Z_OFFSET_ADDRESS, 2, offset_data);
	
}



