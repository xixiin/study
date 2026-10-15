#ifndef __USART_H__
#define __USART_H__

#include "stdio.h"
#include "sys.h"
#define UART1_RX_BUFF_SIZE 128
#define UART1_TX_BUFF_SIZE 64
#define uart_ok   		0
#define uart_error   	1
#define uart_timeout    2
#define uart_einval     3

void uart_init(uint32_t bandrate);

#endif


