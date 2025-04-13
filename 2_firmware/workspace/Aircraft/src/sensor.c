
/*
*		editor : wangld
*		file:	sensor.c
*		date: 2024/04/05
*/

#include "sensor.h"
#include "qmi8658.h"
#include "qmc5883.h"
#include "spl06.h"
#include "math.h"
#include "usart.h"
#include "filter.h"
#include "myiic.h"

#define LOOPBUFFER 1024


struct stAxis gstGyroBias;
struct stSensorRaw gstSenSorRaw;
struct stAxis gstGyroVar;
struct stAxis gstGyroMean;
struct stSensor gstSensor;
struct stQuater gstQuator;
float gAccScale = 1;
struct stGyroBiasBuffer gstGyroBiasBuffer;
struct stStatus gstStatus;
uint8_t gCaliberation = 0;
static float accScaleCalSum = 0;

//计算相关
static float rMat[3][3];	//旋转矩阵
static struct stAxis gstIntErrAxis;	//误差累计
float gKp = 0.4;	//比例增益
float gKi = 0.001;	//积分系数
float gDeltaT = 0.004; //采样周期
static float q0 = 1.0f;	/*四元数*/
static float q1 = 0.0f;
static float q2 = 0.0f;
static float q3 = 0.0f;	



/*低通滤波参数*/
#define GYRO_LPF_CUTOFF_FREQ  80
#define ACCEL_LPF_CUTOFF_FREQ 30
static lpf2pData accLpf[3];
static lpf2pData gyroLpf[3];


struct stGyroBiasBuffer // bias buffer
{
	uint8_t Filled;
	uint8_t FindBias;
    struct stGyro * header;
    struct stGyro buffer[LOOPBUFFER];
};

// 快速开平方求倒
// See: http://en.wikipedia.org/wiki/Fast_inverse_square_root
float invSqrt(float x)
{
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}

void Init_AccGryoFilter(void)
{
	uint8_t i = 0;
	for (i = 0; i < 3; i++)// 初始化加速计和陀螺二阶低通滤波
	{
		lpf2pInit(&gyroLpf[i], 1000, GYRO_LPF_CUTOFF_FREQ);
		lpf2pInit(&accLpf[i],  1000, ACCEL_LPF_CUTOFF_FREQ);
	}
}

void GyroBufferInit(void)
{
    gstGyroBiasBuffer.Filled = 0;
    gstGyroBiasBuffer.header = gstGyroBiasBuffer.buffer;
}

uint8_t imu_FillGyroBuffer(struct stGyro GyroRaw)
{
		//1、填充缓冲
		gstGyroBiasBuffer.header->gx =  GyroRaw.gx;
		gstGyroBiasBuffer.header->gy =  GyroRaw.gy;
		gstGyroBiasBuffer.header->gz =  GyroRaw.gz;
		gstGyroBiasBuffer.header++;
		
		//2、边界判断
		if(gstGyroBiasBuffer.header >= &gstGyroBiasBuffer.buffer[LOOPBUFFER])
		{
				gstGyroBiasBuffer.Filled = 1;
				gstGyroBiasBuffer.header = gstGyroBiasBuffer.buffer;
		}
		return gstGyroBiasBuffer.Filled;
}

