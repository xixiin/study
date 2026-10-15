#include "FreeRTOS_demo.h"
#include "delay.h"

void Delay_us(uint32_t us);  
//Task1特定类型的函数
void keyTask( void * arg );
void readTask( void * arg );
void timeTask( void * arg );

void uartTask( void * arg );
#define uartTask_STACK_SIZE 128
#define uartTask_PRIORITY 3
TaskHandle_t uartTaskHandle = NULL;

void uartreadTask( void * arg );
#define uartreadTask_STACK_SIZE 128
#define uartreadTask_PRIORITY 3
TaskHandle_t uartreadTaskHandle = NULL;

//队列集读取任务
void queueSetTask( void * arg );
#define queueSetTask_STACK_SIZE 128
#define queueSetTask_PRIORITY 3
TaskHandle_t queueSetTaskHandle = NULL;

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

void FreeRTOS_Start(void)
{
    //创建串口二值信号量
    item = xSemaphoreCreateBinary(); // Create a binary semaphore
    //创建按键队列和串口队列
    keyQueue = xQueueCreate(10, sizeof(key_number)); // Create a queue to hold 10 key_number items
    uartQueue = xQueueCreate(10, sizeof(char *)); // Create a queue to hold 10 uint8_t items
    //创建队列集
    queueSet = xQueueCreateSet(2); // Create a queue set to hold 2 items
    //添加队列到队列集
    xQueueAddToSet(keyQueue, queueSet);
    xQueueAddToSet(uartQueue, queueSet);
    // Create tasks, queues, and other FreeRTOS objects here
    xTaskCreate(keyTask, "KEY", keyTask_STACK_SIZE, NULL, keyTask_PRIORITY, &keyTaskHandle);
    // xTaskCreate(readTask, "READ", readTask_STACK_SIZE, NULL, readTask_PRIORITY, &readTaskHandle);
    // xTaskCreate(timeTask, "TIME", timeTask_STACK_SIZE, NULL, timeTask_PRIORITY, &timeTaskHandle);
    xTaskCreate(uartTask, "UART", uartTask_STACK_SIZE, NULL, uartTask_PRIORITY, &uartTaskHandle);
    // xTaskCreate(uartreadTask, "UARTREAD", uartreadTask_STACK_SIZE, NULL, uartreadTask_PRIORITY, &uartreadTaskHandle);
    //创建队列集读取任务
    xTaskCreate(queueSetTask, "QUEUESET", queueSetTask_STACK_SIZE, NULL, queueSetTask_PRIORITY, &queueSetTaskHandle);
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
                    led_toggle1();
                    break;
                case key2:
                    led_toggle2();
                    break;
                case key3:
                    led_toggle3();
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

void uartTask( void * arg )
{
    while(1)
    {
        /* 等待信号量：ISR 收到完整一帧数据后会释放 */
        BaseType_t result = xSemaphoreTake(item, portMAX_DELAY);
        if(result == pdTRUE)
        {
            /* 将数据指针发送到队列，由 uartreadTask 负责消费和清空缓冲区 */
            char *data = (char *)uart1_rx_buf;
            xQueueSend(uartQueue, &data, portMAX_DELAY);
        }
    }
}

void uartreadTask( void * arg )
{
    while(1)
    {
        char *receivedData;
        BaseType_t result = xQueueReceive(uartQueue, &receivedData, portMAX_DELAY); // Receive data from the queue
        if (result == pdTRUE) {
            taskENTER_CRITICAL();
            printf("Received UART data: %s\n", receivedData);
            taskEXIT_CRITICAL();
            //led1 => 翻转led1
            //led2 => 翻转led2
            //led3 => 翻转led3
            //all => 翻转所有led
            if(strcmp(receivedData, "led1") == 0)
            {
                led_toggle1();
            }
            else if(strcmp(receivedData, "led2") == 0)
            {
                led_toggle2();
            }
            else if(strcmp(receivedData, "led3") == 0)
            {
                led_toggle3();
            }
            else if(strcmp(receivedData, "all") == 0)
            {
                led_toggle1();
                led_toggle2();
                led_toggle3();
            }
            /* 数据消费完毕，清空缓冲区准备下一次接收 */
            uart1_rx_clear();
        }
    }
}
void queueSetTask( void * arg )
{
    while(1)
    {
        /* 等待队列集中的任意一个队列有数据 */
        QueueSetMemberHandle_t activatedMember = xQueueSelectFromSet(queueSet, portMAX_DELAY);
        
        if (activatedMember == keyQueue) {
            key_number receivedKey;
            BaseType_t result = xQueueReceive(keyQueue, &receivedKey, 0); // Receive data from the queue
            if (result == pdTRUE) {
                taskENTER_CRITICAL();
                switch (receivedKey) {
                    case key1:
                        led_toggle1();
                        break;
                    case key2:
                        led_toggle2();
                        break;
                    case key3:
                        led_toggle3();
                        break;
                    default:
                        break;
                }
                taskEXIT_CRITICAL();
            }
        } else if (activatedMember == uartQueue) {
            char *receivedData;
            BaseType_t result = xQueueReceive(uartQueue, &receivedData, 0); // Receive data from the queue
            if (result == pdTRUE) {
                taskENTER_CRITICAL();
                printf("Received UART data: %s\n", receivedData);
                taskEXIT_CRITICAL();
                //led1 => 翻转led1
                //led2 => 翻转led2
                //led3 => 翻转led3
                //all => 翻转所有led
                if(strcmp(receivedData, "led1") == 0)
                {
                    led_toggle1();
                }
                else if(strcmp(receivedData, "led2") == 0)
                {
                    led_toggle2();
                }
                else if(strcmp(receivedData, "led3") == 0)
                {
                    led_toggle3();
                }
                else if(strcmp(receivedData, "all") == 0)
                {
                    led_toggle1();
                    led_toggle2();
                    led_toggle3();
                }
                /* 数据消费完毕，清空缓冲区准备下一次接收 */
                uart1_rx_clear();
            }
        }
    }
}