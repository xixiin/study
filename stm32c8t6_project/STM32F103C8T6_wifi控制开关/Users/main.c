#include "sys.h"
#include "uart1.h"
#include "delay.h"
#include "led.h"
#include "esp8266.h"
#include "led.h"
#include "string.h"

void led_init(void);                       /* LED初始化函数声明 */

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
    led_init();                        /* LED初始化 */
	uart_init(115200);
	esp8266_init();
	char recv_data[ESP8266_RX_BUFF_SIZE];
    while(1)
    { 
		esp8266_receive(recv_data);
		if(strstr(recv_data,"ON")!=NULL)
			led_on();
		else if(strstr(recv_data,"OFF")!=NULL)
			led_off();
//		esp8266_test();
		delay_ms(100);

    }
}


