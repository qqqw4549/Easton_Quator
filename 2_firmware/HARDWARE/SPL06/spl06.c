#include "spl06.h"
#include "myiic.h"
#include "usart.h"



//#define SPL06_DEV 0x3B
//#define SPL06_DEV 0x76
#define SPL06_DEV 0xEC

void SPL06_Init()
{
	uint8_t uChipID = 0;
    // wait stable
    i2c_WriteReg(SPL06_DEV,0xE0,0xB6);
    HAL_Delay(300);

    while(uChipID!= 0x58){ 
        // 1.Device Reset & check Device
        uChipID = i2c_ReadReg(SPL06_DEV,0xd0); // Read ID
        HAL_Delay(200);        // device check
    }
    printf("BMP280 check Pass!:deviceID = 0x%x \r\n",uChipID);
    
    //2.Device Settings
}
