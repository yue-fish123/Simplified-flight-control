//#include "stm32g0xx.h"                  // Device header

//extern UART_HandleTypeDef huart1;
//	
//void USART1_FormatSend(const char *any_str, uint8_t *buf)
//{
//  // 1. 按格式拼接字符串（%s对应<any>，%d对应各通道数值，\n是结尾换行）
//  // 若需要增加通道（如ch3），只需在后面加 ",%d" 并补充参数（如ch3_val）
//  sprintf((char*)buf, "%s:%d,%d,%d\n", any_str, ch0_val, ch1_val, ch2_val);
//  
//  // 2. 通过USART1发送拼接好的字符串
//  // strlen((char*)buf)：计算实际拼接长度，避免发送多余字符
//  HAL_UART_Transmit(&huart1, buf, strlen((char*)buf), 1000);  // 超时1秒
//}
