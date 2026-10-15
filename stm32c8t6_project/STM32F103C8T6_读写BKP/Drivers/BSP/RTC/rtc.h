#ifndef __RTC_H__
#define __RTC_H__

#include "sys.h"

void rtc_init(void);
uint16_t rtc_read_bkr(uint8_t bkrx);
void rtc_wirte_bkr(uint8_t bkrx,uint16_t data);

#endif
