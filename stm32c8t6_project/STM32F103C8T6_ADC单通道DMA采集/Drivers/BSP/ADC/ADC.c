#include "ADC.h"

ADC_HandleTypeDef hadc1={0};
DMA_HandleTypeDef hdma_adc1={0};

void ADC_Init(void)
{
    hadc1.Instance = ADC1;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
    hadc1.Init.ContinuousConvMode = ENABLE;
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
        GPIO_InitStruct.Pin = GPIO_PIN_0;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    }    
}

void dma_init(void)
{
    __HAL_RCC_DMA1_CLK_ENABLE();
    
    hdma_adc1.Instance = DMA1_Channel1;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc1.Init.Mode = DMA_CIRCULAR;
    hdma_adc1.Init.Priority = DMA_PRIORITY_MEDIUM;
    HAL_DMA_Init(&hdma_adc1);

    __HAL_LINKDMA(&hadc1,DMA_Handle,hdma_adc1);
}

void adc_channel_config(ADC_HandleTypeDef* hadc,uint32_t channel,uint32_t rank,uint32_t sampling_time)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = channel;
    sConfig.Rank = rank;
    sConfig.SamplingTime = sampling_time;
    HAL_ADC_ConfigChannel(hadc, &sConfig);
}



void adc_dma_init(uint32_t* mar)
{

    ADC_Init();
    dma_init();
    adc_channel_config(&hadc1, ADC_CHANNEL_0, ADC_REGULAR_RANK_1, ADC_SAMPLETIME_239CYCLES_5);
    HAL_ADC_Start_DMA(&hadc1, mar, 1);
}
