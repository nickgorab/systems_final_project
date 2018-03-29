/*
 *             i2c.h
 *
 *   Created on:  February 18, 2017
 *  Last Edited:  February 18, 2018
 *       Author:  Nick Gorab
 *        Board:  5994
 */


#ifndef I2C_H_
#define I2C_H_

#include "system.h"

void i2c_init(void);
void i2c_single_write(char slave_address, char memory_address, char *tx_data);
void i2c_single_read(int slave_address, char memory_address);
void i2c_multiple_write(char slave_address, char memory_address, int size, char *tx_data);
void i2c_multiple_read(int slave_address, char memory_address, int size);

#endif /* I2C_H_ */
