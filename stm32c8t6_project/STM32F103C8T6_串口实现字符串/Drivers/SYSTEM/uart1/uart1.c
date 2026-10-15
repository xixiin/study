#include "sys.h"
#include "uart1.h"
#include "string.h"

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
			__HAL_UART_ENABLE_IT(&uart_handle,UART_IT_RXNE);
	}
}

void USART1_IRQHandler(void)
{
	uint8_t receive_data = 0;
	if(__HAL_UART_GET_FLAG(&uart_handle,UART_FLAG_RXNE) != RESET )
	{
		HAL_UART_Receive(&uart_handle,&receive_data,1,HAL_MAX_DELAY);
		HAL_UART_Transmit(&uart_handle,&receive_data,1,HAL_MAX_DELAY);
	}

}
