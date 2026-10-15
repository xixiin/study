#include "sys.h"
#include "delay.h"
#include "led.h"
#include "lcd1602.h"
#include "exti.h"
#include "alarm.h"
#include "beep.h"
#include "string.h"
#include "stdio.h"
#include "task.h"

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
    led_init();                        /* LED初始化 */
	lcd_init();
	exti_init();
	alarm_init();
	beep_init();
	char temp[20];
		sprintf(temp,"PASS...00/05");
		lcd_showline(1,1,temp);
//	uint8_t cnt=0;
    while(1)
    { 

		led1_task();
		led2_task();
		sensor_take();
		
//		if(vibrate_flag_get()==TRUE)
//		{	
////			set_vibrate_flag(FALSE);
//			cnt++;
//			if(cnt>5)
//				cnt = 0;
//		}
//		sprintf(temp,"cnt=%d",cnt);
//		lcd_showline(1,8,temp);
////		beep_toggle();
////		alarm_on();
//		delay_ms(500);
//		alarm_off();
//		led_on();
//		led_on1();
//		                           
    }
}


