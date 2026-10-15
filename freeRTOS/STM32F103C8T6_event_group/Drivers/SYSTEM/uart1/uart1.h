#ifndef __USART_H__
#define __USART_H__

#include "stdio.h"
#include "sys.h"

/* 错误代码 */
#define UART_EOK                     0      /* 没有错误 */
#define UART_ERROR                   1      /* 通用错误 */
#define UART_ETIMEOUT                2      /* 超时错误 */
#define UART_EINVAL                  3      /* 参数错误 */

/* UART收发缓冲大小 */
#define UART1_RX_BUF_SIZE            128
#define UART1_TX_BUF_SIZE            64

extern UART_HandleTypeDef uart1_handle;          /* UART1句柄 */
extern uint8_t  uart1_rx_buf[UART1_RX_BUF_SIZE]; /* UART1接收缓冲区 */
extern uint16_t uart1_rx_len;                    /* UART1接收字符长度 */

void uart1_init(uint32_t bound);            /* 串口初始化函数 */
void uart1_rx_clear(void);

#endif


