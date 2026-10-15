#include "ic.h"
#include "stdio.h"
#include "string.h"

struct 
{
	uint8_t succeed_flag;
	uint8_t rising_flag;
	uint8_t falling_falag;
	uint16_t timout_cnt;

} capture_status = {0};

uint16_t last_cnt = 0;
uint16_t tim_cnt;

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
	ic_config.ICPolarity = TIM_ICPOLARITY_FALLING;
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

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM2)
	{
		if(capture_status.succeed_flag == 0)
		{
			capture_status.timout_cnt++;
		}
	
	}

}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{

	if(htim->Instance == TIM2)
	{
		if(capture_status.succeed_flag==0 )
		{
			if(capture_status.falling_falag == 1)
			{
				capture_status.succeed_flag = 1;
				TIM_RESET_CAPTUREPOLARITY (&tim2,TIM_CHANNEL_3);
				TIM_SET_CAPTUREPOLARITY(&tim2,TIM_CHANNEL_3,TIM_ICPOLARITY_FALLING);

				printf("捕获到上升沿\r\n");
				last_cnt = HAL_TIM_ReadCapturedValue(&tim2,TIM_CHANNEL_3);
				

			}
			else 
			{
				memset(&capture_status,0,sizeof(capture_status));
				__HAL_TIM_DISABLE(&tim2);
				__HAL_TIM_SET_COUNTER(&tim2,0);
				capture_status.falling_falag=1;
				TIM_RESET_CAPTUREPOLARITY (&tim2,TIM_CHANNEL_3);
				TIM_SET_CAPTUREPOLARITY(&tim2,TIM_CHANNEL_3,TIM_ICPOLARITY_RISING);
				printf("捕获到下降沿\r\n");
				
				__HAL_TIM_ENABLE(&tim2);
			}
			
		}
	
	}

}

void presss_time_get()
{
	if(capture_status.succeed_flag==1)
	{
		printf("按下时间：%dus\r\n",capture_status.timout_cnt*65536+last_cnt);
		memset(&capture_status,0,sizeof(capture_status));
	}

}
