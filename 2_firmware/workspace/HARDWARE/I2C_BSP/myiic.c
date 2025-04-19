#include "myiic.h"


void i2c_WriteReg(uint8_t dev,uint8_t reg,uint8_t val){
	static uint8_t writeval;
	writeval = val;
	HAL_I2C_Mem_Write(&hi2c1 ,dev, reg ,I2C_MEMADD_SIZE_8BIT, &writeval ,1,0xffff);

}

uint8_t i2c_ReadReg(uint8_t dev,uint8_t val){
	static uint8_t readval ;
	HAL_I2C_Mem_Read(&hi2c1 ,dev, val, I2C_MEMADD_SIZE_8BIT , &readval , 1,0xffff);
	return readval;
}
void i2c_ReadMuti(uint8_t dev ,uint8_t len , uint8_t reg ,uint8_t  * Buffer){
	HAL_I2C_Mem_Read(&hi2c1 ,dev, reg, I2C_MEMADD_SIZE_8BIT , Buffer , len,0xffff);
	/*int i = 0;
	for(i = 0;i<len;i++)
	{
		Buffer[i] = i2c_ReadReg(dev,reg+i);
	}*/
}
