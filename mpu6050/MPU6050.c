#include "main.h"
#include "i2c.h"
#include "MPU6050.h"
//#include "Debug.h"
#include <stdio.h>
#include <math.h>
#include "string.h"

extern UART_HandleTypeDef huart1;

float Yaw_Angle_C = 0, Pitch_Angle_C = 0, Roll_Angle_C = 0;
float yaw_a = 0, pitch_a = 0, roll_a = 0;
float yaw_g = 0, pitch_g = 0, roll_g = 0;
uint8_t Read_buf[2] = {0};

infor MPU6050;

void __MPU6050_Write_Reg(uint16_t Reg_address, uint8_t data)
{
    HAL_I2C_Mem_Write(&MPU6050_IIC, MPU6050_Address << 1, Reg_address, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
}

uint8_t __MPU6050_Read_Reg(uint8_t Reg_address)
{
    uint8_t data;
    HAL_I2C_Mem_Read(&MPU6050_IIC, MPU6050_Address << 1, Reg_address, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
    return data;
}

void __MPU6050_Read_Reg_Continue(uint8_t Reg_address, uint8_t *buffer)
{
	HAL_I2C_Mem_Read(&MPU6050_IIC, MPU6050_Address << 1, Reg_address, I2C_MEMADD_SIZE_8BIT, buffer, 2, 100);
}

void MPU6050_Init(void)
{
	__MPU6050_Write_Reg(MPU6050_PWR_MGMT_1, 0x01);
	__MPU6050_Write_Reg(MPU6050_PWR_MGMT_2, 0x00);
	__MPU6050_Write_Reg(MPU6050_SMPLRT_DIV, 0x09);
	__MPU6050_Write_Reg(MPU6050_CONFIG, 0x06);
	__MPU6050_Write_Reg(MPU6050_GYRO_CONFIG, 0x18);
	__MPU6050_Write_Reg(MPU6050_ACCEL_CONFIG, 0x18);
}

//void MPU6050_Call(void)
//{
//    uint8_t temp;
//    temp = __MPU6050_Read_Reg(MPU6050_WHO_AM_I);
//    if ((temp & 0x68) == 0x68) {
////        printf("MPU6050 OK\r\n");
//			 char msg[] = "MPU6050 OK\r\n";
//        HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
//    } else {
//       // printf("MPU6050 can't find\r\n");
//        char msg[] = "MPU6050 can't find\r\n";
//        HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
//    }
//}

void MPU6050_Call(void)
{
    uint8_t temp;
    char msg[100];  // 缓冲区用于格式化输出
    
    // 读取WHO_AM_I寄存器值
    temp = __MPU6050_Read_Reg(MPU6050_WHO_AM_I);
    
    // 格式化temp值为十六进制字符串并发送
    sprintf(msg, "MPU6050 WHO_AM_I = 0x%02X\r\n", temp);
    HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
    
    // 判断传感器是否正常连接
    if ((temp & 0x68) == 0x68) {
        char ok_msg[] = "MPU6050 OK\r\n";
        HAL_UART_Transmit(&huart1, (uint8_t*)ok_msg, strlen(ok_msg), HAL_MAX_DELAY);
    } else {
        char err_msg[] = "MPU6050 can't find\r\n";
        HAL_UART_Transmit(&huart1, (uint8_t*)err_msg, strlen(err_msg), HAL_MAX_DELAY);
    }
}

void MPU6050_GetData(infor* temp)
{
    int16_t data_h, data_l;
	
	data_h = __MPU6050_Read_Reg(MPU6050_ACCEL_XOUT_H);
	data_l = __MPU6050_Read_Reg(MPU6050_ACCEL_XOUT_L);
	(*temp).Acc_X = ((data_h << 8) | data_l) /*+ 1041*/;
	
	data_h = __MPU6050_Read_Reg(MPU6050_ACCEL_YOUT_H);
	data_l = __MPU6050_Read_Reg(MPU6050_ACCEL_YOUT_L);
	(*temp).Acc_Y = ((data_h << 8) | data_l) /*- 78*/;
	
	data_h = __MPU6050_Read_Reg(MPU6050_ACCEL_ZOUT_H);
	data_l = __MPU6050_Read_Reg(MPU6050_ACCEL_ZOUT_L);
	(*temp).Acc_Z = ((data_h << 8) | data_l) /*- 1740*/;
    (*temp).Acc_Z = (float)(*temp).Acc_Z /* 1.7*/;
	
	data_h = __MPU6050_Read_Reg(MPU6050_GYRO_XOUT_H);
	data_l = __MPU6050_Read_Reg(MPU6050_GYRO_XOUT_L);
	(*temp).Gyro_X = ((data_h << 8) | data_l) + 45;			//抵消零飘
	
	data_h = __MPU6050_Read_Reg(MPU6050_GYRO_YOUT_H);
	data_l = __MPU6050_Read_Reg(MPU6050_GYRO_YOUT_L);
	(*temp).Gyro_Y = ((data_h << 8) | data_l) + 15;		//抵消零飘
	
	data_h = __MPU6050_Read_Reg(MPU6050_GYRO_ZOUT_H);
	data_l = __MPU6050_Read_Reg(MPU6050_GYRO_ZOUT_L);
	(*temp).Gyro_Z = ((data_h << 8) | data_l) - 48;		//抵消零飘

	//printf("%d, %d, %d\r\n", (*temp).Acc_X, (*temp).Acc_Y, (*temp).Acc_Z/*, (*temp).Gyro_X, (*temp).Gyro_Y, (*temp).Gyro_Z*/);
	//printf("%d, %d, %d\r\n", /*(*temp).Acc_X, (*temp).Acc_Y, (*temp).Acc_Z,*/ (*temp).Gyro_X, (*temp).Gyro_Y, (*temp).Gyro_Z);
}

void MPU6050_AttitudeEstimation_G(infor MPU6050)
{
	Pitch_Angle_C = Pitch_Angle_C - 0.01 * ((float)MPU6050.Gyro_Y / 16.4);
	Roll_Angle_C = Roll_Angle_C + 0.01 * ((float)MPU6050.Gyro_X / 16.4);
	Yaw_Angle_C = Yaw_Angle_C + 0.01 * ((float)MPU6050.Gyro_Z / 16.4);

	printf("%f, %f, %f\r\n", Pitch_Angle_C, Roll_Angle_C, Yaw_Angle_C);
}

void MPU6050_AttitudeEstimation_A(infor MPU6050)
{
	Yaw_Angle_C = 0;
	Pitch_Angle_C = atan2((float)MPU6050.Acc_X, (float)MPU6050.Acc_Z) / Pi * 180.0;
	Roll_Angle_C = atan2((float)MPU6050.Acc_Y, (float)MPU6050.Acc_Z) / Pi * 180.0;

	printf("%f, %f, %f\r\n", Pitch_Angle_C, Roll_Angle_C, Yaw_Angle_C);
}

void MPU6050_AttitudeEstimation(infor MPU6050)
{
	static float pre_yaw = 0;
	static float pre_roll=0;
	static float pre_pitch=0;
	
	pitch_g = Pitch_Angle_C - 0.01 * ((float)MPU6050.Gyro_Y / 16.4);
	roll_g = Roll_Angle_C + 0.01 * ((float)MPU6050.Gyro_X / 16.4);
	yaw_g = Yaw_Angle_C + 0.01 * ((float)MPU6050.Gyro_Z / 11.4);
	
	pitch_a = atan2((float)MPU6050.Acc_X, (float)MPU6050.Acc_Z) / Pi * 180.0;
	roll_a = atan2((float)MPU6050.Acc_Y, (float)MPU6050.Acc_Z) / Pi * 180.0;
	
	
	float deta_yaw = fabs(yaw_g - pre_yaw);
	float deta_roll=fabs(roll_g-pre_roll);
	float deta_pitch=fabs(pitch_g-pre_pitch);

//防止yaw零飘
	if (deta_yaw > 0.1) {
		Yaw_Angle_C = yaw_g;		
	} else {
		Yaw_Angle_C = pre_yaw;
	}
	//Yaw_Angle_C = yaw_g;
	
//防止roll零飘
	 if(deta_roll>0.1){
		Roll_Angle_C=roll_g;
	 }	
	 else{
		Roll_Angle_C=pre_roll;
	 }
	 
	 //防止pitch零飘
	 if(deta_pitch>0.13){
		Pitch_Angle_C=pitch_g;
	 }
	 else{
		Pitch_Angle_C=pre_pitch;
	 }

	 //Pitch_Angle_C = 0.9091 * pitch_a + 0.0909 * pitch_g - 2.1;
	//Roll_Angle_C = 0.9091 * roll_a + 0.0909 * roll_g + 0.7;
	Pitch_Angle_C = 0.95238 * pitch_g + 0.04762 * pitch_a;
	Roll_Angle_C = 0.95238 * roll_g + 0.04762 * roll_a;
	 
	pre_yaw = Yaw_Angle_C;
	pre_roll=Roll_Angle_C;
	 pre_pitch=Pitch_Angle_C;
	 Roll_Angle_C -= 0.01;
    //printf("%f, %f, %f\r\n", -Pitch_Angle_C, Roll_Angle_C, Yaw_Angle_C);
}

//0.95238, 0.04762
//0.90909, 0.09091
