#ifndef __ESP8266_H__
#define __ESP8266_H__

#include "sys.h"

#define ESP8266_RX_BUF_SIZE         2048
#define ESP8266_TX_BUF_SIZE         64

#define ESP8266_EOK                 0
#define ESP8266_ERROR               1
#define ESP8266_ETIMEOUT            2
#define ESP8266_EINVAL              3

#define ESP8266_STA_MODE            1
#define ESP8266_AP_MODE             2
#define ESP8266_STA_AP_MODE         3

#define ESP8266_SINGLE_CONNECTION   0
#define ESP8266_MULTI_CONNECTION    1

#define WIFI_SSID                   "iQOO Neo10"
#define WIFI_PWD                    "zcc199780"

#define TCP_SERVER_IP               "api.seniverse.com"
#define TCP_SERVER_PORT             "80"

void esp8266_init(uint32_t baudrate);
void esp8266_receive_data(void);
void esp8266_send_data(char *data, uint16_t len);
uint16_t esp8266_copy_rxdata(char *data);
uint8_t esp8266_wait_receive(void);
uint8_t esp8266_get_weather(uint8_t day);

#endif


