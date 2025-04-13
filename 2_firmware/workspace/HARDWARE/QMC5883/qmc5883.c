
#include "qmc5883.h"
#include "myiic.h"
#include "usart.h"



//#define QMC5883_DEV 0x0D
#define QMC5883_DEV 0x1A

union QMC5883_Data{
	uint8_t buffer[6];
    struct {
            int16_t AX;
            int16_t AY;
            int16_t AZ;
        }stData;
} uQMC5883_Data;

void QMC5883_Init()
{
    uint8_t uChipID = 0,ready =0;
   
    // 1.Device Reset & check Device  -32768 and 32767
    //i2c_WriteReg(QMC5883_DEV,0x0A,0x80); //PowerReset
    //delay_ms(100);
    i2c_WriteReg(QMC5883_DEV,0x0B,0x01); //PowerReset
    i2c_WriteReg(QMC5883_DEV,0x20,0x40); //PowerReset
    i2c_WriteReg(QMC5883_DEV,0x21,0x01); //PowerReset
    //i2c_WriteReg(QMC5883_DEV,0x09,0x0d); // 512 OSR 2G ODR 200 
    i2c_WriteReg(QMC5883_DEV,0x09,0x89); //OSR 128 2G ODR 100
    HAL_Delay(200);                           // wait stable
    while( uChipID!= 0xFF) 
    {   
        uChipID = i2c_ReadReg(QMC5883_DEV,0x0D); // Read ID
        HAL_Delay(200);        // device check
    }
    printf("QMC5883 check Pass!:deviceID = 0x%x \r\n",uChipID);
}

void QMC5883_dataGet(void)
{
    i2c_ReadMuti(QMC5883_DEV,6,0x0,&uQMC5883_Data.buffer);
		#ifdef TEST_MODE	
    printf("[QMI8658]:AX=%d AY=%d AZ=%d \r\n", \
    (int16_t)(uQMC5883_Data.stData.AX), (int16_t)(uQMC5883_Data.stData.AY), (int16_t)(uQMC5883_Data.stData.AZ));
		#endif
}
