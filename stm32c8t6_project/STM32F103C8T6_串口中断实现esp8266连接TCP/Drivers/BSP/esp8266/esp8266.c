#include "sys.h"
#include "esp8266.h"
#include "string.h"
#include "delay.h"
#include "stdarg.h"

uint8_t esp8266_rxbuff[ESP8266_TX_BUFF_SIZE];
uint8_t esp8266_tx_buf[ESP8266_TX_BUFF_SIZE];
uint16_t esp8266_rx_cnt = 0,esp8266_rx_len = 0;
uint16_t esp8266_cnt =0,esp8266_old_cnt = 0;

UART_HandleTypeDef esp8266_handle = {0};

void esp8266_uart_init(uint32_t bandrate)
{
	esp8266_handle.Instance = USART3;
	esp8266_handle.Init.BaudRate = bandrate;
	esp8266_handle.Init.WordLength = UART_WORDLENGTH_8B;
	esp8266_handle.Init.StopBits = UART_STOPBITS_1;
	esp8266_handle.Init.Parity = UART_PARITY_NONE;
	esp8266_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	esp8266_handle.Init.Mode = UART_MODE_TX_RX;
	HAL_UART_Init(&esp8266_handle);

}



void USART3_IRQHandler(void)
{
	uint8_t receive_data = 0;
	if(__HAL_UART_GET_FLAG(&esp8266_handle,UART_FLAG_RXNE) != RESET )
	{
		if(esp8266_rx_cnt>=sizeof(esp8266_rxbuff))
			esp8266_rx_cnt = 0;
		receive_data = (uint8_t)(esp8266_handle.Instance->DR & (uint8_t)0x00FF);
		esp8266_rxbuff[esp8266_rx_cnt++] = receive_data;
	}
	if(__HAL_UART_GET_FLAG(&esp8266_handle,UART_FLAG_ORE) != RESET)
	{
		__HAL_UART_CLEAR_OREFLAG(&esp8266_handle);
	}
}

uint8_t esp8266_wait_receive(void)
{
	if(esp8266_rx_cnt==0)
		return esp8266_error;
	if(esp8266_rx_cnt == esp8266_old_cnt)
	{
		esp8266_rx_cnt = 0;
		return esp8266_ok;
	}
	esp8266_old_cnt = esp8266_rx_cnt;
	return esp8266_error;
}

void esp8266_rx_clear(void)
{
	memset(esp8266_rxbuff,0,sizeof(esp8266_rxbuff));
	esp8266_rx_cnt = 0;
	esp8266_old_cnt = 0;
}

uint8_t esp8266send_command(char *cmd,char *res)
{
	uint16_t timeout = 1000;
	esp8266_rx_clear();
	HAL_UART_Transmit(&esp8266_handle,(uint8_t *)cmd,strlen(cmd),100);
	
	while(timeout--)
	{
		if(esp8266_wait_receive() == esp8266_ok)
		{
			if(strstr((const char*)esp8266_rxbuff,res)!=NULL)
				return esp8266_ok;
			else
				memset(esp8266_rxbuff, 0, sizeof(esp8266_rxbuff)); // 清空缓冲区，等待下一包响应
		}
		delay_ms(10);
	}
	
	return esp8266_error;
}

void esp8266_send_data(char *fmt,...)
{
	va_list ap;
	uint16_t len;
	
	va_start(ap, fmt);
	vsprintf((char *)esp8266_tx_buf,fmt,ap);
	va_end(ap);
	
	len = strlen((const char*)esp8266_tx_buf);
	HAL_UART_Transmit(&esp8266_handle,(uint8_t *)esp8266_tx_buf,len,100);
}

void esp8266_receive(void)
{
	if(esp8266_wait_receive() == esp8266_ok)
	{
		printf("recv:%s\r\n",esp8266_rxbuff);
		esp8266_rx_len = 0;
	}

}
uint8_t esp8266_at_text()
{
	return esp8266send_command("AT\r\n","OK");
	
}


uint8_t esp8266Set_mode(uint8_t mode)
{
	switch(mode)
	{
		case esp8266_sta_mode: 
		{
			return esp8266send_command("AT+CWMODE=1\r\n","OK");
		}
		case esp8266_ap_mode:
		{
			return esp8266send_command("AT+CWMODE=2\r\n","OK");
		}
		case esp8266_sta_ap_mode:
		{
			return esp8266send_command("AT+CWMODE=3\r\n","OK");
		}
		default :
			return esp8266_einval;
	}
	
}

uint8_t esp8266_join_ap(char *SSID,char *PWD)
{
	char cmd[64];
	sprintf(cmd,"AT+CWJAP=\"%s\",\"%s\"\r\n",SSID,PWD);
	return esp8266send_command(cmd,"WIFI GOT IP");
}
uint8_t esp8266_conection_mode(uint8_t mode)
{
	char cmd[64];
	sprintf(cmd,"AT+CIPMUX=%d\r\n",mode);
	return esp8266send_command(cmd,"OK");
}

uint8_t esp8266_connect_tcp_server(char *Server_ip,char *server_port)
{
	char cmd[64];
	sprintf(cmd,"AT+CIPSTART=\"TCP\",\"%s\",%s\r\n",Server_ip,server_port);
	return esp8266send_command(cmd,"CONNECT");	

}

uint8_t esp8266_enter_unvarished(void)
{
	uint8_t ret;
	ret = esp8266send_command("AT+CIPMODE=1\r\n","OK");
	ret += esp8266send_command("AT+CIPSEND\r\n",">");
	if(ret == esp8266_ok)
		return esp8266_ok;
	else 
		return esp8266_error;
}

void esp8266_init(void)
{
	printf("初始化开始\r\n");
	esp8266_uart_init(115200);
	//其他初始化
	printf("1、测试esp8266是否存在\r\n");
	while(esp8266_at_text())
	{

		delay_ms(500);
	}
	printf("2、设置工作模式为STA\r\n");
	while(esp8266Set_mode(esp8266_sta_mode))
	{
		delay_ms(500);
	}	
	printf("3、设置单路连接模式\r\n");
	while(esp8266_conection_mode(esp8266_single_mode))
	{
		delay_ms(500);
	}	
	printf("4、连接WIFI，SSID：%s,PWD：%s\r\n",WIFI_SSID,WIFI_PWD);
	while(esp8266_join_ap(WIFI_SSID,WIFI_PWD))
	{
		delay_ms(500);
	}
	printf("esp8255初始化成功\r\n");
	printf("5、连接TCP服务器，server:%s,port %s\r\n",TCP_SERVER_IP,TCP_SERVER_PORT);
	while(esp8266_connect_tcp_server(TCP_SERVER_IP,TCP_SERVER_PORT))
	{
		delay_ms(500);
	}	
	
	printf("6、进入透传模式\r\n");
	while(esp8266_enter_unvarished())
	{
		delay_ms(500);
	}		
	
	
	printf("esp8255初始化成功\r\n");
	printf("esp8255连接到TCP服务器\r\n");
}



void esp8266_text(void)
{
	if(esp8266send_command("AT","OK")==esp8266_ok)
	{
		printf("esp8266 test%s\r\n",esp8266_rxbuff);
	}

}

void esp8266_test(void)
{
	esp8266_send_data("this is from esp8266\r\n");
	esp8266_receive();

}

