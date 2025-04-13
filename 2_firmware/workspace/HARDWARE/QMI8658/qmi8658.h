#ifndef __QMI8658_H
#define __QMI8658_H


/****
* �Ĵ����б�
*/
//General Purpose Registers
#define WHO_AM_I        0x00
#define REVISION_ID     0x01
//Setup and Control Registers
#define CTRL1           0x02
#define CTRL2           0x03
#define CTRL3           0x04
#define CTRL4           0x05
#define CTRL5           0x06
#define CTRL6           0x07
#define CTRL7           0x08
#define CTRL8           0x09
#define CTRL9           0x0A
//Host Controlled Calibration Registers(See CTRL9, Usage is Optional)
#define CAL1_L          0x0B
#define CAL1_H          0x0C
#define CAL2_L          0x0D
#define CAL2_H          0x0E
#define CAL3_L          0x0F
#define CAL3_H          0x10
#define CAL4_L          0x11
#define CAL4_H          0x12
//FIFO Registers
#define FIFO_WTM_TH     0x13
#define FIFO_CTRL       0x14
#define FIFO_SMPL_CNT   0x15
#define FIFO_STATUS     0x16
#define FIFO_DATA       0x17
//Status Registers
#define STATUSINT       0x2D
#define STATUS0         0x2E
#define STATUS1         0x2F
//Timestamp Register
#define TIMESTAMP_LOW   0x30
#define TIMESTAMP_MID   0x31
#define TIMESTAMP_HIGH  0x32
//Data Output Registers (16 bits 2��s Complement Except Self-Test Sensor Data, AE-CLIP and AE_OVFLOW)
#define TEMP_L          0x33
#define TEMP_H          0x34

#define AX_L            0x35
#define AX_H            0x36
#define AY_L            0x37
#define AY_H            0x38
#define AZ_L            0x39
#define AZ_H            0x3A
#define GX_L            0x3B
#define GX_H            0x3C
#define GY_L            0x3D
#define GY_H            0x3E
#define GZ_L            0x3F
#define GZ_H            0x40

#define dQW_L           0x49
#define dQW_H           0x4A
#define dQX_L           0x4B
#define dQX_H           0x4C
#define dQY_L           0x4D
#define dQY_H           0x4E
#define dQZ_L           0x4F
#define dQZ_H           0x50
#define dVX_L           0x51
#define dVX_H           0x52
#define dVY_L           0x53
#define dVY_H           0x54
#define dVZ_L           0x55
#define dVZ_H           0x56
#define AE_REG1         0x57
#define AE_REG2         0x58
#define RESET_REG       0x60
//Reset Register



void QMI8658_Init(void);
void QMI8658_dataGet(void);

#endif
