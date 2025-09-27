#include "stm32g0xx.h"                  // Device header

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart1;
extern uint8_t rx_data;

void  buletooth_sendbyte(uint8_t data){
	HAL_UART_Transmit(&huart2, &data, 1, 0xFFFF);
}

void buletooth_sendstring(uint8_t *str){
	uint16_t len = 0;
    while (str[len] != '\0')
    {
        len++;
    }
    HAL_UART_Transmit(&huart2, str, len, 0xFFFF);
}

void buletooth_StartReceiveIT(void){
	HAL_UART_Receive_IT(&huart2, &rx_data, 1);
}

// UART接收完成回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart2)
    {
        // 收到数据后可以在这里处理
        // 示例：将收到的数据原样发送回去（回显功能）
        buletooth_sendbyte(rx_data);
         HAL_UART_Transmit(&huart1, &rx_data, 1, 500);  // 超时500ms
        // 继续开启中断接收下一个字节
        buletooth_StartReceiveIT();
    }
}
