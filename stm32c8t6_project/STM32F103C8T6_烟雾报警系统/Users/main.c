#include "sys.h"
#include "uart1.h"
#include "delay.h"
#include "ADC.h"
#include "OLED.h"
#include "led.h"
#include "key.h"
#include "alarm.h"

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
    ADC_Init();                         /* ADC初始化 */
    uart1_init(115200);
    printf("running...\r\n");
	led_init();
	OLED_Init();
	OLED_Clear();
	key_init();
	alarm_init();
	uint8_t scand_num=0;
	float high=1.8,adc_num;
    while(1)
    { 
		adc_num = (float)ADC_GetValue(ADC_CHANNEL_0)/4096.0*3.3;
        printf("ADC Value: %f\r\n", (float)(ADC_GetValue(ADC_CHANNEL_0)/4096.0*3.3)); /* 获取ADC值并打印 */
        OLED_ShowString(1,1,"ADCValue=");
		OLED_ShowNum(1,10,(int)adc_num/4096.0*3.3,1);
		OLED_ShowChar(1,11,'.');
		OLED_ShowNum(1,12,(int)((ADC_GetValue(ADC_CHANNEL_0)*100)/4096*3.3)%100,2);
		OLED_ShowString(2,1,"high=");
		OLED_ShowNum(2,5,(int)high,1);
		OLED_ShowChar(2,6,'.');
		OLED_ShowNum(2,7,(int)(high*10)%10,1);
		scand_num=key_scand();
		if(scand_num==1)
		{
			high+=0.2;
			if(high>3.3)
				high=1.2;
		}	
		else if(scand_num==2)
		{
			high-=0.2;
			if(high<1.2)
				high=1.2;
		}
		if(adc_num>high)
		{
			alarm_on();
			led_on();
		}
		else 
		{
			alarm_off();
			led_off();
		}
    }
}