uint8_t imu_GetGyroBias(struct stGyro GyroRaw)
{ 
		//1、fill buffer
		imu_FillGyroBuffer(GyroRaw);
		//2、find Gyro Bias
		if(!gstGyroBiasBuffer.FindBias)
		{
				if(gstGyroBiasBuffer.Filled)
				{
						signed long gx=0,gy=0,gz=0,gx2=0,gy2=0,gz2=0;
						int i=0;
						for(i = 0 ; i < LOOPBUFFER ; i++)
						{
                            gx += gstGyroBiasBuffer.buffer[i].gx;
                            gy += gstGyroBiasBuffer.buffer[i].gy;
                            gz += gstGyroBiasBuffer.buffer[i].gz;
                        
                            gx2+=gstGyroBiasBuffer.buffer[i].gx * gstGyroBiasBuffer.buffer[i].gx;
                            gy2+=gstGyroBiasBuffer.buffer[i].gy * gstGyroBiasBuffer.buffer[i].gy;
                            gz2+=gstGyroBiasBuffer.buffer[i].gz * gstGyroBiasBuffer.buffer[i].gz;
						}
						
						gstGyroVar.x = (gx2 - ( signed long)(gx*gx)/LOOPBUFFER)/LOOPBUFFER;
						gstGyroVar.y = (gy2 - ( signed long)(gy*gy)/LOOPBUFFER)/LOOPBUFFER;
						gstGyroVar.z = (gz2 - ( signed long)(gz*gz)/LOOPBUFFER)/LOOPBUFFER;
						
						gstGyroMean.x = (float)gx/LOOPBUFFER;
						gstGyroMean.y = (float)gy/LOOPBUFFER;
						gstGyroMean.z = (float)gz/LOOPBUFFER;
						
						if(gstGyroVar.x < VAR_THREHOLD_ZERO && gstGyroVar.y < VAR_THREHOLD_ZERO && gstGyroVar.z < VAR_THREHOLD_ZERO)
						{
								//3、bias founded!
								gstGyroBias.x = gstGyroMean.x;
								gstGyroBias.y = gstGyroMean.y;
								gstGyroBias.z = gstGyroMean.z;
								gstGyroBiasBuffer.FindBias = 1;
						}else {
								gstGyroBiasBuffer.Filled = 0;
						}
				}
		}
		return gstGyroBiasBuffer.FindBias;
}

/*二阶低通滤波*/
static void applyAxis3fLpf(lpf2pData *data, struct stAxis * in)
{
    in->x = lpf2pApply(&data[0], in->x);
    in->y = lpf2pApply(&data[1], in->y);
    in->z = lpf2pApply(&data[2], in->z);
}


