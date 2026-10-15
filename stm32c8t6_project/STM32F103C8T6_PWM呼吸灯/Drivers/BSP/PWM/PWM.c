#include "pwm.h"
#include "led.h"

TIM_HandleTypeDef htim2 = {0};



	//pwm配置
void pwm_init(uint16_t ARR,uint16_t PSC)
{	
	TIM_OC_InitTypeDef pwm_config = {0};
	
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = PSC;
	htim2.Init.Period = ARR;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	
	HAL_TIM_PWM_Init(&htim2);
	
	pwm_config.OCMode = TIM_OCMODE_PWM1;
	pwm_config.Pulse = ARR/2;
	pwm_config.OCPolarity = TIM_OCPOLARITY_LOW;
	HAL_TIM_PWM_ConfigChannel(&htim2,&pwm_config,TIM_CHANNEL_1);
	
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);

}
	
	//msp函数配置
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance   ==  TIM2)
	{	
		//开启时钟
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_TIM2_CLK_ENABLE();

		GPIO_InitTypeDef gpio_init={0};
		gpio_init.Pin = GPIO_PIN_0;
		gpio_init.Mode = GPIO_MODE_AF_PP;
		gpio_init.Pull = GPIO_NOPULL;
		gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
		
		HAL_GPIO_Init(GPIOA, &gpio_init);
		
	}

}


	//修改ARRֵ
void pwm_compare_set(uint16_t val)
{
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,val);
	
}





