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
		HAL_UART_Receive(&esp8266_handle,&receive_data,1,HAL_MAX_DELAY);
		esp8266_rxbuff[esp8266_rx_cnt++] = receive_data;
		HAL_UART_Transmit(&esp8266_handle,&receive_data,1,HAL_MAX_DELAY);
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
	
}


void esp8266_receive(void)
{
	if(esp8266_wait_receive() == esp8266_ok)
	{
		printf("recv:%s\r\n",esp8266_rxbuff);
		esp8266_rx_len = 0;
	}

}

void esp8266_init(void)
{
	esp8266_uart_init(115200);
	//其他初始化
	
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

void esp8266_text(void)
{
	if(esp8266send_command("AT\r\n","OK")==esp8266_ok)
	{
		printf("esp8266 test%s\r\n",esp8266_rxbuff);
	}

}
