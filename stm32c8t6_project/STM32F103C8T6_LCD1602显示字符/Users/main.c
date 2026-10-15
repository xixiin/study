#include "sys.h"
#include "uart1.h"
#include "delay.h"
#include "led.h"
#include "lcd1602.h"


void led_init(void);                       /* LED初始化函数声明 */

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
    led_init();                        /* LED初始化 */
	lcd_init();


    while(1)
    { 
		led_toggle0();
		delay_ms(200);
		lcd_showline(1,1,"hello");
		lcd_showline(2,1,"");
//		                           
    }
}


