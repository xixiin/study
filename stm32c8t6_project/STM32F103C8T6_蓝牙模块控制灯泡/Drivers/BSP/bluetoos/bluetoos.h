#ifndef __BLUETOOS_H__
#define __BLUETOOS_H__

#include "stdio.h"
#include "sys.h"


/* UART收发缓冲大小 */
#define UART3_RX_BUF_SIZE            128
#define UART3_TX_BUF_SIZE            64


void bt_init(uint32_t bound);            /* 串口初始化函数 */
void bluetoos_send(char *sendbuff,uint8_t size);
void bt_send(char *format,...);

#endif


