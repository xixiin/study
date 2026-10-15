#include "sys.h"
#include "key.h"


void key_init()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef gpio_init={0};
	gpio_init.Pin = GPIO_PIN_1;
	gpio_init.Mode = GPIO_MODE_INPUT;
	gpio_init.Pull = GPIO_PULLUP;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOA, &gpio_init);
	


}

uint8_t key_scand(void)
{
	return HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1);

}

