#include "iwdg.h"

IWDG_HandleTypeDef iwdg_handle = {0};

void iwdg_init(uint8_t psc ,uint16_t rlr)
{
	
	iwdg_handle.Instance =IWDG;
	iwdg_handle.Init.Prescaler = psc;
	iwdg_handle.Init.Reload = rlr;
	
	HAL_IWDG_Init(&iwdg_handle);

}

void iwdg_feed(void)
{
	HAL_IWDG_Refresh(&iwdg_handle);

}
