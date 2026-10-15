#include "sys.h"
#include "uart1.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "OLED.h"
#include "cJSON.h"
#include "esp8266.h"
#include "weather.h"

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */                       
    uart1_init(115200);
	esp8266_init(115200);
    printf("running...\r\n");
	printf("你好\r\n");
    key_init();
	OLED_Init();
	OLED_Clear();
	oled_weather();
	get_3dayweather();
    show_weather(0);
	uint8_t keynum = 0;
    uint8_t day = 0;
    while(1)
    { 
        keynum = key_scand();
        if(keynum == 1)
        {
            day++;
            day = day % 3; // 循环显示三天的天气
            show_weather(day);
        }
        else if(keynum == 2)
        {

            printf("更新天气信息...\r\n");
                get_3dayweather();
                show_weather(day);
                day = 0; // 重置为第一天
        }

        
    }
}

