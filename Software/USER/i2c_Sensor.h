#ifndef __I2C_Sensor_H
#define	__I2C_Sensor_H

#include "stm32f10x.h"

#define I2C_Speed       100000  
#define sensor_address  1

void I2C1_GPIO_Config(void);
void I2C1_Mode_Configu(unsigned char slve_address);

#endif /* __I2C_Sensor_H */