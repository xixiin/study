#include "sys.h"
#include "uart1.h"
#include "delay.h"
#include "led.h"
#include "exti.h"
#include "alarm.h"



int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
    led_init();						/* LED初始化 */
	exti_init();
	alarm_init();
	led_on();	
	uint8_t alert_flag_mode = FALSE;
    while(1)
    { 
		//A是否按下
		if(get_buttonA_flag()==TRUE)
		{
			alarm_on();
			delay_ms(500);
			alarm_off();
			alert_flag_mode = TRUE;
		}
		//B是否按下
		if(get_buttonB_flag()==TRUE)
		{
			if(get_alarm_status()==alarm_status_on)
			{
				alarm_off();
			}
			else 
			{
				alarm_on();
				delay_ms(500);
				alarm_off();
			}

			alert_flag_mode = FALSE;
		}
		//警戒模式
		if(alert_flag_mode==TRUE)
		{
			if(vibrate_flag_get()==TRUE)
			{
				alarm_on();
				
			}
		}
		else
			set_vibrate_flag(FALSE);
		
        
		

    }
}


