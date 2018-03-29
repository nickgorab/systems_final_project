/**
 * main.c
 */

#include "system.h"

/* Global struct deceleration */
sensdat_t sensor_data;



/* Initializes the MPU6050 struct */
void Data_Struct_Init(sensdat_t* sendat) {

    sensdat = sensdat;

    int i = 0;
    for(i = 0; i <= sizeof(sensdat->data_buffer); i++) {
        sensdat->data_buffer[i] = 0x00;
    }

    sensdat->x_accel   = 0;
    sensdat->y_accel   = 0;
    sensdat->z_accel   = 0;
    sensdat->prox_data = 0;

    return;
}

void Sensor_Connection_Test(sensdat_t* sensdat) {
    int connected = 0;
    while(1) {
        if(VCNL4200_Get_ID(sensdat) == 0x68) {
            connected++;
        }
        if(connected == 1) {
            break;
        }
    }
}


int main(void)
{

    WDTCTL = WDTPW | WDTHOLD;                     // Disables Watchdog Timer
    PM5CTL0 &= ~LOCKLPM5;                         // Disables high-impedance mode
    uint16_t proxdat = 0;

    i2c_init();
    Data_Struct_Init(&sensor_data);
   // MPU6050_Wake_Up();
    P1DIR |= BIT1;
    P1OUT &= ~BIT1;
  //  VCNL4200_Read_CONF_REG();
    VCNL4200_Start_PS();

    __delay_cycles(50);
    while(1) {
    proxdat = VCNL4200_Get_PS_Data(&sensor_data);
    }

    __no_operation();
}
