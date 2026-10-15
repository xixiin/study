#include "lcd1602.h"
#include "delay.h"

//RS
#define RS_GPIO_PORT   GPIOB
#define RS_GPIO_Pin   GPIO_PIN_13
#define RS_HIGH     HAL_GPIO_WritePin(RS_GPIO_PORT,RS_GPIO_Pin,GPIO_PIN_SET);
#define RS_LOW     HAL_GPIO_WritePin(RS_GPIO_PORT,RS_GPIO_Pin,GPIO_PIN_RESET);
//RW
#define RW_GPIO_PORT   GPIOB
#define RW_GPIO_Pin   GPIO_PIN_14
#define RW_HIGH     HAL_GPIO_WritePin(RW_GPIO_PORT,RW_GPIO_Pin,GPIO_PIN_SET);
#define RW_LOW     HAL_GPIO_WritePin(RW_GPIO_PORT,RW_GPIO_Pin,GPIO_PIN_RESET);
//EN
#define EN_GPIO_PORT   GPIOB
#define EN_GPIO_Pin   GPIO_PIN_15
#define EN_HIGH     HAL_GPIO_WritePin(EN_GPIO_PORT,EN_GPIO_Pin,GPIO_PIN_SET);
#define EN_LOW     HAL_GPIO_WritePin(EN_GPIO_PORT,EN_GPIO_Pin,GPIO_PIN_RESET);

void lcd_init()
{
	lcd_gpio_init();
	lcd_start();
}

void lcd_gpio_init()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef gpio_init={0};
	gpio_init.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|
					GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
	gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init.Pull = GPIO_PULLUP;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOA, &gpio_init);
	gpio_init.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init.Pull = GPIO_PULLUP;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOB, &gpio_init);

}
void lcd_start()
{
	delay_ms(15);
	lcd_write_cmd(0x38);
	delay_ms(5);
	lcd_write_cmd(0x38);
	lcd_write_cmd(0x08);
	lcd_write_cmd(0x01);
	lcd_write_cmd(0x06);
	lcd_write_cmd(0x0c);
	
}
	
void lcd_write_cmd(char cmd)
{
	RS_LOW;
	RW_LOW;
	EN_LOW;
	GPIOA->ODR = cmd;
	delay_ms(5);
	EN_HIGH;
	delay_ms(5);
	EN_LOW;
	
}

void lcd_write_data(char data)
{
	RS_HIGH;
	RW_LOW;
	EN_LOW;
	GPIOA->ODR = data;
	delay_ms(5);
	EN_HIGH;
	delay_ms(5);
	EN_LOW;
	
}

void lcd_showchar(char data)
{
	lcd_write_cmd(0x80+0x02);
	lcd_write_data(data);
	
	
}
void lcd_showline(char row,char col ,char *string)
{
	switch (row)
	{
		case 1:
		{
			lcd_write_cmd(0x80+col);
			while(*string)
			{
				lcd_write_data(*string);
				string++;
			}
			break;
		}
		case 2:
		{
			lcd_write_cmd(0x80+0x40+col);
			while(*string)
			{
				lcd_write_data(*string);
				string++;
			}
			break;
		}			
			
			
	}

}
