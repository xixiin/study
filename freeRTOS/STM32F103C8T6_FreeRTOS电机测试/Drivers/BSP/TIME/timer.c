#include "timer.h"
#include "led.h"

TIM_HandleTypeDef htim1 = {0};



	//定时器初始化
void timer_init(uint16_t ARR,uint16_t PSC)
{	
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = PSC;
	htim1.Init.Period = ARR;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	
	HAL_TIM_Base_Init(&htim1);
	HAL_TIM_Base_Start_IT(&htim1);
	

}
	
	//msp函数
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if(htim->Instance   ==  TIM1)
	{
		__HAL_RCC_TIM1_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM1_UP_IRQn,2,2);
		HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);
		
	}


	else if (htim->Instance == TIM3)
	{
		/*开启时钟*/
		__HAL_RCC_TIM3_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();

		/*GPIO初始化：PA6(TIM3_CH1), PA7(TIM3_CH2) 上拉输入*/
		GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
	else if (htim->Instance == TIM4)
	{
		/*开启时钟*/
		__HAL_RCC_TIM4_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();

		/*GPIO初始化：PB6(TIM4_CH1), PB7(TIM4_CH2) 上拉输入*/
		GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
}


	//中断函数
void TIM1_UP_IRQHandler()
{
	HAL_TIM_IRQHandler(&htim1);
	
}

uint32_t timecount;

	//更新中段回调函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{
		timecount++;
	
	}
}

