#include "rtc.h"

RTC_HandleTypeDef rtc_handle = {0};

void rtc_init(void)
{
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_RCC_BKP_CLK_ENABLE();
	HAL_PWR_EnableBkUpAccess();
	
	rtc_handle.Instance = RTC;
	rtc_handle.Init.AsynchPrediv = 32767;
	rtc_handle.Init.OutPut = RTC_OUTPUTSOURCE_NONE;
	HAL_RTC_Init(&rtc_handle);
}

uint16_t rtc_read_bkr(uint8_t bkrx)
{
	uint32_t data = 0;
	data=HAL_RTCEx_BKUPRead(&rtc_handle,bkrx);
	return (uint16_t)data;

}

void rtc_wirte_bkr(uint8_t bkrx,uint16_t data)
{
	HAL_RTCEx_BKUPWrite(&rtc_handle,bkrx,data);


}
