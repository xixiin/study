#include "FreeRTOS_demo.h"
#include "delay.h"
#include "uart1.h"
#include <string.h>

void Delay_us(uint32_t us);  
//Task1特定类型的函数
void receiveTask( void * arg );
void timeTask( void * arg );

//timeTask的堆栈大小
#define timeTask_STACK_SIZE 128
//timeTask的优先级
#define timeTask_PRIORITY 3
//receiveTask的堆栈大小
#define receiveTask_STACK_SIZE 128
//receiveTask的优先级
#define receiveTask_PRIORITY 3



TaskHandle_t receiveTaskHandle = NULL;
TaskHandle_t timeTaskHandle = NULL;


void FreeRTOS_Start(void)
{



    xTaskCreate(receiveTask, "RECEIVE", receiveTask_STACK_SIZE, NULL, receiveTask_PRIORITY, &receiveTaskHandle);
    xTaskCreate(timeTask, "TIME", timeTask_STACK_SIZE, NULL, timeTask_PRIORITY, &timeTaskHandle);
    // Start the scheduler
    vTaskStartScheduler();

    // If all is well, the scheduler will now be running, and the following line will never be reached.
    // If the following line does execute, then there was insufficient FreeRTOS heap memory available
    // for the idle and/or timer tasks to be created.
    for(;;);
}
typedef enum
{
    key_NONE = 0,
    key1 = 1,
    key2 = 2,
    key3 = 3
} key_number;
uint8_t key_value=0;

void receiveTask( void * arg )
{

    while(1)
    {
        //接收任务等待通知，等待接收中断通知
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY); //等待通知，阻塞任务直到接收到通知
        //接收到通知后，处理接收到的数据
            /* 处理接收到的数据（uart1_rx_buf 在 ISR 中已填充并添加了 '\0'） */
            printf("Received: %s\r\n", (char*)uart1_rx_buf);
            /* 清空缓冲区，准备下一次接收 */
            uart1_rx_clear();

    }
        

}
void Delay_us(uint32_t us)         
{
    uint32_t count = us*72/9;
    while(count--)
    {
        __NOP();
    }

}
uint8_t tasktime[128];
void timeTask( void * arg )
{
    while(1)
    {
        taskENTER_CRITICAL();
        vTaskGetRunTimeStatistics((char*)tasktime, sizeof(tasktime));
        printf("Task run time statistics:\n%s\n", tasktime);
        taskEXIT_CRITICAL();
        vTaskDelay(2000);
    }

}
