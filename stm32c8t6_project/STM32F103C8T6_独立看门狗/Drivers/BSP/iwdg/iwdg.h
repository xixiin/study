#ifndef __IWDG_H__
#define __IWDG_H__

#include "sys.h"
void iwdg_init(uint8_t psc ,uint16_t rlr);
void iwdg_feed(void);

#endif
