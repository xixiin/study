#include "sys.h"
#include "uart1.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "OLED.h"
#include "rtc.h"

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */                       
    uart1_init(115200);
    printf("running...\r\n");
	rtc_init();
	rtc_wirte_bkr(1,0xA5A5);
	printf("读出来的值为：%X\r\n",rtc_read_bkr(1));
    while(1)
    { 

    }
}

