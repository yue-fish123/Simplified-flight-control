#ifndef __CONTROL_H_
#define __CONTROL_H_

#include "stm32g0xx_hal.h"

// 无人机控制状态结构体（记录蓝牙指令对应的运动状态）
typedef struct {
    uint8_t forward  : 1;   // 前进（B键）
    uint8_t backward : 1;   // 后退（E键）
    uint8_t left     : 1;   // 左移（D键）
    uint8_t right    : 1;   // 右移（F键）
    int32_t throttle;        // 油门（0-1000，I/G/K控制）
    int32_t yaw_offset;      // 航向角偏移（-5~5，H/J控制）
} DroneControlState;

// 全局变量声明
extern DroneControlState drone_state;
extern float Roll_Angle_C, Pitch_Angle_C, Yaw_Angle_C;
extern float Roll_Angle_Ex, Pitch_Angle_Ex, Yaw_Angle_Ex;
extern float Roll_P, Roll_I, Roll_D, Roll_Pout, Roll_Iout, Roll_Dout, Roll_Out;
extern float Pitch_P, Pitch_I, Pitch_D, Pitch_Pout, Pitch_Iout, Pitch_Dout, Pitch_Out;
extern float Yaw_P, Yaw_I, Yaw_D, Yaw_Pout, Yaw_Iout, Yaw_Dout, Yaw_Out;
extern float Old_Roll, Old_Pitch, Old_Yaw;
extern uint16_t motor1, motor2, motor3, motor4;
extern uint16_t THROTTLE;
extern uint32_t cal_imu;
extern const uint16_t Moto_PwmMax;

// 函数声明
void Pid_init(void);
void CONTROL(float Cur_Roll, float Cur_Pitch, float Cur_Yaw, float Ex_Roll, float Ex_Pitch, float Ex_Yaw);
void Moto_PwmRflash(int16_t MOTO1_PWM, int16_t MOTO2_PWM, int16_t MOTO3_PWM, int16_t MOTO4_PWM);
void Bluetooth_Control_Loop(void);
void Update_Control_Input(void);
void handle_command(uint8_t data);

#endif
