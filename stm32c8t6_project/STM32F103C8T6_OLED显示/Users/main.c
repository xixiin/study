#include "sys.h"
#include "uart1.h"
#include "delay.h"
#include "OLED.h"


//{0x00,0x00,0xC0,0x38,0xE0,0x00,0x00,0x00,0x20,0x3C,0x23,0x02,0x02,0x27,0x38,0x20},/*"A",0*/
//{0x20,0x24,0x24,0x24,0xFE,0x23,0x22,0x20,0x20,0xFF,0x20,0x22,0x2C,0xA0,0x20,0x00},
//{0x00,0x08,0x48,0x84,0x7F,0x02,0x41,0x40,0x20,0x13,0x0C,0x14,0x22,0x41,0xF8,0x00},/*"我",0*/


int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
    uart1_init(115200);
    printf("running...\r\n");
	oled_init();
	oled_file(0x00);
//	oled_i2c_write_cmd(0xB0); A
//	oled_i2c_write_cmd(0x00);
//	oled_i2c_write_cmd(0x10);
//	oled_set_cousor(0,1);

//	oled_i2c_write_data(0x00);
//	oled_i2c_write_data(0x00);
//	oled_i2c_write_data(0xC0);
//	oled_i2c_write_data(0x38);
//	oled_i2c_write_data(0xE0);
//	oled_i2c_write_data(0x00);
//	oled_i2c_write_data(0x00);
//	oled_i2c_write_data(0x00);
//	oled_set_cousor(0,2);
//	oled_i2c_write_data(0x20);
//	oled_i2c_write_data(0x3C);
//	oled_i2c_write_data(0x23);
//	oled_i2c_write_data(0x02);
//	oled_i2c_write_data(0x02);
//	oled_i2c_write_data(0x27);
//	oled_i2c_write_data(0x38);
//	oled_i2c_write_data(0x20);	
//	oled_set_cousor(0,1);		我

//	oled_i2c_write_data(0x20);
//	oled_i2c_write_data(0x24);
//	oled_i2c_write_data(0x24);
//	oled_i2c_write_data(0x24);
//	oled_i2c_write_data(0xFE);
//	oled_i2c_write_data(0x23);
//	oled_i2c_write_data(0x22);
//	oled_i2c_write_data(0x20);
//	oled_i2c_write_data(0x20);
//	oled_i2c_write_data(0xFF);
//	oled_i2c_write_data(0x20);
//	oled_i2c_write_data(0x22);
//	oled_i2c_write_data(0x2C);
//	oled_i2c_write_data(0xA0);
//	oled_i2c_write_data(0x20);
//	oled_i2c_write_data(0x00);
//	oled_set_cousor(0,2);

//	oled_i2c_write_data(0x00);
//	oled_i2c_write_data(0x08);
//	oled_i2c_write_data(0x48);
//	oled_i2c_write_data(0x84);
//	oled_i2c_write_data(0x7F);
//	oled_i2c_write_data(0x02);
//	oled_i2c_write_data(0x41);
//	oled_i2c_write_data(0x40);
//	oled_i2c_write_data(0x20);
//	oled_i2c_write_data(0x13);
//	oled_i2c_write_data(0x0C);
//	oled_i2c_write_data(0x14);
//	oled_i2c_write_data(0x22);
//	oled_i2c_write_data(0x41);
//	oled_i2c_write_data(0xF8);
//	oled_i2c_write_data(0x00);	
//	oled_show_char(0,0,'x',16);
//	oled_show_char(8,0,'a',16);
//	oled_show_string(16,0,"hello",16);
	uint8_t i=0;
	for(i=0;i<5;i++)
		oled_show_chinese(i*16,0,i,16);

    while(1)
    { 
		
    }
}


