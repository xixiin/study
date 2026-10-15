#include "sys.h"
#include "uart1.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "OLED.h"
#include "W25Q128.h"

uint8_t data_write[4] = {0xaa,0xbb,0xcc,0xdd};
uint8_t data_read[4] = {0};

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */                       
    uart1_init(115200);
    printf("running...\r\n");
	OLED_Init();
	OLED_Clear();
	w25q128_init();
	uint16_t devie_id = w25q128_read_id();
	printf("device_id:%X\r\n",devie_id);
//	w25q128_erase_sector(0x000000);
//	w25q128_write_page(0x000000,data_write,4);
	w25q128_read_data(0x000000,data_read,4);
	OLED_ShowHexNum(1,1,data_read[0],2);
	OLED_ShowHexNum(1,5,data_read[1],2);
	OLED_ShowHexNum(2,1,data_read[2],2);
	OLED_ShowHexNum(2,5,data_read[3],2);
    while(1)
    { 

    }
}

