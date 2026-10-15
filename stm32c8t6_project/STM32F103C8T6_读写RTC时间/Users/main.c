#include "sys.h"
#include "uart1.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "OLED.h"
#include "rtc.h"

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */                       
    uart1_init(115200);
    printf("running...\r\n");
	rtc_init();
	
	if(rtc_read_bkr(1)!=0xA5A5)
	{
		rtc_wirte_bkr(1,0xA5A5);
		printf("读出来的值为：%X\r\n",rtc_read_bkr(1));
		struct tm timedata = {0};
		timedata.tm_year =2026;
		timedata.tm_mon =6;
		timedata.tm_mday = 24;
		timedata.tm_hour = 17;
		timedata.tm_min = 28;
		timedata.tm_sec =20;
		rtc_set_time(timedata);
	}

    while(1)
    { 
		rtc_get_time();
		delay_ms(1000);
    }
}

