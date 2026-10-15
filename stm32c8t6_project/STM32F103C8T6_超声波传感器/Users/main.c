#include "sys.h"
#include "uart1.h"
#include "delay.h"
#include "led.h"
#include "hcsr04.h"


void led_init(void);                       /* LED初始化函数声明 */

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
	hcsr04_init();
	uart1_init(115200);
    while(1)
    { 
		printf("dis :%.2f\r\n",hcsr04_get_length());
		delay_ms(1000);
    }
}


