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
	pwm_init(100-1,720-1);
	uint16_t i=0;
    while(1)
    { 
	
		for(i = 0 ;i<=100;i++)
		{
			pwm_compare_set(i);
			delay_ms(20);
		}
		for(i = 100 ;i>0;i--)
		{
			pwm_compare_set(i);
			delay_ms(20);
		}
    }
}


