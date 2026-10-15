#ifndef __ESP8266_H__
#define __ESP8266_H__

#include "stdio.h"
#include "sys.h"
#define ESP8266_RX_BUFF_SIZE 		128
#define ESP8266_TX_BUFF_SIZE 		64
#define esp8266_ok   				0
#define esp8266_error   			1
#define esp8266_timeout    			2
#define esp8266_einval     			3
#define esp8266_sta_mode 			1
#define esp8266_ap_mode 			2
#define esp8266_sta_ap_mode 		3

#define esp8266_single_mode 		0
#define esp8266_multi_mode 			1

#define WIFI_SSID 					"iQOO Neo10"
#define WIFI_PWD  					"zcc199780"

#define TCP_SERVER_IP 				"10.117.195.207"
#define TCP_SERVER_PORT  			"1"

void esp8266_init(void);
void esp8266_receive(void);
void esp8266_test(void);
uint8_t esp8266_at_text(void);


#endif


