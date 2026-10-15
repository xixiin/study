#include "timer.h"
#include "led.h"

TIM_HandleTypeDef htim2 = {0};



	//定时器初始化
void timer_init(uint16_t ARR,uint16_t PSC)
{	
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = PSC;
	htim2.Init.Period = ARR;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	
	HAL_TIM_Base_Init(&htim2);
	HAL_TIM_Base_Start_IT(&htim2);
	

}
	
	//msp函数
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance   ==  TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM2_IRQn,2,2);
		HAL_NVIC_EnableIRQ(TIM2_IRQn);
		
	}

}


	//中断函数
void TIM2_IRQHandler()
{
	HAL_TIM_IRQHandler(&htim2);
	
}



	//更新中段回调函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM2)
	{
		led_toggle0();
	
	}
}

