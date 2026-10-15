#include "sys.h"
#include "uart1.h"
#include "delay.h"
#include "led.h"
#include "iwdg.h"


int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
    led_init();                         /* LED初始化 */
    uart1_init(115200);
    printf("gogogo\r\n");
	iwdg_init(IWDG_PRESCALER_32,1250);
	if(__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST)!=RESET)
	{
		printf("看门狗复位\r\n");
		__HAL_RCC_CLEAR_RESET_FLAGS();
	}
	else
	{
		printf("外部复位\r\n");
		
	}
    while(1)
    { 
		delay_ms(1500);
		iwdg_feed();
		printf("succeed\r\n");
    }
}

