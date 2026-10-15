#include "sys.h"
#include "uart1.h"
#include "delay.h"
#include "led.h"
#include "counter.h"
#include "stdio.h"

                       /* LED初始化函数声明 */

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
    led_init();                        /* LED初始化 */
	counter_init(65536-1,0);
	uart1_init(115200);
	printf("hello");
    while(1)
    { 
		counter_get();
//        led_toggle();
//        delay_ms(500);

    }
}


