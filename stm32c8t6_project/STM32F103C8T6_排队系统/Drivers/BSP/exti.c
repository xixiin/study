#include "sys.h"
#include "exti.h"
#include "led.h"
#include "delay.h"


uint8_t vibrate_flag = FALSE;

void exti_init()
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef gpio_init={0};
	gpio_init.Pin = GPIO_PIN_11;
	gpio_init.Mode = GPIO_MODE_IT_FALLING;
	gpio_init.Pull = GPIO_PULLUP;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOB, &gpio_init);
	HAL_NVIC_SetPriority(EXTI15_10_IRQn,2,0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

void EXTI15_10_IRQHandler()
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);

}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	delay_ms(20);
	if(GPIO_Pin == GPIO_PIN_11)
	{
		// 再次判断引脚是否为低电平，确保是有效按下
		if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == GPIO_PIN_RESET)
		{
//			led_toggle1();
			vibrate_flag = TRUE ;
		}
		
		// 清除在延时 20ms 期间因为按键抖动又重新生成的中断标志位
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_11);
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

