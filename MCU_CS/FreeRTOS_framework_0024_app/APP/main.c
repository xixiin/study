#include "SWM320.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "task_start.h"
#include "task_host.h"
#include "task_lisen.h"
#include "task_ctrl.h"
/*
	start
		初始化一些外设
		创建host   主机任务
		创建listen 监听任务
		创建ctrl   工作任务
		
	while(1)
	{
		//网络服务
	}
	创建了多个任务后，打印是乱的，不完整
	解决方法
	(1)对打印的操作上锁
	（2）通过队列的方式依次打印（缺点浪费内存）

*/

void SerialInit(void);
//extern void creat_task(void);
int main(void)
{ 	
 	SystemInit();

	SerialInit();
	printf("----------------------------------\r\n");
	printf("system run APP\r\n");
	

	creat_task();
	vTaskStartScheduler();
}






void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTA, PIN2, FUNMUX0_UART0_RXD, 1);	//GPIOA.2配置为UART0输入引脚
	PORT_Init(PORTA, PIN3, FUNMUX1_UART0_TXD, 0);	//GPIOA.3配置为UART0输出引脚
 	
 	UART_initStruct.Baudrate = 115200;
	UART_initStruct.RXThresholdIEn = 0;
	UART_initStruct.TXThresholdIEn = 0;
	UART_initStruct.TimeoutIEn = 0;
 	UART_Init(UART0, &UART_initStruct);
	UART_Open(UART0);
}

/****************************************************************************************************************************************** 
* 函数名称: fputc()
* 功能说明: printf()使用此函数完成实际的串口打印动作
* 输    入: int ch		要打印的字符
*			FILE *f		文件句柄
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
int fputc(int ch, FILE *f)
{
 	while(UART_IsTXFIFOFull(UART0));
	
	UART_WriteByte(UART0, ch);
 	
	return ch;
}


/******************************************************************************
 * Debug hooks (called by FreeRTOS when an assertion or stack overflow occurs)
 ******************************************************************************/
void vAssertCalled( const char *pcFile, unsigned long ulLine )
{
	printf("G_ERR %s:%lu\r\n", pcFile, ulLine);
	__disable_irq();
	for(;;);
}

void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
	printf("G_STKOF %s\r\n", pcTaskName);
	__disable_irq();
	for(;;);
}
