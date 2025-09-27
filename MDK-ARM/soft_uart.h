#ifndef __SOFT_UART_
#define __SOFT_UART_

#include "types.h"

#define UART_RX_PIN     GPIO_PIN_11
#define UART_RX_PORT    GPIOB
#define UART_TX_PIN     GPIO_PIN_10
#define UART_TX_PORT    GPIOB
#define BIT_TIME_US     104  // 9600 波特率，104us 每位

void uart_delay(uint32_t us);
void soft_uart_init(void);
void soft_uart_send(uint8_t data);
uint8_t soft_uart_recieve(void);

#endif
