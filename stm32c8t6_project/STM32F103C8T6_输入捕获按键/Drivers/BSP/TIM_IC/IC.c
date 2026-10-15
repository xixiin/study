#include "ic.h"
#include "stdio.h"

TIM_HandleTypeDef tim2 = {0};

//初始化ic
void in_init(uint16_t arr,uint16_t psc)
{
	tim2.Instance = TIM2;
	tim2.Init.Period = arr;
	tim2.Init.Prescaler = psc;
	tim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	HAL_TIM_IC_Init(&tim2);
	
	TIM_IC_InitTypeDef ic_config = {0};
	ic_config.ICFilter  = 0;
	ic_config.ICPolarity = TIM_ICPOLARITY_RISING;
	ic_config.ICPrescaler = TIM_ICPSC_DIV1;
	ic_config.ICSelection = TIM_ICSELECTION_DIRECTTI;
	
	HAL_TIM_IC_ConfigChannel(&tim2,&ic_config,TIM_CHANNEL_3);
	__HAL_TIM_ENABLE_IT(&tim2,TIM_IT_UPDATE);
	HAL_TIM_IC_Start_IT(&tim2,TIM_CHANNEL_3);
	
	
}

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM2)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_TIM2_CLK_ENABLE();
		
		HAL_NVIC_SetPriority(TIM2_IRQn,2,2);
		HAL_NVIC_EnableIRQ(TIM2_IRQn);
		
		GPIO_InitTypeDef gpio_init={0};
		gpio_init.Pin = GPIO_PIN_2;
		gpio_init.Mode = GPIO_MODE_AF_INPUT;
		gpio_init.Pull = GPIO_PULLUP;
		gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
		
		HAL_GPIO_Init(GPIOA, &gpio_init);
	
	}


}


void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&tim2);

}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	printf("捕获到下降沿");
	if(htim->Instance == TIM2)
	{
		
	
	}

}

