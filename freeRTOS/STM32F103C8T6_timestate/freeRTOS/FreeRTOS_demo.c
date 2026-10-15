#include "FreeRTOS_demo.h"
#include "delay.h"

void Delay_us(uint32_t us);  
//Task1特定类型的函数
void Task1( void * arg );
void Task2( void * arg );
void Task3( void * arg );

//Task3的堆栈大小
#define Task3_STACK_SIZE 128
//Task3的优先级
#define Task3_PRIORITY 3
//Task2的堆栈大小
#define Task2_STACK_SIZE 128
//Task2的优先级
#define Task2_PRIORITY 3
//Task1的堆栈大小
#define Task1_STACK_SIZE 128
//Task1的优先级
#define Task1_PRIORITY 3

TaskHandle_t Task1Handle = NULL;
TaskHandle_t Task2Handle = NULL;
TaskHandle_t Task3Handle = NULL;
void FreeRTOS_Start(void)
{
    // Create tasks, queues, and other FreeRTOS objects here
    xTaskCreate(Task1, "TASK1", Task1_STACK_SIZE, NULL, Task1_PRIORITY, &Task1Handle);
    xTaskCreate(Task2, "TASK2", Task2_STACK_SIZE, NULL, Task2_PRIORITY, &Task2Handle);
    xTaskCreate(Task3, "TASK3", Task3_STACK_SIZE, NULL, Task3_PRIORITY, &Task3Handle);
    // Start the scheduler
    vTaskStartScheduler();

    // If all is well, the scheduler will now be running, and the following line will never be reached.
    // If the following line does execute, then there was insufficient FreeRTOS heap memory available
    // for the idle and/or timer tasks to be created.
    for(;;);
}

void Task1( void * arg )
{
    while(1)
    {
        led_toggle1();
        //FreeRTOS提供的延时函数

        // Delay_us(200000); 
        vTaskDelay(500); // 延时500毫秒

    }

}
void Task2( void * arg )
{
    while(1)
    {
        led_toggle2();
        vTaskDelay(1000);
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

uint8_t keynum = 0;
//调度器的挂起和恢复
// void Task3( void * arg )
// {
//     while(1)
//     {
//         keynum = key_scand();
//         if(keynum == 1)
//         {
//             printf("Key %d is pressed\r\n", keynum);
//            taskENTER_CRITICAL();
//            Delay_us(5000000); // 延时5秒
//             taskEXIT_CRITICAL();

//         }
//         else if(keynum == 2)
//         {
//             //挂起调度器
//             vTaskSuspendAll();
//             printf("挂起调度器\r\n");
//             Delay_us(5000000);
//             //恢复调度器
//             printf("恢复调度器\r\n");
//             xTaskResumeAll();
        
//         }


//     }

// }
//获取任务状态,获取最小剩余堆栈空间
// void Task3( void * arg )
// {
//     while(1)
//     {
//         keynum = key_scand();
//         if(keynum == 1)
//         {
//             //获取task1的状态
//             eTaskState state = eTaskGetState(Task1Handle);
//             taskENTER_CRITICAL();
//             if(state == eRunning)
//             {
//                 printf("Task1 is running\r\n");
//             }
//             else if(state == eReady)
//             {
//                 printf("Task1 is ready\r\n");
//             }
//             else if(state == eBlocked)
//             {
//                 printf("Task1 is blocked\r\n");
//             }
//             else if(state == eSuspended)
//             {
//                 printf("Task1 is suspended\r\n");
//             }
//             else if(state == eDeleted)
//             {
//                 printf("Task1 is deleted\r\n");
//             }
//             else
//             {
//                 printf("Task1 state is invalid\r\n");
//             }
//             taskEXIT_CRITICAL();
//             //获取task1的最小剩余堆栈空间
//             UBaseType_t minStackSpace = uxTaskGetStackHighWaterMark(Task1Handle);
//             printf("Task1 min stack space: %d\r\n", minStackSpace);
//         }
//         else if(keynum == 2)
//         {
//             eTaskState state = eTaskGetState(Task2Handle);
//             taskENTER_CRITICAL();
//             if(state == eRunning)
//             {
//                 printf("Task2 is running\r\n");
//             }
//             else if(state == eReady)
//             {
//                 printf("Task2 is ready\r\n");
//             }
//             else if(state == eBlocked)
//             {
//                 printf("Task2 is blocked\r\n");
//             }
//             else if(state == eSuspended)
//             {
//                 printf("Task2 is suspended\r\n");
//             }
//             else if(state == eDeleted)
//             {
//                 printf("Task2 is deleted\r\n");
//             }
//             else
//             {
//                 printf("Task2 state is invalid\r\n");
//             }
//             taskEXIT_CRITICAL();
//             UBaseType_t minStackSpace = uxTaskGetStackHighWaterMark(Task2Handle);
//             printf("Task2 min stack space: %d\r\n", minStackSpace);
        
//         }


//     }

// }

// uint8_t runtime_stats_buffer[256]; // 用于存储运行时间统计信息的缓冲区
// void Task3( void * arg ){
//     while(1)
//     {
//             // 获取任务运行时间统计信息
//         //    vTaskGetRunTimeStats((char *)runtime_stats_buffer);
//         //    printf("Task Runtime Stats:\r\n%s\r\n", runtime_stats_buffer);
//         //获取任务表格
//         vTaskList((char *)runtime_stats_buffer);
//         printf("Task List:\r\n%s\r\n", runtime_stats_buffer);
//            vTaskDelay(1000); // 延时1秒

//     }

// }
TaskStatus_t pxTaskStatusArray[10]; // 用于存储任务状态信息的数组
uint32_t totalRunTime = 0; // 用于存储总运行时间
void Task3( void * arg ){
    while(1)
    {
        //获取单个任务信息
        //获取task1INfo
        // TaskStatus_t task1Info;
        // vTaskGetInfo(Task1Handle, &task1Info, pdTRUE, eInvalid);
        // // 句柄 接收信息的结构体 是否获取剩余堆栈空间 需要获取任务状态
        // printf("Task1 Name: %s, State: %d, Priority: %d, Stack High Water Mark: %d\r\n", 
        //     task1Info.pcTaskName, task1Info.eCurrentState, task1Info.uxCurrentPriority, task1Info.usStackHighWaterMark);   
        //获取多个任务信息
        uxTaskGetSystemState(pxTaskStatusArray, uxTaskGetNumberOfTasks(), &totalRunTime); // 获取系统中最多10个任务的状态信息
        for(UBaseType_t i = 0; i < uxTaskGetNumberOfTasks(); i++)
        {
            printf("Task Name: %s, State: %d, Priority: %d, Stack High Water Mark: %d\r\n", 
                pxTaskStatusArray[i].pcTaskName, pxTaskStatusArray[i].eCurrentState, 
                pxTaskStatusArray[i].uxCurrentPriority, pxTaskStatusArray[i].usStackHighWaterMark);
        }
        printf("Total Run Time: %d\r\n", totalRunTime);
        vTaskDelay(1000); // 延时1秒
    }

}