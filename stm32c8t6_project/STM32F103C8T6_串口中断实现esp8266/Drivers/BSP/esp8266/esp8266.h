#ifndef __ESP8266_H__
#define __ESP8266_H__

#include "stdio.h"
#include "sys.h"
#define ESP8266_RX_BUFF_SIZE 128
#define ESP8266_TX_BUFF_SIZE 64
#define esp8266_ok   		0
#define esp8266_error   	1
#define esp8266_timeout    2
#define esp8266_einval     3

void esp8266_init(void);
void esp8266_receive(void);
void esp8266_text(void);

#endif


