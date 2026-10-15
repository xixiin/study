#ifndef __LCD1602_H__
#define __LCD1602_H__

#include "sys.h"
void lcd_init(void);
void lcd_gpio_init(void);
void lcd_start(void);
void lcd_write_cmd(char cmd);
void lcd_write_data(char data);
void lcd_showchar(char data);
void lcd_showline(char row,char col ,char *string);

#endif
