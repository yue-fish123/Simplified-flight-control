#include "user.h"
/*

互斥锁调用
osMutexWait(FLASH_MutexHandle, osWaitForever);
osMutexRelease(FLASH_MutexHandle);

定时器：
HAL_TIM_Base_Start_IT(&htim7);开启定时器及中断 
HAL_TIM_Base_Start(&htim5);开启定时器
__HAL_TIM_CLEAR_IT(&htim5, TIM_IT_UPDATE);清除定时器中断标志

串口：
HAL_UART_Receive_IT(&huart1,&UART1_Buffer,1);打开串口接受中断并接受一个字符
__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);打开串口接受中断
__HAL_UART_CLEAR_FLAG(huart, UART_FLAG_TC);
__HAL_UART_GET_FLAG(huart, UART_FLAG_TC);


PWM：	
HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);开启对应通道PWM
	
ADC+DMA：
HAL_ADC_Start_DMA(&hadc1,ADC_Value,ADC_CHANNEL_NUMBER);开启ADC+DMA转换
*/

void APP_Init(void){
	HAL_Delay(200);//系统器件上电时间
	
	//Load_MCU_UID();
//	__HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_TC);
//	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
	//__HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
	
	//while(HAL_ADCEx_Calibration_Start(&hadc1) != HAL_OK);
	//HAL_ADC_Start_DMA(&hadc1,ADC_Value,ADC_CHANNEL_NUMBER);//开启ADC+DMA转换
	AnBT_DMP_MPU6050_Init();
	Pid_init();
	
		
}

