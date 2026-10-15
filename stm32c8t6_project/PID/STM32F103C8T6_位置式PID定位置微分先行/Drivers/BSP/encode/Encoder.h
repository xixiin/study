#ifndef __ENCODER_H
#define __ENCODER_H

void Encoder_Init(void);
int16_t Encoder_Get(uint8_t n);   // 获取编码器增量（每次调用返回差值）
void Encoder_SetLocation(uint8_t n, int16_t Location);

#endif
