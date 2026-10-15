#include "sys.h"
#include "Key.h"
#include "delay.h"

void key_init()
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef gpio_init={0};
	gpio_init.Pin = GPIO_PIN_10|GPIO_PIN_11;
	gpio_init.Mode = GPIO_MODE_INPUT;
	gpio_init.Pull = GPIO_PULLUP;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOB, &gpio_init);
	gpio_init.Pin = GPIO_PIN_11|GPIO_PIN_12;
	gpio_init.Mode = GPIO_MODE_INPUT;
	gpio_init.Pull = GPIO_PULLUP;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;

	HAL_GPIO_Init(GPIOA, &gpio_init);
}

uint8_t key_scand(void)
{
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)==GPIO_PIN_RESET)
	{
		
		while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)==GPIO_PIN_RESET);
		return 1;
	}
		
	else if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)==GPIO_PIN_RESET)
	{
		
		while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)==GPIO_PIN_RESET);
		return 2;
	}
	else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_11)==GPIO_PIN_RESET)
	{
		
		while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_11)==GPIO_PIN_RESET);
		return 3;
	}
	else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_12)==GPIO_PIN_RESET)
	{
		
		while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_12)==GPIO_PIN_RESET);
		return 4;
	}
	else 
		return 0;
}

