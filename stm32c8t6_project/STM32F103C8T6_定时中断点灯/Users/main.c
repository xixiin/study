#include "sys.h"
#include "uart1.h"
#include "delay.h"
#include "led.h"
#include "timer.h"


void led_init(void);                       /* LED初始化函数声明 */

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
    led_init();                        /* LED初始化 */
	led_on_0();
	timer_init(5000-1,7200-1);
    while(1)
    { 
		
    }
}


