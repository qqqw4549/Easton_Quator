#include "myiic.h"


void i2c_WriteReg(uint8_t dev,uint8_t reg,uint8_t val){
	uint8_t writeval = val;
	HAL_I2C_Mem_Write_DMA(&hi2c1 ,dev, reg, 1 , &writeval , 1);
}

uint8_t i2c_ReadReg(uint8_t dev,uint8_t val){
	static uint8_t readval = 0;
	HAL_I2C_Mem_Read_DMA(&hi2c1 ,dev, val, 1 , &readval , 1);
	return readval;
}
void i2c_ReadMuti(uint8_t dev ,uint8_t len , uint8_t reg ,uint8_t * Buffer){
	HAL_I2C_Mem_Read_DMA(&hi2c1 ,dev, reg, len , Buffer , len);
}
