#ifndef __CAN_H__
#define __CAN_H__

#include "sys.h"


void CAN_Init(void);
uint8_t CAN_receive_data(uint8_t *buf);
void CAN_send_data(uint32_t id, uint8_t* data, uint8_t length);


#endif
