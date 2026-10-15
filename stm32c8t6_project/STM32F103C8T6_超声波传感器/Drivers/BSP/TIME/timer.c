#include "timer.h"
#include "led.h"
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

void hcsr04_init(void)
{



}