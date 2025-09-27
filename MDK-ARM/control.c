#include "stm32g0xx.h"                  // Device header
#include "control.h"
#include "MPU6050.h"
#include "soft_uart.h"

// 全局变量定义
DroneControlState drone_state = {0};
//float Roll_Angle_C = 0, Pitch_Angle_C = 0, Yaw_Angle_C = 0;
float Roll_Angle_Ex = 0, Pitch_Angle_Ex = 0, Yaw_Angle_Ex = 0;
float Roll_P = 0, Roll_I = 0, Roll_D = 0, Roll_Pout = 0, Roll_Iout = 0, Roll_Dout = 0, Roll_Out = 0;
float Pitch_P = 0, Pitch_I = 0, Pitch_D = 0, Pitch_Pout = 0, Pitch_Iout = 0, Pitch_Dout = 0, Pitch_Out = 0;
float Yaw_P = 0, Yaw_I = 0, Yaw_D = 0, Yaw_Pout = 0, Yaw_Iout = 0, Yaw_Dout = 0, Yaw_Out = 0;
float Old_Roll = 0, Old_Pitch = 0, Old_Yaw = 0;
uint16_t motor1 = 0, motor2 = 0, motor3 = 0, motor4 = 0;
uint16_t THROTTLE = 0;
uint32_t cal_imu = 0;
const uint16_t Moto_PwmMax = 1000;  // 电机最大PWM值

// PID参数初始化
void Pid_init(void) {
    Roll_P = 2.4f;
    Roll_I = 0.0f;
    Roll_D = 100.0f;
    
    Pitch_P = 2.4f;
    Pitch_I = 0.0f;
    Pitch_D = 100.0f;
    
    Yaw_P = 2.0f;
    Yaw_I = 0.0f;
    Yaw_D = 10.0f;
    
    Roll_Pout = 0;
    Roll_Iout = 0;
    Roll_Dout = 0;
    
    Pitch_Pout = 0;
    Pitch_Iout = 0;
    Pitch_Dout = 0;
    
    Yaw_Pout = 0;
    Yaw_Iout = 0;
    Yaw_Dout = 0;
}

// 将蓝牙状态转换为控制量（期望角度和油门）
void Update_Control_Input(void) {
    if (drone_state.throttle > 0) {
        THROTTLE = drone_state.throttle;
    }
    //如果 throttle 被设为 0（停止），也可以处理
    else if (drone_state.throttle == 0) {
         THROTTLE = 0;
    }

    //2. 读取方向：前进/后退 → 控制俯仰角（Pitch）
    if (drone_state.forward) {
        Pitch_Angle_Ex = -10;  // 负值 = 前倾（前进）
    } else if (drone_state.backward) {
        Pitch_Angle_Ex = 10;   // 正值 = 后倾（后退）
    } else {
        Pitch_Angle_Ex = 0;    // 没有前后指令，保持水平
    }

    //3. 读取左右移动 → 控制横滚角（Roll）
    if (drone_state.left) {
        Roll_Angle_Ex = -10;   // 负值 = 左倾（左移）
    } else if (drone_state.right) {
        Roll_Angle_Ex = 10;    // 正值 = 右倾（右移）
    } else {
        Roll_Angle_Ex = 0;     // 没有左右指令，保持水平
    }

    // 4. 读取偏航 → 控制航向角变化率
    Yaw_Angle_Ex = drone_state.yaw_offset;  // -3 或 +3
}

// 电机PWM输出函数
void Moto_PwmRflash(int16_t MOTO1_PWM, int16_t MOTO2_PWM, int16_t MOTO3_PWM, int16_t MOTO4_PWM) {
	//uart_send_string("PWM: ");
   // uart_send_byte('0' + (MOTO1_PWM / 100));HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_9);//LED3
    //uart_send_string("\r\n");HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_9);//LED3
    // 限制PWM最大值
    if (MOTO1_PWM > Moto_PwmMax) MOTO1_PWM = Moto_PwmMax;
    if (MOTO2_PWM > Moto_PwmMax) MOTO2_PWM = Moto_PwmMax;
    if (MOTO3_PWM > Moto_PwmMax) MOTO3_PWM = Moto_PwmMax;
    if (MOTO4_PWM > Moto_PwmMax) MOTO4_PWM = Moto_PwmMax;
    // 限制最小值为0
    if (MOTO1_PWM < 0) MOTO1_PWM = 0;
    if (MOTO2_PWM < 0) MOTO2_PWM = 0;
    if (MOTO3_PWM < 0) MOTO3_PWM = 0;
    if (MOTO4_PWM < 0) MOTO4_PWM = 0;

    // 输出到定时器（根据实际硬件配置修改定时器和通道）
    TIM1->CCR1 = MOTO1_PWM;
    TIM1->CCR2 = MOTO2_PWM;
    TIM1->CCR3 = MOTO3_PWM;
    TIM1->CCR4 = MOTO4_PWM;
	
}

