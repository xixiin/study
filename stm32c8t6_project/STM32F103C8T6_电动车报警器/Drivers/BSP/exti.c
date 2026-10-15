#include "sys.h"
#include "exti.h"
#include "led.h"
#include "delay.h"

uint8_t button_A_flag = FALSE;
uint8_t button_B_flag = FALSE;
uint8_t vibrate_flag = FALSE;

void exti_init()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef gpio_init={0};
	gpio_init.Pin = GPIO_PIN_7;
	gpio_init.Mode = GPIO_MODE_IT_RISING;
	gpio_init.Pull = GPIO_PULLDOWN;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOA, &gpio_init);
	HAL_NVIC_SetPriority(EXTI9_5_IRQn,2,0);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
	
	gpio_init.Pin = GPIO_PIN_0;
	gpio_init.Mode = GPIO_MODE_IT_RISING;
	gpio_init.Pull = GPIO_PULLDOWN;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &gpio_init);
	HAL_NVIC_SetPriority(EXTI0_IRQn,2,0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	
	gpio_init.Pin = GPIO_PIN_2;
	gpio_init.Mode = GPIO_MODE_IT_FALLING;
	gpio_init.Pull = GPIO_PULLUP;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &gpio_init);
	HAL_NVIC_SetPriority(EXTI2_IRQn,2,0);
	HAL_NVIC_EnableIRQ(EXTI2_IRQn);
	
}

void EXTI9_5_IRQHandler()
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);

}
void EXTI0_IRQHandler()
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);

}
void EXTI2_IRQHandler()
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
//	delay_ms(20);
	if(GPIO_Pin == GPIO_PIN_7)
	{
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7) == GPIO_PIN_SET)
//			led_toggle();
			button_A_flag = TRUE ;
		
	}
	else if(GPIO_Pin == GPIO_PIN_0)
	{
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0) == GPIO_PIN_SET)

//			led_toggle1();
			button_B_flag = TRUE ;
	
	}
	else if(GPIO_Pin == GPIO_PIN_2)
	{
//		led_toggle1();
		vibrate_flag = TRUE ;
	}
}

uint8_t get_buttonA_flag()
{
	uint8_t temp= button_A_flag;
	button_A_flag = FALSE;
	return temp;

}
void set_buttonA_flag(uint8_t value)
{
	button_A_flag = value;
}
uint8_t get_buttonB_flag()
{
	uint8_t temp= button_B_flag;
	button_B_flag = FALSE;
	return temp;

}
void set_buttonB_flag(uint8_t value)
{
	button_B_flag = value;
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

