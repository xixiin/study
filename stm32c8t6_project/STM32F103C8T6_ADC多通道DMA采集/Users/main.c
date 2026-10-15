#include "sys.h"
#include "uart1.h"
#include "delay.h"
#include "ADC.h"

uint16_t adc_value[2]={0};

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
    adc_dma_init((uint32_t *)&adc_value);                         /* ADC初始化 */
    uart1_init(115200);
    printf("running...\r\n");
    while(1)
    { 
        printf("ADC0: %f\r\n", (float)adc_value[0]/4096.0*3.3); /* 获取ADC值并打印 */
        printf("ADC1: %f\r\n", (float)adc_value[1]/4096.0*3.3); /* 获取ADC值并打印 */
        delay_ms(1000); /* 延时1秒 */
    }
}

