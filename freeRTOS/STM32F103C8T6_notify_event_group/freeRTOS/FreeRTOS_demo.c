#include "FreeRTOS_demo.h"
#include "delay.h"

void Delay_us(uint32_t us);  
//Task1特定类型的函数
void keyTask( void * arg );
void readTask( void * arg );
void timeTask( void * arg );

void ledtask(void * arg);
#define ledtask_STACK_SIZE 128
#define ledtask_PRIORITY 3
TaskHandle_t ledtaskHandle = NULL;

//timeTask的堆栈大小
#define timeTask_STACK_SIZE 128
//timeTask的优先级
#define timeTask_PRIORITY 3
//readTask的堆栈大小
#define readTask_STACK_SIZE 128
//readTask的优先级
#define readTask_PRIORITY 3
//keyTask的堆栈大小
#define keyTask_STACK_SIZE 128
//keyTask的优先级
#define keyTask_PRIORITY 3

typedef enum
{
    key_NONE = 0,
    key1 = 1,
    key2 = 2,
    key3 = 3
} key_number;

TaskHandle_t keyTaskHandle = NULL;
TaskHandle_t readTaskHandle = NULL;
TaskHandle_t timeTaskHandle = NULL;
//接收按键
QueueHandle_t keyQueue ;
//接收串口数据
QueueHandle_t uartQueue;
QueueHandle_t item; 
QueueSetHandle_t queueSet;
EventGroupHandle_t xEventGroupHandle;

void FreeRTOS_Start(void)
{

    //创建事件标志组
    // xEventGroupHandle = xEventGroupCreate();
    //创建串口二值信号量
    item = xSemaphoreCreateBinary(); // Create a binary semaphore
    //创建按键队列和串口队列
    keyQueue = xQueueCreate(10, sizeof(key_number)); // Create a queue to hold 10 key_number items

    xQueueAddToSet(keyQueue, queueSet);
    xQueueAddToSet(uartQueue, queueSet);
    // Create tasks, queues, and other FreeRTOS objects here
    xTaskCreate(ledtask, "LED", ledtask_STACK_SIZE, NULL, ledtask_PRIORITY, &ledtaskHandle);
    xTaskCreate(keyTask, "KEY", keyTask_STACK_SIZE, NULL, keyTask_PRIORITY, &keyTaskHandle);
    xTaskCreate(readTask, "READ", readTask_STACK_SIZE, NULL, readTask_PRIORITY, &readTaskHandle);
    // xTaskCreate(timeTask, "TIME", timeTask_STACK_SIZE, NULL, timeTask_PRIORITY, &timeTaskHandle);

    // Start the scheduler
    vTaskStartScheduler();

    // If all is well, the scheduler will now be running, and the following line will never be reached.
    // If the following line does execute, then there was insufficient FreeRTOS heap memory available
    // for the idle and/or timer tasks to be created.
    for(;;);
}

uint8_t key_value=0;
void keyTask( void * arg )
{
    while(1)
    {
        key_value = key_scand();
        key_number key = (key_number)key_value;
       
        if (key != key_NONE) {
            
            xQueueSend(keyQueue, &key, portMAX_DELAY); // Send data to the queue
        }

        vTaskDelay(10); 
    }

}
void readTask( void * arg )
{
    while(1)
    {

        key_number receivedKey;
        BaseType_t result = xQueueReceive(keyQueue, &receivedKey, portMAX_DELAY); // Receive data from the queue
        if (result == pdTRUE) {
            taskENTER_CRITICAL();
            switch (receivedKey) {
                case key1:
                    //在事件标志中设置标志位0
                    // xEventGroupSetBits(xEventGroupHandle, (1 << 0)); // Set bit 0
                    xTaskNotify(ledtaskHandle, (1 << 0), eSetBits); // Set bit 0
                    break;
                case key2:
                    //在事件标志中设置标志位1
                    // xEventGroupSetBits(xEventGroupHandle, (1 << 1)); // Set bit 1
                    xTaskNotify(ledtaskHandle, (1 << 1), eSetBits); // Set bit 1
                    break;
                case key3:
                    //在事件标志中设置标志位2
                    // xEventGroupSetBits(xEventGroupHandle, (1 << 2)); // Set bit 2
                    xTaskNotify(ledtaskHandle, (1 << 2), eSetBits); // Set bit 2
                    break;
                default:
                    break;
            }
            taskEXIT_CRITICAL();
        
        }
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

void ledtask(void * arg)
{
    while(1)
    {
        //等待事件标志组中的标志位0被设置
    //   xEventGroupWaitBits(
    //         xEventGroupHandle,    // 事件标志组句柄
    //         0x07, // 等待的标志位掩码
    //         pdFALSE,               // 等待后清除标志位
    //         pdTRUE,              // 等待所有标志位都被设置
    //         portMAX_DELAY         // 无限等待
    //     );
    //当进入方法开始挂起等待条件开始时 =》是否需要清除标志位？  需要清除标志位
    //当退出方法时 =》是否需要清除标志位？  不需要清除标志位
    uint32_t Notify_value = 0;
        xTaskNotifyWait(0x00, 0x00, &Notify_value, portMAX_DELAY);
        if(Notify_value == 0x07) {
            //切换LED状态
            led_toggle1();
            led_toggle2();
            led_toggle3();
            vTaskDelay(500); 
            led_toggle1();
            led_toggle2();
            led_toggle3();
           vTaskDelay(500); 
                //手动清楚通知事件
            xTaskNotifyStateClear(NULL); // Clear the notification state for the current task
             ulTaskNotifyValueClear(NULL, 0x07); // Clear all notification bits for the current task
        }
     

        //手动清除事件标志组中的标志位
        // xEventGroupClearBits(xEventGroupHandle, (1 << 0) | (1 << 1) | (1 << 2));


    }
}