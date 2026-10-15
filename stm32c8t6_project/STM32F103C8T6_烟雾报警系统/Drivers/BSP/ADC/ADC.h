#ifndef __ADC_H
#define __ADC_H

#include "sys.h"

void ADC_Init(void);
uint32_t ADC_GetValue(uint32_t channel);

#endif /* __ADC_H */
