#include "sys.h"
#include "uart1.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "OLED.h"
#include "dht11.h"
#include "string.h"

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */                       
    uart1_init(115200);
    printf("running...\r\n");
	
    uint8_t dht11_data[4];
    while(1)
    { 
        memset(dht11_data, 0, 4);
        dht11_read(dht11_data);
    }
}

