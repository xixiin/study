#include "sys.h"
#include "esp8266.h"
#include "string.h"
#include "delay.h"

uint8_t esp8266_rxbuff[ESP8266_TX_BUFF_SIZE];
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
	uint8_t timeout = 250;
	esp8266_rx_clear();
	HAL_UART_Transmit(&esp8266_handle,(uint8_t *)cmd,strlen(cmd),100);
	
	while(timeout--)
	{
		if(esp8266_wait_receive() == esp8266_ok)
		{
			if(strstr((const char*)esp8266_rxbuff,res)!=NULL)
				return esp8266_ok;

		}
		delay_ms(10);
	}
	
	return esp8266_error;
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


void esp8266_init(void)
{
	printf("初始化开始\r\n");
	esp8266_uart_init(115200);
	//其他初始化
	printf("测试esp8266是否存在\r\n");
	while(esp8266_at_text())
	{

		delay_ms(500);
	}
	printf("设置工作模式为STA\r\n");
	while(esp8266Set_mode(esp8266_sta_mode))
	{
		delay_ms(500);
	}	
	printf("设置单路连接模式\r\n");
	while(esp8266_conection_mode(esp8266_single_mode))
	{
		delay_ms(500);
	}	
	printf("连接WIFI，SSID：%s,PWD：%s\r\n",WIFI_SSID,WIFI_PWD);
	while(esp8266_join_ap(WIFI_SSID,WIFI_PWD))
	{
		delay_ms(500);
	}
	printf("连接成功");
}



void esp8266_text(void)
{
	if(esp8266send_command("AT","OK")==esp8266_ok)
	{
		printf("esp8266 test%s\r\n",esp8266_rxbuff);
	}

}
