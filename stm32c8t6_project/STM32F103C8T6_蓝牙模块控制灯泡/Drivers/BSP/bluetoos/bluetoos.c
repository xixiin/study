#include "sys.h"
#include "bluetoos.h"
#include "string.h"
#include "stdarg.h"
#include "stdio.h"
#include "led.h"

UART_HandleTypeDef uart3_handle;                                            /* UART1句柄 */

uint8_t uart3_rx_buf[UART3_RX_BUF_SIZE];                                    /* UART1接收缓冲区 */
uint16_t uart3_rx_len = 0;                                                  /* UART1接收字符长度 */


/**
 * @brief       串口1初始化函数
 * @param       baudrate: 波特率, 根据自己需要设置波特率值
 * @retval      无
 */
void bt_init(uint32_t baudrate)
{
    /*UART1 初始化设置*/
    uart3_handle.Instance = USART3;                                         /* USART1 */
    uart3_handle.Init.BaudRate = baudrate;                                  /* 波特率 */
    uart3_handle.Init.WordLength = UART_WORDLENGTH_8B;                      /* 字长为8位数据格式 */
    uart3_handle.Init.StopBits = UART_STOPBITS_1;                           /* 一个停止位 */
    uart3_handle.Init.Parity = UART_PARITY_NONE;                            /* 无奇偶校验位 */
    uart3_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;                      /* 无硬件流控 */
    uart3_handle.Init.Mode = UART_MODE_TX_RX;                               /* 收发模式 */
    HAL_UART_Init(&uart3_handle);                                           /* HAL_UART_Init()会使能UART1 */
}



/**
 * @brief       UART1接收缓冲区清除
 * @param       无
 * @retval      无
 */
void uart3_rx_clear(void)
{
    memset(uart3_rx_buf, 0, sizeof(uart3_rx_buf));                          /* 清空接收缓冲区 */
    uart3_rx_len = 0;                                                       /* 接收计数器清零 */
}

/**
 * @brief       串口1中断服务函数
 * @note        在此使用接收中断及空闲中断，实现不定长数据收发
 * @param       无
 * @retval      无
 */
void USART3_IRQHandler(void)
{
    uint8_t receive_data = 0;   
    if(__HAL_UART_GET_FLAG(&uart3_handle, UART_FLAG_RXNE) != RESET){        /* 获取接收RXNE标志位是否被置位 */
        if(uart3_rx_len >= sizeof(uart3_rx_buf))                            /* 如果接收的字符数大于接收缓冲区大小， */
            uart3_rx_len = 0;                                               /* 则将接收计数器清零 */
        HAL_UART_Receive(&uart3_handle, &receive_data, 1, 1000);            /* 接收一个字符 */
        uart3_rx_buf[uart3_rx_len++] = receive_data;                        /* 将接收到的字符保存在接收缓冲区 */
    }

    if (__HAL_UART_GET_FLAG(&uart3_handle, UART_FLAG_IDLE) != RESET)        /* 获取接收空闲中断标志位是否被置位 */
    {
        printf("recv: %s\r\n", uart3_rx_buf);                               /* 将接收到的数据打印出来 */
		
		if(strstr((char *)uart3_rx_buf,"ON")!= NULL||strstr((char *)uart3_rx_buf,"on")!= NULL)
		{
			led_on();
			bt_send("on\r\n");
		}
		else if(strstr((char *)uart3_rx_buf,"OFF")!= NULL||strstr((char *)uart3_rx_buf,"off")!= NULL)
		{
			led_off();
			bt_send("off\r\n");
		}
        uart3_rx_clear();
        __HAL_UART_CLEAR_IDLEFLAG(&uart3_handle);                           /* 清除UART总线空闲中断 */
    }
}



void bluetoos_send(char *sendbuff,uint8_t size)
{
	HAL_UART_Transmit(&uart3_handle,(uint8_t *)sendbuff,size,100);

}

void bt_send(char *format,...)
{
	uint8_t sendbuf[128];
	va_list arg;
	va_start(arg, format);
	vsprintf((char*)sendbuf,format,arg);
	va_end(arg);
	HAL_UART_Transmit(&uart3_handle,sendbuf,strlen((char*)sendbuf),100);


}	
