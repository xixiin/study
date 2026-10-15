#include "sys.h"

void stm32_clock_init(uint32_t plln)
{
	HAL_StatusTypeDef ret = HAL_ERROR;
	RCC_OscInitTypeDef rcc_Oscinit = {0};
	rcc_Oscinit.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	rcc_Oscinit.HSEState = RCC_HSE_ON;
	rcc_Oscinit.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	rcc_Oscinit.PLL.PLLMUL = plln;
	rcc_Oscinit.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	rcc_Oscinit.PLL.PLLState = RCC_PLL_ON;
	ret = HAL_RCC_OscConfig(&rcc_Oscinit);
	if(ret != HAL_OK)
	{
		while(1);
	}
	RCC_ClkInitTypeDef rcc_Clkinit = {0};
	rcc_Clkinit.AHBCLKDivider = RCC_SYSCLK_DIV1;
	rcc_Clkinit.APB1CLKDivider = RCC_HCLK_DIV2;
	rcc_Clkinit.APB2CLKDivider = RCC_HCLK_DIV1;
	rcc_Clkinit.ClockType = (RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2);
	rcc_Clkinit.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	ret = HAL_RCC_ClockConfig(&rcc_Clkinit,FLASH_LATENCY_2);
	if(ret != HAL_OK)
	{
		while(1);
	}
	
}
