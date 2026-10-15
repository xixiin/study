#include "led.h"
#include "sys.h"



void led_init()
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef gpio_init={0};
	gpio_init.Pin = GPIO_PIN_9|GPIO_PIN_8;
	gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init.Pull = GPIO_NOPULL;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOB, &gpio_init);
	led_off();
	led_off1();
}
	
void led_on()
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET);

}
	
void led_off()
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);

}
void led_on1()
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);

}
	
void led_off1()
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);

}

void led_toggle()
{
	HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_8);
}
void led_toggle1()
{
	HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_9);
}
