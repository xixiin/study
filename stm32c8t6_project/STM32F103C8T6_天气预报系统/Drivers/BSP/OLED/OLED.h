#ifndef __OLED_H
#define __OLED_H

#include "sys.h"
#define null_slect 			0
#define year_slect 			1
#define mon_slect 			2
#define day_slect 			3
#define hour_slect 			4
#define min_slect 			5
#define second_slect 		6
#define alarm_hour_slect 	7
#define alarm_min_slect 	8
#define alarm_second_slect 	9

#define OFF    0
#define ON 	   1


void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowChinese(uint8_t Line, uint8_t Column, uint16_t index, uint8_t font[][32]);
void OLED_ShowChineseStr(uint8_t Line, uint8_t Column, uint16_t *str, uint8_t font[][32]);
void oled_show(void);
void oled_clear(uint8_t Line, uint8_t Column);
void oled_weather(void);
#endif
