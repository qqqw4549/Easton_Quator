
/*
*		editor : wangld
*		file:	sensor.h
*		date: 2024/04/05
*/

#ifndef __SENSOR_H
#define __SENSOR_H
#include <stdint.h>

#define VAR_THREHOLD_ZERO 30000
#define ACC_SCALE_NUM 200

struct stAxis{
    float x;
    float y;
    float z;
};

struct stGyro{
    int16_t gx;
    int16_t gy;
    int16_t gz;
};
struct stAcc{
    int16_t ax;
    int16_t ay;
    int16_t az;
};

struct stSensorRaw{
		struct stAcc AccRaw;
		struct stGyro GyroRaw;
};
struct stSensor{
		struct stAxis Acc;
		struct stAxis Gyro;
};
struct stQuater
{
		float q0;
		float q1;
		float q2;
		float q3;
};

struct stStatus
{
		float pitch;
		float roll;
		float yaw;
};



extern struct stSensorRaw gstSenSorRaw;
float invSqrt(float x);
uint8_t imu_GetGyroBias(struct stGyro GyroRaw);
void GyroBufferInit(void);
void sensorsTask(void);
void Init_AccGryoFilter(void);
#endif

