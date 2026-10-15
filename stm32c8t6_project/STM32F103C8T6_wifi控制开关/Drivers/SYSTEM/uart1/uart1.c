#include "sys.h"
#include "uart1.h"
#include "string.h"

uint8_t uart1_rxbuff[UART1_RX_BUFF_SIZE];
uint16_t uart1_rx_cnt = 0,uart1_rx_len = 0;
uint16_t uart1_cnt =0,uart1_old_cnt = 0;

UART_HandleTypeDef uart_handle = {0};

void uart_init(uint32_t bandrate)
{
	uart_handle.Instance = USART1;
	uart_handle.Init.BaudRate = bandrate;
	uart_handle.Init.WordLength = UART_WORDLENGTH_8B;
	uart_handle.Init.StopBits = UART_STOPBITS_1;
	uart_handle.Init.Parity = UART_PARITY_NONE;
	uart_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uart_handle.Init.Mode = UART_MODE_TX_RX;
	HAL_UART_Init(&uart_handle);

}


void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{
			__HAL_RCC_GPIOA_CLK_ENABLE();
			__HAL_RCC_USART1_CLK_ENABLE();
		
			GPIO_InitTypeDef gpio_init={0};
			gpio_init.Pin = GPIO_PIN_9;
			gpio_init.Mode = GPIO_MODE_AF_PP;
			gpio_init.Pull = GPIO_PULLUP;
			gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
			
			HAL_GPIO_Init(GPIOA, &gpio_init);
			
			gpio_init.Pin = GPIO_PIN_10;
			gpio_init.Mode = GPIO_MODE_INPUT;
			gpio_init.Pull = GPIO_PULLUP;
			gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
			
			HAL_GPIO_Init(GPIOA, &gpio_init);
			HAL_NVIC_EnableIRQ(USART1_IRQn);
			HAL_NVIC_SetPriority(USART1_IRQn,2,2);
			__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);
	}
		if(huart->Instance == USART3)
	{
			__HAL_RCC_GPIOB_CLK_ENABLE();
			__HAL_RCC_USART3_CLK_ENABLE();
		
			GPIO_InitTypeDef gpio_init={0};
			gpio_init.Pin = GPIO_PIN_10;
			gpio_init.Mode = GPIO_MODE_AF_PP;
			gpio_init.Pull = GPIO_PULLUP;
			gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
			
			HAL_GPIO_Init(GPIOB, &gpio_init);
			
			gpio_init.Pin = GPIO_PIN_11;
			gpio_init.Mode = GPIO_MODE_INPUT;
			gpio_init.Pull = GPIO_PULLUP;
			gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
			
			HAL_GPIO_Init(GPIOB, &gpio_init);
			HAL_NVIC_EnableIRQ(USART3_IRQn);
			HAL_NVIC_SetPriority(USART3_IRQn,2,2);
			__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);
	}
}

void USART1_IRQHandler(void)
{
	uint8_t receive_data = 0;
	if(__HAL_UART_GET_FLAG(&uart_handle,UART_FLAG_RXNE) != RESET )
	{
		if(uart1_rx_cnt>=sizeof(uart1_rxbuff))
			uart1_rx_cnt = 0;
		receive_data = (uint8_t)(uart_handle.Instance->DR & (uint8_t)0x00FF);
		uart1_rxbuff[uart1_rx_cnt++] = receive_data;
	}
	if(__HAL_UART_GET_FLAG(&uart_handle,UART_FLAG_ORE) != RESET)
	{
		__HAL_UART_CLEAR_OREFLAG(&uart_handle);
	}
}

uint8_t uart1_wait_receive(void)
{
	if(uart1_rx_cnt==0)
		return uart_error;
	if(uart1_rx_cnt == uart1_old_cnt)
	{
		uart1_rx_cnt = 0;
		return uart_ok;
	}
	uart1_old_cnt = uart1_rx_cnt;
	return uart_error;
}

void uart_rx_clear(void)
{
	memset(uart1_rxbuff,0,sizeof(uart1_rxbuff));
	uart1_rx_cnt = 0;
	uart1_old_cnt = 0;
}


void uart1_receive_test(void)
{
	if(uart1_wait_receive() == uart_ok)
	{
		printf("recv:%s\r\n",uart1_rxbuff);
		uart1_rx_len = 0;
	}

}
int fputc(int ch , FILE *f)
{
	while((USART1->SR & 0x40)==0);
	
	USART1->DR = (uint8_t)ch;
	return ch;

}
