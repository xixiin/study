#include "sys.h"
#include "uart1.h"
#include "delay.h"
#include "led.h"
#include "DMA.h"


void led_init(void);                       /* LED初始化函数声明 */

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
    led_init();                        /* LED初始化 */
	uart1_init(115200);
	printf("111");
	dma_init();
	dma_transmit_start();
    while(1)
    { 
		

    }
}


