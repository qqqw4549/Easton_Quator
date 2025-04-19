/*
*		editor : wangld
*		file:	qmi8658.c
*		date: 2024/04/05
*/

    #include "qmi8658.h"

    #include "myiic.h"
    #include "usart.h"

    #include "sensor.h"

    #define QMI8658_DEV 0xD5
    #define SPL06_DEV 0xEC
    #define QMC5883_DEV 0x1A



union QMI8658_Data{
    uint8_t buffer[12];
    struct {
    	int16_t AX;
            int16_t AY;
            int16_t AZ;
            int16_t GX;
            int16_t GY;
            int16_t GZ;
        }stData;
} uQMI8658_Data;



void QMI8658_Init()
{
    uint8_t uChipID = 0;
    
    // 1.Device Reset & check Device
    i2c_WriteReg(QMI8658_DEV,CTRL9,0xA2); //PowerReset
    HAL_Delay(200);                           // wait stable
    while(uChipID != 0x5) 
	{
    	HAL_Delay(200);        // device check
		uChipID = i2c_ReadReg(QMI8658_DEV,0x00); // Read ID
	}
		
    printf("QMI8658 check Pass!\r\n");
    
    //2.Device Settings
    i2c_WriteReg(QMI8658_DEV,CTRL1,0x40); //��ַ������С��
    i2c_WriteReg(QMI8658_DEV,CTRL2,0x33); //���ü��ٶȼ� ����+-16g(2048)��������ODR==1kHZ
    i2c_WriteReg(QMI8658_DEV,CTRL3,0x53); //����������  ���̡�512 dps(64) ,��������940HZ
    //i2c_WriteReg(QMI8658_DEV,CTRL5,0x0); //��ʹ�õ�ͨ�˲���
    i2c_WriteReg(QMI8658_DEV,CTRL5,0x77); //ʹ�õ�ͨ�˲���  13.37% ��ODR
    i2c_WriteReg(QMI8658_DEV,CTRL6,0x0); //��ʹ��Motion on Demand
    i2c_WriteReg(QMI8658_DEV,CTRL7,0x03); // ʹ�ܼ��ٶȼƺ�������
}

void QMI8658_dataGet(void)
{
    i2c_ReadMuti(QMI8658_DEV,12,AX_L,uQMI8658_Data.buffer);
    gstSenSorRaw.AccRaw.ax = uQMI8658_Data.stData.AX;
	gstSenSorRaw.AccRaw.ay = uQMI8658_Data.stData.AY;
	gstSenSorRaw.AccRaw.az = uQMI8658_Data.stData.AZ;
	gstSenSorRaw.GyroRaw.gx  = uQMI8658_Data.stData.GX;
	gstSenSorRaw.GyroRaw.gy  = uQMI8658_Data.stData.GY;
	gstSenSorRaw.GyroRaw.gz  = uQMI8658_Data.stData.GZ;
/*
    uQMI8658_Data.buffer[0] = i2c_ReadReg(QMI8658_DEV,AX_L);
    uQMI8658_Data.buffer[1] = i2c_ReadReg(QMI8658_DEV,AX_L+1);
    uQMI8658_Data.buffer[2] = i2c_ReadReg(QMI8658_DEV,AX_L+2);
    uQMI8658_Data.buffer[3] = i2c_ReadReg(QMI8658_DEV,AX_L+3);
    uQMI8658_Data.buffer[4] = i2c_ReadReg(QMI8658_DEV,AX_L+4);
    uQMI8658_Data.buffer[5] = i2c_ReadReg(QMI8658_DEV,AX_L+5);
    uQMI8658_Data.buffer[6] = i2c_ReadReg(QMI8658_DEV,AX_L+6);
    uQMI8658_Data.buffer[7] = i2c_ReadReg(QMI8658_DEV,AX_L+7);
    uQMI8658_Data.buffer[8] = i2c_ReadReg(QMI8658_DEV,AX_L+8);
    uQMI8658_Data.buffer[9] = i2c_ReadReg(QMI8658_DEV,AX_L+9);
    uQMI8658_Data.buffer[10] = i2c_ReadReg(QMI8658_DEV,AX_L+10);
    uQMI8658_Data.buffer[11] = i2c_ReadReg(QMI8658_DEV,AX_L+11);*/

    /*#if 0
    printf("%d %d %d %d %d %d ", \
    (int16_t)uQMI8658_Data.stData.AX,(int16_t)uQMI8658_Data.stData.AY,(int16_t)uQMI8658_Data.stData.AZ,\
    (int16_t)uQMI8658_Data.stData.GX,(int16_t)uQMI8658_Data.stData.GY,(int16_t)uQMI8658_Data.stData.GZ  );
		#endif

		gstSenSorRaw.AccRaw.ax = uQMI8658_Data.stData.AX;
		gstSenSorRaw.AccRaw.ay = uQMI8658_Data.stData.AY;
		gstSenSorRaw.AccRaw.az = uQMI8658_Data.stData.AZ;
		gstSenSorRaw.GyroRaw.gx  = uQMI8658_Data.stData.GX;
		gstSenSorRaw.GyroRaw.gy  = uQMI8658_Data.stData.GY;
		gstSenSorRaw.GyroRaw.gz  = uQMI8658_Data.stData.GZ;

		#ifdef TEST_MODE	
		printf("[QMI8658]:AX=%d AY=%d AZ=%d GX=%d GY=%d GZ=%d \r\n", \
    (int16_t)uQMI8658_Data.stData.AX,(int16_t)uQMI8658_Data.stData.AY,(int16_t)uQMI8658_Data.stData.AZ,\
    (int16_t)uQMI8658_Data.stData.GX,(int16_t)uQMI8658_Data.stData.GY,(int16_t)uQMI8658_Data.stData.GZ  );
		#endif*/
}
