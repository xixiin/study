#include "sys.h"
#include "uart1.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "OLED.h"
#include "esp8266.h"

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */                       
    uart1_init(115200);
    printf("running...\r\n");
	printf("你好\r\n");
//	esp8266_init(115200);
	OLED_Init();
	OLED_Clear();
	OLED_ShowString(1,1,"helloworld!");

    while(1)
    { 

    }
}

