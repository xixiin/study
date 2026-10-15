#include "wwdg.h"
#include "led.h"

WWDG_HandleTypeDef wwdg_handle = {0};

void wwdg_init(uint8_t tr ,uint8_t wr,uint32_t psc)
{
	
	wwdg_handle.Instance =WWDG;
	wwdg_handle.Init.Prescaler = psc;
	wwdg_handle.Init.Window = wr;
	wwdg_handle.Init.Counter = tr;
	wwdg_handle.Init.EWIMode= WWDG_EWI_ENABLE; 
	
	HAL_WWDG_Init(&wwdg_handle);

}
void HAL_WWDG_MspInit(WWDG_HandleTypeDef *hwwdg)
{
	__HAL_RCC_WWDG_CLK_ENABLE();
	HAL_NVIC_SetPriority(WWDG_IRQn,2,2);
	HAL_NVIC_EnableIRQ(WWDG_IRQn);
}
void WWDG_IRQHandler(void)
{
	HAL_WWDG_IRQHandler(&wwdg_handle);

}
void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef *hwwdg)
{
	wwdg_feed();
	led_toggle();
}
void wwdg_feed(void)
{
	HAL_WWDG_Refresh(&wwdg_handle);

}
