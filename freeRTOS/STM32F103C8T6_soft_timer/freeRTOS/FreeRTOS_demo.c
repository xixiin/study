#include "FreeRTOS_demo.h"
#include "delay.h"

void Delay_us(uint32_t us);  
//Task1特定类型的函数
void keyTask( void * arg );
void timeTask( void * arg );



//timeTask的堆栈大小
#define timeTask_STACK_SIZE 128
//timeTask的优先级
#define timeTask_PRIORITY 3

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

TaskHandle_t timeTaskHandle = NULL;

void timer1_callback( TimerHandle_t xTimer );
void timer2_callback( TimerHandle_t xTimer );
#define timer1_callback_PERIOD 500
#define timer2_callback_PERIOD 1000

TimerHandle_t timer1_handle = NULL;
TimerHandle_t timer2_handle = NULL;
void FreeRTOS_Start(void)
{


    //使用软件定时器
    timer1_handle = xTimerCreate("timer1", timer1_callback_PERIOD, pdTRUE, NULL, timer1_callback);
    timer2_handle = xTimerCreate("timer2", timer2_callback_PERIOD, pdTRUE, NULL, timer2_callback);
    xTaskCreate(keyTask, "KEY", keyTask_STACK_SIZE, NULL, keyTask_PRIORITY, &keyTaskHandle);
    // xTaskCreate(timeTask, "TIME", timeTask_STACK_SIZE, NULL, timeTask_PRIORITY, &timeTaskHandle);
    //启动软件定时器
    xTimerStart(timer1_handle, 0);
    xTimerStart(timer2_handle, 0);
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
    uint16_t timer_count = timer1_callback_PERIOD;
    uint8_t timer_flag = 0;
    while(1)
    {
        key_value = key_scand();
        if(key_value == key1)
        {
            timer_flag = !timer_flag;
            if(timer_flag)
            {
                xTimerStart(timer1_handle, 0); // 启动定时器1
            }
            else
            {
                xTimerStop(timer1_handle, 0); // 停止定时器1
            }
        }
        else if(key_value == key2)
        {
           timer_count+=50;
           if(timer_count > 1000)
           {
               timer_count = 1000;
           }
           xTimerChangePeriod(timer1_handle, timer_count, 0); // 改变定时器1的周期为1000ms
        }
        else if(key_value == key3)
        {
           timer_count-=50;
           if(timer_count < 20)
           {
               timer_count = 20;
           }
           xTimerChangePeriod(timer1_handle, timer_count, 0); // 改变定时器1的周期为1000ms

        }

        vTaskDelay(10); 
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

void timer1_callback( TimerHandle_t xTimer )
{
    if(timer1_handle == xTimer)
    {
        led_toggle1();
    }
    

}
void timer2_callback( TimerHandle_t xTimer )
{
    if(timer2_handle == xTimer)
    {
        led_toggle2();
    }
    

}