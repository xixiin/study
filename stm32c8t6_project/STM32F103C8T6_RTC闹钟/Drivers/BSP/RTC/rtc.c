#include "rtc.h"
#include "stdio.h"

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
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
	__HAL_RCC_RTC_ENABLE();
	RCC_OscInitTypeDef osc_initstruct = {0};
	RCC_PeriphCLKInitTypeDef periph_initstruct = {0};
	
	osc_initstruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
	osc_initstruct.LSEState = RCC_LSE_ON;
	osc_initstruct.PLL.PLLState = RCC_PLL_NONE;
	
	periph_initstruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	periph_initstruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	
	HAL_RCC_OscConfig(&osc_initstruct);
	HAL_RCCEx_PeriphCLKConfig(&periph_initstruct);
	HAL_NVIC_SetPriority(RTC_Alarm_IRQn,2,2);
	HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
}

void RTC_Alarm_IRQHandler(void)
{
	HAL_RTC_AlarmIRQHandler(&rtc_handle);
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	printf("ring ring ring\r\n");
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

void rtc_get_time(void)
{
	RTC_TimeTypeDef rtc_time = {0};
	RTC_DateTypeDef rtc_date = {0};
	HAL_RTC_GetTime(&rtc_handle,&rtc_time,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&rtc_handle,&rtc_date,RTC_FORMAT_BIN);
	printf("rtc time :%d-%02d-%02d %02d-%02d-%02d\r\n",rtc_date.Year+2000,rtc_date.Month,rtc_date.Date,
										rtc_time.Hours,rtc_time.Minutes,rtc_time.Seconds);
	
}

void rtc_set_time(struct tm time_data)
{
	
	RTC_TimeTypeDef rtc_time = {0};
	RTC_DateTypeDef rtc_date = {0};
	rtc_time.Hours = time_data.tm_hour;
	rtc_time.Minutes = time_data.tm_min;
	rtc_time.Seconds = time_data.tm_sec;
	
	HAL_RTC_SetTime(&rtc_handle,&rtc_time,RTC_FORMAT_BIN);
	
	rtc_date.Year = time_data.tm_year-2000;
	rtc_date.Month = time_data.tm_mon;
	rtc_date.Date = time_data.tm_mday;

	HAL_RTC_SetDate(&rtc_handle,&rtc_date,RTC_FORMAT_BIN);
	while(!(__HAL_RTC_ALARM_CLEAR_FLAG(&rtc_handle,RTC_FLAG_RTOFF)));
}
void rct_set_alaem(struct tm alarm_data)
{
	RTC_AlarmTypeDef alarm_da = {0};
	alarm_da.Alarm = RTC_ALARM_A;
	alarm_da.AlarmTime.Hours = alarm_data.tm_hour;
	alarm_da.AlarmTime.Minutes = alarm_data.tm_min;
	alarm_da.AlarmTime.Seconds = alarm_data.tm_sec;
	HAL_RTC_SetAlarm_IT(&rtc_handle,&alarm_da,RTC_FORMAT_BIN);

}
