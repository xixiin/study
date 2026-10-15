#include "hcsr04.h"
#include "delay.h"
#include "hcsr04.h"

TIM_HandleTypeDef htim2 = {0};



	//定时器初始化
void timer_init(void)
{	
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 72-1;  //psc
	htim2.Init.Period = 65536-1;  //arr
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	
	HAL_TIM_Base_Init(&htim2);
//	HAL_TIM_Base_Start(&htim2);
	

}
	
	//msp函数
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance   ==  TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();

		
	}

}

void tim_star(void)
{
	HAL_TIM_Base_Start(&htim2);
}
void tim_stop(void)
{
	HAL_TIM_Base_Stop(&htim2);
}

uint16_t get_cnt(void)
{
	return __HAL_TIM_GetCounter(&htim2) ;
		
}
void tim_set_cnt(uint16_t val)
{
	__HAL_TIM_SetCounter(&htim2,val);

}
void hcsr04_gpio_init(void)
{
	TRIG_GPIO_CLK_ENABLE();
	ECHO_GPIO_CLK_ENABLE();
	GPIO_InitTypeDef gpio_init={0};
	gpio_init.Pin = TRIG_PIN;
	gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init.Pull = GPIO_NOPULL;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(TRIG_PORT, &gpio_init);
	gpio_init.Pin = ECHO_PIN;
	gpio_init.Mode = GPIO_MODE_INPUT;
	gpio_init.Pull = GPIO_NOPULL;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(ECHO_PORT, &gpio_init);

}
void hcsr04_init(void)
{
	timer_init();
	hcsr04_gpio_init();

}
uint16_t time_cnt = 0;
float distance = 0;
float hcsr04_get_length(void)
{
	TRIG_HIGH();
	delay_us(15);
	TRIG_LOW();
	while(ECHO_STATE()==GPIO_PIN_RESET);
	tim_star();
	tim_set_cnt(0);
	while(ECHO_STATE()==GPIO_PIN_SET);
	tim_stop();
	time_cnt=get_cnt();
	distance = time_cnt*0.01715;
	return distance;
}

