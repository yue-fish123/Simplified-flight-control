#ifndef __BULETOOTH_H_
#define __BULETOOTH_H_

extern UART_HandleTypeDef huart2;

void buletooth_sendbyte(uint8_t data);
void buletooth_sendstring(uint8_t *str);
void buletooth_StartReceiveIT(void);

#endif
