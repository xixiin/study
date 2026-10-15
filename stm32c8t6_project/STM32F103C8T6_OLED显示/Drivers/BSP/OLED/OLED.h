#ifndef __OLED_H__
#define __OLED_H__

#include "sys.h"


#define OLED_I2C_SCL_CLK()		__HAL_RCC_GPIOB_CLK_ENABLE()
#define OLED_I2C_SCL_PORT		GPIOB
#define OLED_I2C_SCL_PIN		GPIO_PIN_8

#define OLED_I2C_SDA_CLK()		__HAL_RCC_GPIOB_CLK_ENABLE()
#define OLED_I2C_SDA_PORT		GPIOB
#define OLED_I2C_SDA_PIN		GPIO_PIN_9

#define OLED_I2C_SCL_SET() 		HAL_GPIO_WritePin(OLED_I2C_SCL_PORT,OLED_I2C_SCL_PIN,GPIO_PIN_SET)
#define OLED_I2C_SCL_RESET() 	HAL_GPIO_WritePin(OLED_I2C_SCL_PORT,OLED_I2C_SCL_PIN,GPIO_PIN_RESET)

#define OLED_I2C_SDA_SET() 		HAL_GPIO_WritePin(OLED_I2C_SDA_PORT,OLED_I2C_SDA_PIN,GPIO_PIN_SET)
#define OLED_I2C_SDA_RESET() 	HAL_GPIO_WritePin(OLED_I2C_SDA_PORT,OLED_I2C_SDA_PIN,GPIO_PIN_RESET)

void oled_init(void);
void oled_i2c_write_data(uint8_t data);
void oled_i2c_write_cmd(uint8_t cmd);
void oled_file(uint8_t data);
void oled_set_cousor(uint8_t x,uint8_t y);
void oled_show_char(uint8_t x,uint8_t y ,uint8_t num,uint8_t size);
void oled_show_string(uint8_t x,uint8_t y ,char *p,uint8_t size);
void oled_show_chinese(uint8_t x,uint8_t y,uint8_t N,uint8_t size);

#endif
