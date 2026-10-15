#include "led.h"
#include "sys.h"



void led_init()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef gpio_init={0};
	gpio_init.Pin = GPIO_PIN_8|GPIO_PIN_9;
	gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init.Pull = GPIO_PULLUP;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOA, &gpio_init);

	led_off_0();
	led_off_1();

}
	
void led_on_0()
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);

}
void led_on_1()
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_RESET);

}
void led_off_0()
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);

}
void led_off_1()
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_SET);

}
void led_toggle0()
{
	HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_8);
}                          
void led_toggle1()         
{                          
	HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_9);
}
