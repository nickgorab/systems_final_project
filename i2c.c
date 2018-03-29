/*
 *             i2c.c
 *
 *   Created on:  March 24, 2018
 *  Last Edited:  March 24, 2018
 *       Author:  Nick Gorab
 *        Board:  5994
 */


#include "system.h"

int     RX_Byte_Ctr;        // Coutner to make sure all of the information is received
int     TX_Byte_Ctr;        // Counter to make sure all of the information is sent
int i  =0;
int v = 0;
char    rx_data[6],          // Creates an array to store memory data
        tx_data[6],
        dataBuff[6];        // Data buffer which stores data to be sent



void i2c_init(void){
    P7SEL0    |= BIT0           // SDA line for I2C using UCB2
              |  BIT1;          // SCL line for I2C using UCB2
    UCB2CTLW0 |= UCSWRST;       // Enters reset state, USCI stops operation
    UCB2CTLW1 |= UCASTP_2;      // Sends stop bit when UCTBCNT is reached
    UCB2CTLW0 |= UCMST          // Master Mode
              |  UCMODE_3       // I2C Mode
              |  UCSSEL__SMCLK; // Synchronous Mode
    UCB2BRW    = 0x08;          // SMCLK/8
    UCB2CTLW0 &= ~UCSWRST;      // Exits reset mode, USCI enters operation
    UCB2IE    |= UCTXIE         // Data received interrupt enable
              |  UCRXIE;        // Data ready to transmit interrupt enable
}



void i2c_single_write(char slave_address, char memory_address, char * data){
    UCB2I2CSA = slave_address;          // Sets slave address
    TX_Byte_Ctr = 2;                    // Sets the amount of bytes to be sent
    UCB2TBCNT  = 0x02;                  // Expecting to receive 2 bytes of data


    /* Loads the data into the tx_data buffer */
    tx_data[0] = memory_address;

    /* Iterates through the pointed array */
    unsigned int j = 0;
    for(j = 0; j < 1; j++) {
        tx_data[j + 1] = data[j];     // Offset by 2 to avoid address overwrite
    }


    UCB2CTL1 |= UCTR                    // Enables TX Mode
             |  UCTXSTT;                // Sends start condition
    __bis_SR_register(LPM0_bits | GIE); // Enters Low-Power mode and enables global interrupt
}



void i2c_multiple_write(char slave_address, char memory_address, int size, char *data) {
    UCB2I2CSA = slave_address;          // Sets slave address
    TX_Byte_Ctr = 1 + size;             // Sets the amount of bytes to be sent
    UCB2TBCNT  = 1 + size;                  // Expecting to receive 2 bytes of data


    /* Loads the data into the tx_data buffer */
    tx_data[0] = memory_address;

    /* Iterates through the pointed array */
    unsigned int j = 0;
    for(j = 0; j < size; j++) {
        tx_data[j + 1] = data[j];     // Offset by 2 to avoid address overwrite
    }


    UCB2CTL1 |= UCTR                    // Enables TX Mode
             |  UCTXSTT;                // Sends start condition
    __bis_SR_register(LPM0_bits | GIE); // Enters Low-Power mode and enables global interrupt
}



void i2c_single_read(int slave_address, char memory_address){
    UCB2I2CSA = slave_address;              // Sets slave address
    RX_Byte_Ctr = 1;                        // Sets the amount of bytes to be received
    TX_Byte_Ctr = 1;                        // Sets the amount of bytes to be sent
    i = 0;                                  // Counting variable set to zero
    tx_data[0] = memory_address;            // Loads LSB into sending buffer

    UCB2CTL1 |= UCTR                        // Enables TX Mode
             |  UCTXSTT;                    // Sends start condition
     __bis_SR_register(LPM0_bits | GIE);    // Enters Low-Power mode and enables global interrupt
    UCB2TBCNT  = 0x01;                      // Sets 1 byte of data to be sent
    __delay_cycles(8);                      // Delays cycles for unknown reason

    UCB2CTL1 &= ~UCTR;                      // Enters RX Mode
    UCB2CTL1 |= UCTXSTT;                    // Sends start condition
    __bis_SR_register(LPM0_bits | GIE);     // Enters Low-Power mode and enables global interrupt
}



void i2c_multiple_read(int slave_address, char memory_address, int size){
    UCB2I2CSA = slave_address;              // Sets slave address
    UCB2TBCNT  = 4;                         // Set to 4 to avoid stop condition
    RX_Byte_Ctr = size;                     // Sets the amount of bytes to be received
    TX_Byte_Ctr = 1;                        // Sets the amount of bytes to be sent
    i = 0;                                  // Counting variable set to zero
    tx_data[0] = memory_address;            // Loads LSB into sending buffer

    UCB2CTL1 |= UCTR                        // Enables TX Mode
             |  UCTXSTT;                    // Sends start condition
     __bis_SR_register(LPM0_bits | GIE);    // Enters Low-Power mode and enables global interrupt
    UCB2TBCNT  = 0x03;                      // Sets 1 byte of data to be sent
    __delay_cycles(8);                      // Delays cycles for unknown reason
    UCB2TBCNT  = size;
    UCB2CTL1 &= ~UCTR;                      // Enters RX Mode
    UCB2CTL1 |= UCTXSTT;                    // Sends start condition
    __bis_SR_register(LPM0_bits | GIE);     // Enters Low-Power mode and enables global interrupt
}






#pragma vector = EUSCI_B2_VECTOR
__interrupt void USCI_B2_ISR(void) {
    switch(__even_in_range(UCB2IV, USCI_I2C_UCBIT9IFG)) {

        case USCI_I2C_UCRXIFG0:                             // I2C RX Interrupt
            if(RX_Byte_Ctr > 1){                            // Checks if there is more data to be received
                (&sensor_data)->data_buffer[v] = UCB2RXBUF;   // Loads the data array
                v++;                                        // Increments the pointer
                RX_Byte_Ctr--;                              // Decrement TX byte counter
            } else if (RX_Byte_Ctr == 1){
                (&sensor_data)->data_buffer[v] = UCB2RXBUF;   // Loads the data array
                v++;                                        // Increments the pointer
                RX_Byte_Ctr--;
                v=0;
                __bic_SR_register_on_exit(LPM0_bits);   // Exit LPM0
            }
        break;\

        case USCI_I2C_UCTXIFG0:                         // I2C TX Interrupt
            if(TX_Byte_Ctr > 1){
                UCB2TXBUF = tx_data[i];                // Example send
                i++;
                TX_Byte_Ctr--;
            } else if (TX_Byte_Ctr == 1){
                UCB2TXBUF = tx_data[i];                // Example send
                TX_Byte_Ctr--;
                i = 0;
                __bic_SR_register_on_exit(LPM0_bits); // Exit LPM0
        }
            break;
}   }
