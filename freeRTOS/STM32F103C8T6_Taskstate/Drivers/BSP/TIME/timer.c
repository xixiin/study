#include "timer.h"
#include "led.h"
#include "stdio.h"
TIM_HandleTypeDef htim2 = {0};

TIM_HandleTypeDef htim3 = {0};

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
	
	htim3.Instance = TIM3;
	htim3.Init.Prescaler = PSC;
	htim3.Init.Period = ARR;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	
	HAL_TIM_Base_Init(&htim3);
	HAL_TIM_Base_Start_IT(&htim3);

}
	
	//msp函数
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance   ==  TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM2_IRQn,9,0);
		HAL_NVIC_EnableIRQ(TIM2_IRQn);
		
	}
	if(htim->Instance   ==  TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM3_IRQn,15,0);
		HAL_NVIC_EnableIRQ(TIM3_IRQn);
		
	}
}


	//中断函数
void TIM2_IRQHandler()
{
	HAL_TIM_IRQHandler(&htim2);
	
}

void TIM3_IRQHandler()
{
	HAL_TIM_IRQHandler(&htim3);
	
}


	//更新中段回调函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM2)
	{
		printf("TIM2 running...\r\n");
	
	}
	if(htim->Instance == TIM3)
	{
		printf("TIM3 running...\r\n");
	}
}

