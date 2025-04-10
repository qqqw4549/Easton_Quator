#ifndef _MYIIC_H_
#define _MYIIC_H_

#include <stdint.h>
#include "i2c.h"
#include "main.h"
void i2c_WriteReg(uint8_t dev,uint8_t reg,uint8_t val);
uint8_t i2c_ReadReg(uint8_t dev,uint8_t val);
void i2c_ReadMuti(uint8_t dev ,uint8_t len , uint8_t reg ,uint8_t * Buffer);
#endif
