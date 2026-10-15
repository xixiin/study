#include "RT.h"

ADC_HandleTypeDef hadc1={0};

void RT_Init(void)
{
    hadc1.Instance = ADC1;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.NbrOfConversion = 1;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.NbrOfDiscConversion = 0;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    HAL_ADC_Init(&hadc1);

    HAL_ADCEx_Calibration_Start(&hadc1);
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
   if(hadc->Instance == ADC1)
    {
        RCC_PeriphCLKInitTypeDef adc_init_clk = {0};
        __HAL_RCC_ADC1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        adc_init_clk.PeriphClockSelection = RCC_PERIPHCLK_ADC;
        adc_init_clk.AdcClockSelection = RCC_ADCPCLK2_DIV6;
        HAL_RCCEx_PeriphCLKConfig(&adc_init_clk);

        GPIO_InitTypeDef GPIO_InitStruct = {0};
        GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    }    
}

void adc_channel_config(ADC_HandleTypeDef* hadc,uint32_t channel,uint32_t rank,uint32_t sampling_time)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = channel;
    sConfig.Rank = rank;
    sConfig.SamplingTime = sampling_time;
    HAL_ADC_ConfigChannel(hadc, &sConfig);
}

uint32_t ADC_GetValue(uint32_t n)
{
	if(n==1)
	{
		    adc_channel_config(&hadc1, 2, ADC_REGULAR_RANK_1, ADC_SAMPLETIME_239CYCLES_5);
	}
	else if(n==2)
	{
		    adc_channel_config(&hadc1, 3, ADC_REGULAR_RANK_1, ADC_SAMPLETIME_239CYCLES_5);
	}
	else if(n==3)
	{
		    adc_channel_config(&hadc1, 4, ADC_REGULAR_RANK_1, ADC_SAMPLETIME_239CYCLES_5);
	}
	else if(n==4)
	{
		    adc_channel_config(&hadc1, 5, ADC_REGULAR_RANK_1, ADC_SAMPLETIME_239CYCLES_5);
	}
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    return HAL_ADC_GetValue(&hadc1); 
}
