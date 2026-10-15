#include "sys.h"
#include "uart1.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "OLED.h"
#include "esp8266.h"
#include "FreeRTOS_demo.h"
#include "timer.h"

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */                       
    uart1_init(115200);
    printf("running...\r\n");
	printf("你好\r\n");
//	esp8266_init(115200);
	led_init();
	OLED_Init();
	OLED_Clear();
    key_init();
	OLED_ShowString(1,1,"helloFreeRTOS");
    // timer_init(10-1,72-1);
	FreeRTOS_Start();
	
	
    while(1)
    { 

    }
}

