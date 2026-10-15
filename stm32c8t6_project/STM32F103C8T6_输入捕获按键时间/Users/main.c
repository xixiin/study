#include "sys.h"
#include "uart1.h"
#include "delay.h"
#include "led.h"
#include "ic.h"
#include "stdio.h"

                       /* LED初始化函数声明 */

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
    led_init();                        /* LED初始化 */
	in_init(65536-1,72-1);
	uart1_init(115200);
    while(1)
    { 
		presss_time_get();
		delay_ms(10);
//        led_toggle();
//        delay_ms(500);

    }
}


