#include "sys.h"
#include "uart1.h"
#include "delay.h"
#include "led.h"
#include "exti.h"


void led_init(void);                       /* LED初始化函数声明 */

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
    led_init();						/* LED初始化 */
	exti_init();	
    uart1_init(115200);
    printf("running...\r\n");
    while(1)
    { 
		if(vibrate_flag_get() == TRUE)
		{
			led_on();
			delay_ms(200);
		}
		else 
			led_off();

		
        
		

    }
}


