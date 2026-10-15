#include "sys.h"
#include "Key.h"
#include "delay.h"

void key_init()
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef gpio_init={0};
	gpio_init.Pin = GPIO_PIN_0|GPIO_PIN_1;
	gpio_init.Mode = GPIO_MODE_INPUT;
	gpio_init.Pull = GPIO_PULLUP;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOB, &gpio_init);
	gpio_init.Pin = GPIO_PIN_5|GPIO_PIN_4;
	gpio_init.Mode = GPIO_MODE_INPUT;
	gpio_init.Pull = GPIO_PULLUP;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;

	HAL_GPIO_Init(GPIOA, &gpio_init);
}

uint8_t key_scand(void)
{
	/* 非阻塞按键扫描：只在下降沿（按下瞬间）返回键值，不忙等松开 */
	static uint8_t last_key = 0;
	uint8_t current = 0;

	if      (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET) current = 1;
	else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET) current = 2;
	else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == GPIO_PIN_RESET) current = 3;
	else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == GPIO_PIN_RESET) current = 4;

	if (current != 0 && current != last_key)
	{
		last_key = current;
		return current;   /* 按下瞬间返回键值 */
	}
	last_key = current;
	return 0;             /* 无新按键 */
}