void sensorsTask(void *pvParameters)
{
    long cnt=0;
    QMI8658_Init();//初始化陀螺仪以及加速度计
    QMC5883_Init();//初始化电子罗盘
    //SPL06_Init();//初始化气压计
	while(1)
	{
			//1、传感器获取原始数据
			QMI8658_dataGet();
		   /* printf("INITf:%d %d %d %d %d %d \r\n", \
(s16)gstSenSorRaw.AccRaw.ax,(s16)gstSenSorRaw.AccRaw.ay,(s16)gstSenSorRaw.AccRaw.az,\
(s16)gstSenSorRaw.GyroRaw.gx,(s16)gstSenSorRaw.GyroRaw.gy,(s16)gstSenSorRaw.GyroRaw.gz  );*/
			//2、获取传感器偏置
			uint8_t uGyroBiasFound = imu_GetGyroBias(gstSenSorRaw.GyroRaw);
			//3、获取传感器实际测量值
			if(uGyroBiasFound)
			{
					static uint32_t accScaleCnt = 0;
					accScaleCalSum += sqrtf(powf(gstSenSorRaw.AccRaw.ax,2)+powf(gstSenSorRaw.AccRaw.ay,2)+powf(gstSenSorRaw.AccRaw.az,2));
					accScaleCnt++;
					if(accScaleCnt >= ACC_SCALE_NUM)
					{
							gAccScale = accScaleCalSum / ACC_SCALE_NUM;
						  accScaleCnt = 0;
							accScaleCalSum = 0;
					}

					//加速度 g
					gstSensor.Acc.x = -(gstSenSorRaw.AccRaw.ax)*9.8/2048/gAccScale;
					gstSensor.Acc.y =  (gstSenSorRaw.AccRaw.ay)*9.8/2048/gAccScale;
					gstSensor.Acc.z =  (gstSenSorRaw.AccRaw.az)*9.8/2048/gAccScale;
					applyAxis3fLpf(gyroLpf, &gstSensor.Acc);
					//弧度 rad
					gstSensor.Gyro.x = -(gstSenSorRaw.GyroRaw.gx - gstGyroMean.x)*0.000027;
					gstSensor.Gyro.y =  (gstSenSorRaw.GyroRaw.gy - gstGyroMean.y)*0.000027;
					gstSensor.Gyro.z =  (gstSenSorRaw.GyroRaw.gz - gstGyroMean.z)*0.000027;
					applyAxis3fLpf(gyroLpf, &gstSensor.Gyro);


			float normalize=0;
			struct stAxis stErrAxis;

			// 当传感器数值为有效值时才处理
			if((gstSensor.Acc.x!=0.0f) || (gstSensor.Acc.z!=0.0f) || (gstSensor.Acc.z!=0.0f))
			{
					//单位化加速度值
					normalize = invSqrt(gstSensor.Acc.x * gstSensor.Acc.x + gstSensor.Acc.y * gstSensor.Acc.y + gstSensor.Acc.z * gstSensor.Acc.z);
					gstSensor.Acc.x *= normalize;
					gstSensor.Acc.y *= normalize;
					gstSensor.Acc.z *= normalize;
				
					//向量叉乘运算
					stErrAxis.x = gstSensor.Acc.y * rMat[2][2] - gstSensor.Acc.z * rMat[2][1];
					stErrAxis.y = gstSensor.Acc.z * rMat[2][0] - gstSensor.Acc.x * rMat[2][2];
					stErrAxis.z = gstSensor.Acc.x * rMat[2][1] - gstSensor.Acc.y * rMat[2][0];
				
					//误差累计积分
					gstIntErrAxis.x += gKi * stErrAxis.x * gDeltaT;
					gstIntErrAxis.y += gKi * stErrAxis.y * gDeltaT;
					gstIntErrAxis.z += gKi * stErrAxis.z * gDeltaT;
				
					//用叉积误差来做PI修正陀螺零偏，即抵消陀螺读数中的偏移量
					gstSensor.Gyro.x += gKp * stErrAxis.x + gstIntErrAxis.x;
					gstSensor.Gyro.y += gKp * stErrAxis.y + gstIntErrAxis.y;
					gstSensor.Gyro.z += gKp * stErrAxis.z + gstIntErrAxis.z;
			}

			float q0Last = q0;
			float q1Last = q1;
			float q2Last = q2;
			float q3Last = q3;
			q0 += (-q1Last * gstSensor.Gyro.x - q2Last * gstSensor.Gyro.y - q3Last * gstSensor.Gyro.z) * gDeltaT/2;
			q1 += ( q0Last * gstSensor.Gyro.x + q2Last * gstSensor.Gyro.z - q3Last * gstSensor.Gyro.y) * gDeltaT/2;
			q2 += ( q0Last * gstSensor.Gyro.y - q1Last * gstSensor.Gyro.z + q3Last * gstSensor.Gyro.x) * gDeltaT/2;
			q3 += ( q0Last * gstSensor.Gyro.z + q1Last * gstSensor.Gyro.y - q2Last * gstSensor.Gyro.x) * gDeltaT/2;

			//单位化四元数
			normalize = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
			q0 *= normalize;
			q1 *= normalize;
			q2 *= normalize;
			q3 *= normalize;

			//计算旋转矩阵
			rMat[0][0] = 1.0f - 2.0f * q2*q2 - 2.0f * q3*q3;
			rMat[0][1] = 2.0f * (q1*q2 + (-q0)*q3);
			rMat[0][2] = 2.0f * (q1*q3 - (-q0)*q2);

			rMat[1][0] = 2.0f * (q1*q2 - (-q0)*q3);
			rMat[1][1] = 1.0f - 2.0f * q1*q1 - 2.0f * q3*q3;
			rMat[1][2] = 2.0f * (q2*q3 + (-q0)*q1);

			rMat[2][0] = 2.0f * (q1*q3 + (-q0)*q2);
			rMat[2][1] = 2.0f * (q2*q3 - (-q0)*q1);
			rMat[2][2] = 1.0f - 2.0f * q1*q1 - 2.0f * q2*q2;

			//欧拉角
			gstStatus.pitch = -asinf(rMat[2][0]) * 57.29578f;
			gstStatus.roll = atan2f(rMat[2][1], rMat[2][2]) * 57.29578f;
			gstStatus.yaw = atan2f(rMat[1][0], rMat[0][0]) * 57.29578f;

			cnt++;
			//if(cnt%500==0)
			//printf("%f %f %f\r\n",gstStatus.pitch,gstStatus.roll,gstStatus.yaw);
			//delay_xms(10);

			
		}
			/*#if 0
			if(!gCaliberation)
			{
					accBuf[2] = gstSensor.Acc.x* rMat[2][0] + gstSensor.Acc.y * rMat[2][1] + gstSensor.Acc.z * rMat[2][2];  // Z - AXIS
			}
			#endif*/
	}
}
