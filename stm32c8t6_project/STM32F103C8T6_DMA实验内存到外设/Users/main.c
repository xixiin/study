#include "sys.h"
#include "uart1.h"
#include "delay.h"
#include "led.h"
#include "DMA.h"

extern UART_HandleTypeDef uart1_handle;
uint8_t tx_buf[1000] ;  /* 发送数据缓冲区 */

void led_init(void);                       /* LED初始化函数声明 */

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
    led_init();                        /* LED初始化 */
	uart1_init(115200);
	printf("111");
	dma_init();

    int i;
    for(i=0; i<1000; i++)
    {
        tx_buf[i] = 'A';  /* 填充发送数据缓冲区 */
    }
    HAL_UART_Transmit_DMA(&uart1_handle, tx_buf, 1000); /* 通过DMA发送数据 */
    while(1)
    { 
		

    }
}


