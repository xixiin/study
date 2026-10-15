#ifndef __RTC_H__
#define __RTC_H__

#include "sys.h"
#include "time.h"

void rtc_init(void);
uint16_t rtc_read_bkr(uint8_t bkrx);
void rtc_wirte_bkr(uint8_t bkrx,uint16_t data);
void rtc_get_time(uint8_t *time_data);
void rtc_set_time(uint8_t *time_data);
void rct_set_alaem(uint8_t *alarm_data);
void rtc_get_alarm(uint8_t *alarm_data);

#endif
