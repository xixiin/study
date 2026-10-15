#include "sys.h"
#include "uart1.h"
#include "delay.h"
#include "led.h"
#include "pwm.h"


void led_init(void);                       

int main(void)
{
    HAL_Init();                         
    stm32_clock_init(RCC_PLL_MUL9); 
    led_init(); 
	uart1_init(115200);	
	printf("hello world");
	pwm_init(2000 - 1,720-1);
//	uint16_t i=0;
    while(1)
    { 
		sg90_angle_set(120);
//		for(i = 0 ;i<=180;i++)
//		{
//			sg90_angle_set(i);
//			delay_ms(20);
//		}
//		for(i = 180 ;i>0;i--)
//		{
//			sg90_angle_set(i);
//			delay_ms(20);
//		}
    }
}


