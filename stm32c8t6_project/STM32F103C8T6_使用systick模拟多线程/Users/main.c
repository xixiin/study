#include "sys.h"
#include "uart1.h"
#include "delay.h"
#include "led.h"
#include "task.h"


void led_init(void);                       /* LED初始化函数声明 */

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
    led_init();                        /* LED初始化 */
    uart1_init(115200);
    printf("running...\r\n");
	led_toggle0();
    while(1)
    { 
        task1();
		task2();
//		led_toggle0(); 
//		delay_ms(200);
//		led_toggle1();
//		delay_ms(500);
//		led_toggle0();
//		led_toggle1();
    }
}


