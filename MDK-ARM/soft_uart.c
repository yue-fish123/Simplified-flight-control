#include "stm32g0xx.h"                  // Device header
#include "soft_uart.h"

#define UART_RX_PIN     GPIO_PIN_11
#define UART_RX_PORT    GPIOB
#define UART_TX_PIN     GPIO_PIN_10
#define UART_TX_PORT    GPIOB
#define BIT_TIME_US     104  // 9600 波特率，104us 每位

// 延时函数（微秒级）
void uart_delay(uint32_t us) {
    uint32_t ticks = us * (SystemCoreClock / 1000000) / 8;
    while (ticks--);
}

void soft_uart_init(void){
	 // TX 引脚初始化为高电平
    HAL_GPIO_WritePin(UART_TX_PORT, UART_TX_PIN, GPIO_PIN_SET);
    // RX 引脚已在 MX_GPIO_Init 中配置
    
}

void soft_uart_send(uint8_t data){
	// 起始位：低
    HAL_GPIO_WritePin(UART_TX_PORT, UART_TX_PIN, GPIO_PIN_RESET);
    uart_delay(BIT_TIME_US);

    // 数据位：8 位，低位先发
    for (int i = 0; i < 8; i++) {
        HAL_GPIO_WritePin(UART_TX_PORT, UART_TX_PIN, (data >> i) & 0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET);
        uart_delay(BIT_TIME_US);
    }

    // 停止位：高
    HAL_GPIO_WritePin(UART_TX_PORT, UART_TX_PIN, GPIO_PIN_SET);
    uart_delay(BIT_TIME_US);
}

uint8_t soft_uart_recieve(void){
	 uint8_t data = 0;

    // 等待起始位（低电平）
    while (HAL_GPIO_ReadPin(UART_RX_PORT, UART_RX_PIN) == GPIO_PIN_SET);

    // 起始位中点采样
    uart_delay(BIT_TIME_US / 2 + BIT_TIME_US);

    // 接收 8 位
    for (int i = 0; i < 8; i++) {
        if (HAL_GPIO_ReadPin(UART_RX_PORT, UART_RX_PIN) == GPIO_PIN_SET) {
            data |= (1 << i);
        }
        uart_delay(BIT_TIME_US);
    }

    return data;
}


