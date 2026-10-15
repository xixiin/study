#ifndef __BLUETOOS_H__
#define __BLUETOOS_H__

#include "stdio.h"
#include "sys.h"


/* UART收发缓冲大小 */
#define UART2_RX_BUF_SIZE            128
#define UART2_TX_BUF_SIZE            64


void bt_init(uint32_t bound);            /* 串口初始化函数 */
void bluetoos_send(char *sendbuff,uint8_t size);
void bt_send(char *format,...);

extern uint8_t  uart2_rx_buf[UART2_RX_BUF_SIZE];
extern uint16_t uart2_rx_len;

#endif