// 核心控制函数（PID计算与电机分配）
void CONTROL(float Cur_Roll, float Cur_Pitch, float Cur_Yaw, 
             float Ex_Roll, float Ex_Pitch, float Ex_Yaw) {
    // 计算PID输出（偏差=当前值-期望值）
    Roll_Pout = Roll_P * (Cur_Roll - Ex_Roll);
    Roll_Dout = Roll_D * (Cur_Roll - Old_Roll);
    
    Pitch_Pout = Pitch_P * (Cur_Pitch - Ex_Pitch);
    Pitch_Dout = Pitch_D * (Cur_Pitch - Old_Pitch);
    
    Yaw_Pout = Yaw_P * (Cur_Yaw - Ex_Yaw);
    Yaw_Dout = Yaw_D * (Cur_Yaw - Old_Yaw);
    
    // 总输出（暂时不使用积分项）
    Roll_Out = Roll_Pout + Roll_Iout + Roll_Dout;
    Pitch_Out = Pitch_Pout + Pitch_Iout + Pitch_Dout;
    Yaw_Out = Yaw_Pout + Yaw_Iout + Yaw_Dout;
    
    // 保存当前角度用于下次微分计算
    Old_Roll = Cur_Roll;
    Old_Pitch = Cur_Pitch;
    Old_Yaw = Cur_Yaw;
    
    // 电机分配（油门+姿态修正）
    if (THROTTLE > 50) {  // 油门大于阈值时启动电机
        motor1 = THROTTLE + Roll_Out + Pitch_Out + Yaw_Out;
        motor2 = THROTTLE - Roll_Out - Pitch_Out + Yaw_Out;
        motor3 = THROTTLE + Roll_Out - Pitch_Out - Yaw_Out;
        motor4 = THROTTLE - Roll_Out + Pitch_Out - Yaw_Out;
    } else {  // 油门过低时停转
        motor1 = motor2 = motor3 = motor4 = 0;
    }
    
    // 输出到电机
    Moto_PwmRflash(motor1, motor2, motor3, motor4);
		//Moto_PwmRflash(100, 100, 100, 100);
}
						 
extern UART_HandleTypeDef huart1;
// 蓝牙控制主循环（10ms调用一次）
void Bluetooth_Control_Loop(void) {
    // 此处应添加IMU角度读取代码（示例：假设已通过传感器获取角度）
    // Roll_Angle_C = ...;  // 实际横滚角
    // Pitch_Angle_C = ...; // 实际俯仰角
    // Yaw_Angle_C = ...;   // 实际航向角
	// 读取MPU6050原始数据
//        MPU6050_GetData(&MPU6050);
//        
//        // 进行姿态解算（使用互补滤波）
//        MPU6050_AttitudeEstimation(MPU6050);
//        
//        char buffer[50];
//        int len = snprintf(buffer, sizeof(buffer), "R:%.2f,P:%.2f,Y:%.2f\r\n", Roll_Angle_C, Pitch_Angle_C, Yaw_Angle_C);
//        HAL_UART_Transmit(&huart1, (uint8_t*)buffer, len, HAL_MAX_DELAY);
	
    Update_Control_Input();  // 从蓝牙状态更新期望角度和油门
	
    CONTROL(Roll_Angle_C, Pitch_Angle_C, Yaw_Angle_C,
            Roll_Angle_Ex, Pitch_Angle_Ex, Yaw_Angle_Ex);
	
   
}

void handle_command(uint8_t data){
	switch(data){
			case 0x31:  // 前进
				drone_state.forward = 1;
				drone_state.backward = 0;
				break;
			
			case 0x37:  // 左移
				drone_state.left = 1;
				drone_state.right = 0;
				break;
			
			case 0x34:  // 后退
				drone_state.backward = 1;
				drone_state.forward = 0;
				break;
			
			case 0x39:  // 右移
				drone_state.right = 1;
				drone_state.left = 0;
				break;
			
			case 0x38:  // 低速启动
				drone_state.throttle = 400;
				break;
			
			case 0x36:  // 加速上升
				drone_state.throttle += 50;
				break;
			
			case 0x32:  // 减速下降
				drone_state.throttle -= 100;
				break;
			
			case 0x33:  // 航向左偏
				drone_state.yaw_offset = -3;
				break;
			
			case 0x35:  // 航向右偏
				drone_state.yaw_offset = 3;
				break;
	//		case 0x30:
	//			Moto_PwmRflash(1000, 1000, 1000, 1000);
	//			break;
			default:
				HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_12);
				break;
		}
			
}
