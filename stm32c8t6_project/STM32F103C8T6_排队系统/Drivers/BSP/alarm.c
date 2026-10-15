#include "alarm.h"
#include "sys.h"



void alarm_init()
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef gpio_init={0};
	gpio_init.Pin = GPIO_PIN_10;
	gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init.Pull = GPIO_PULLUP;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOB, &gpio_init);
	alarm_off();

}
	
void alarm_on()
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);

}
	
void alarm_off()
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);

}

uint8_t get_alarm_status()
{
	return (uint8_t)HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10);

}
