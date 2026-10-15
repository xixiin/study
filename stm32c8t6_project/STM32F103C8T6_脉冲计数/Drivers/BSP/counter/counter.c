#include "counter.h"
#include "stdio.h"

TIM_HandleTypeDef tim2 = {0};

//³õÊ¼»¯counter
void counter_init(uint16_t arr,uint16_t psc)
{
	TIM_SlaveConfigTypeDef counter_config = {0};
	tim2.Instance = TIM2;
	tim2.Init.Period = arr;
	tim2.Init.Prescaler = psc;
	tim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	HAL_TIM_IC_Init(&tim2);
	
	counter_config.SlaveMode = TIM_SLAVEMODE_EXTERNAL1;
	counter_config.InputTrigger = TIM_TS_TI2FP2;
	counter_config.TriggerPolarity = TIM_TRIGGERPOLARITY_FALLING;
	counter_config.TriggerFilter = 20;
	
	HAL_TIM_SlaveConfigSynchro(&tim2,&counter_config);
	HAL_TIM_IC_Start(&tim2,TIM_CHANNEL_2);
	
	
}

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM2)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_TIM2_CLK_ENABLE();
		

		
		GPIO_InitTypeDef gpio_init={0};
		gpio_init.Pin = GPIO_PIN_1;
		gpio_init.Mode = GPIO_MODE_AF_PP;
		gpio_init.Pull = GPIO_PULLUP;
		gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
		
		HAL_GPIO_Init(GPIOA, &gpio_init);
	
	}


}
uint16_t new_cnt = 0;
uint16_t old_cnt = 0;

void counter_get(void)
{
	new_cnt=__HAL_TIM_GET_COUNTER(&tim2); 
	if(old_cnt!=new_cnt)
	{
		printf("CNT=%d\r\n",new_cnt);
		old_cnt=new_cnt;
	}
}


