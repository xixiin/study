#include "sys.h"
#include "exti.h"
#include "led.h"
#include "delay.h"


uint8_t vibrate_flag = FALSE;

void exti_init()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef gpio_init={0};
	gpio_init.Pin = GPIO_PIN_2;
	gpio_init.Mode = GPIO_MODE_IT_FALLING;
	gpio_init.Pull = GPIO_PULLUP;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOA, &gpio_init);
	HAL_NVIC_SetPriority(EXTI2_IRQn,2,0);
	HAL_NVIC_EnableIRQ(EXTI2_IRQn);

}

void EXTI2_IRQHandler()
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);

}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	delay_ms(20);
	if(GPIO_Pin == GPIO_PIN_2)
	{
//		led_toggle1();
		vibrate_flag = TRUE ;
	}
}

uint8_t vibrate_flag_get(void)
{
	uint8_t temp = vibrate_flag;
	vibrate_flag = FALSE ;
	return temp;
}

void set_vibrate_flag(uint8_t value)
{
	vibrate_flag = value;
}

