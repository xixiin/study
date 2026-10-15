#include "FreeRTOS_demo.h"
#include "delay.h"

void Delay_us(uint32_t us);  
//Task1特定类型的函数
void PIDIntTask( void * arg );
void speedTask( void * arg );
void locationTask( void * arg );

/* 互斥量句柄，保护共享变量 */
SemaphoreHandle_t xPIDMutex = NULL;
/* 蓝牙接收信号量：ISR 收到一帧数据后释放 */
SemaphoreHandle_t xBluetoothSemaphore = NULL;

void Key(void * arg);
#define KEY_TASK_STACK_SIZE 128
#define KEY_TASK_PRIORITY 2

TaskHandle_t keyTaskHandle = NULL;

void OLED(void * arg );
#define OLED_TASK_STACK_SIZE 256
#define OLED_TASK_PRIORITY 3

TaskHandle_t OLEDTaskHandle = NULL;

void MPU6050_Task(void * arg );
#define MPU6050_TASK_STACK_SIZE 256
#define MPU6050_TASK_PRIORITY 3

TaskHandle_t MPU6050TaskHandle = NULL;

#define speedTask_STACK_SIZE 128
//speedTask的优先级
#define speedTask_PRIORITY 3
//PIDIntTask的堆栈大小
#define PIDIntTask_STACK_SIZE 128
//PIDIntTask的优先级
#define PIDIntTask_PRIORITY 3
//locationTask的堆栈大小
#define locationTask_STACK_SIZE 128
//locationTask的优先级
#define locationTask_PRIORITY 4

void blueToosTask(void * arg );
#define blueToosTask_STACK_SIZE 128
#define blueToosTask_PRIORITY 3
TaskHandle_t blueToosTaskHandle = NULL;

TaskHandle_t PIDIntTaskHandle = NULL;
TaskHandle_t speedTaskHandle = NULL;
TaskHandle_t locationTaskHandle = NULL;

void FreeRTOS_Start(void)
{
    /* 创建互斥量，保护 PID / pwm / speed / location 等共享变量 */
    xPIDMutex = xSemaphoreCreateMutex();
    /* 创建二值信号量，用于蓝牙 ISR → 任务通知 */
    xBluetoothSemaphore = xSemaphoreCreateBinary();

    // Create tasks, queues, and other FreeRTOS objects here
    xTaskCreate(speedTask, "SPEED", speedTask_STACK_SIZE, NULL, speedTask_PRIORITY, &speedTaskHandle);
    xTaskCreate(locationTask, "LOCATION", locationTask_STACK_SIZE, NULL, locationTask_PRIORITY, &locationTaskHandle);
    xTaskCreate(PIDIntTask, "PID_INT", PIDIntTask_STACK_SIZE, NULL, PIDIntTask_PRIORITY, &PIDIntTaskHandle);
    xTaskCreate(Key, "KEY", KEY_TASK_STACK_SIZE, NULL, KEY_TASK_PRIORITY, &keyTaskHandle);
    xTaskCreate(OLED, "OLED", OLED_TASK_STACK_SIZE, NULL, OLED_TASK_PRIORITY, &OLEDTaskHandle);
    xTaskCreate(MPU6050_Task, "MPU6050", MPU6050_TASK_STACK_SIZE, NULL, MPU6050_TASK_PRIORITY, &MPU6050TaskHandle);
    xTaskCreate(blueToosTask, "BLUETOOS", blueToosTask_STACK_SIZE, NULL, blueToosTask_PRIORITY, &blueToosTaskHandle);
    // Start the scheduler
    vTaskStartScheduler();

    // If all is well, the scheduler will now be running, and the following line will never be reached.
    // If the following line does execute, then there was insufficient FreeRTOS heap memory available
    // for the idle and/or timer tasks to be created.
    for(;;);
}

uint8_t key_value=0;
int16_t pwm;
void Key(void * arg)
{
    while(1)
    {
        key_value=key_scand();
        if(key_value==1)
        {
            if (xSemaphoreTake(xPIDMutex, portMAX_DELAY) == pdTRUE)
            {
                pwm+=500;
            
                xSemaphoreGive(xPIDMutex);
            }
        }
        else if(key_value==2)
        {
            if (xSemaphoreTake(xPIDMutex, portMAX_DELAY) == pdTRUE)
            {
                pwm-=500;
          
                xSemaphoreGive(xPIDMutex);
            }
        }
        else if(key_value==3)
        {
            led_on();
        }
        else if(key_value==4)
        {
            led_off();
        }

        vTaskDelay(10);
    }

}



float speed, location;
PID_HandleTypeDef SpeedPID={0};  // PID 句柄
PID_HandleTypeDef LocationPID={0};

void PIDIntTask( void * arg )
{
    PID_Init(&SpeedPID, 0.38f, 0.26f, 0.0f, 0.3f, -99.0f, 99.0f, -99.0f, 99.0f);   // 初始化速度环 PID
    PID_Init(&LocationPID, 0.2f, 0.0f, 0.38f, 0.3f, -99.0f, 99.0f, -150.0f, 150.0f); // 初始化位置环 PID
    while(1)
    {
        if (xSemaphoreTake(xPIDMutex, portMAX_DELAY) == pdTRUE)
        {
            PID_SetTarget(&LocationPID, pwm);
            xSemaphoreGive(xPIDMutex);
        }
        vTaskDelay(10); 
    }
}
void speedTask( void * arg )
{
    while(1)
    {
        /* 读取编码器速度（增量），并累加得到位置 */
        if (xSemaphoreTake(xPIDMutex, portMAX_DELAY) == pdTRUE)
        {
            speed = Encoder_Get(1);
            location += speed;
            Motor_SetPWM(1, PID_Compute(&SpeedPID, speed));
            xSemaphoreGive(xPIDMutex);
        }
        vTaskDelay(40);
    }
}

void locationTask( void * arg )
{
    while(1)
    {
        /* 外环：位置环 —— 用 LocationPID 计算位置误差，
           输出作为速度环的目标值（级联结构） */
        if (xSemaphoreTake(xPIDMutex, portMAX_DELAY) == pdTRUE)
        {
            float speedTarget = PID_Compute(&LocationPID, location);
            PID_SetTarget(&SpeedPID, speedTarget);
            xSemaphoreGive(xPIDMutex);
        }
        vTaskDelay(60);
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

int16_t AccX, AccY, AccZ;
int16_t GyroX, GyroY, GyroZ;
void OLED(void * arg )
{
    while(1)
    {
        if (xSemaphoreTake(xPIDMutex, portMAX_DELAY) == pdTRUE)
        {
            // OLED_Printf(0,0,OLED_8X16,"Tar:%+4.2f",LocationPID.Target);
            // OLED_Printf(0,16,OLED_8X16,"Act:%+4.2f",LocationPID.Actual);
            // OLED_Printf(0,32,OLED_8X16,"Out:%+4.2f",LocationPID.OutFilt);
            // OLED_Printf(64,0,OLED_8X16,"Kp:%+4.2f",LocationPID.Kp);
            // OLED_Printf(64,16,OLED_8X16,"Ki:%+4.2f",LocationPID.Ki);
            // OLED_Printf(64,32,OLED_8X16,"Kd:%+4.2f",LocationPID.Kd);
            // OLED_Printf(0,48,OLED_8X16,"PWM:%+4d",pwm);
            OLED_Printf(0,0,OLED_8X16,"AX:%+05d",AccX);
            OLED_Printf(0,16,OLED_8X16,"AY:%+05d",AccY);
            OLED_Printf(0,32,OLED_8X16,"AZ:%+05d",AccZ);
            OLED_Printf(64,0,OLED_8X16,"GX:%+05d",GyroX);
            OLED_Printf(64,16,OLED_8X16,"GY:%+05d",GyroY);
            OLED_Printf(64,32,OLED_8X16,"GZ:%+05d",GyroZ);
            xSemaphoreGive(xPIDMutex);
        }
        OLED_Update();
        vTaskDelay(100);
    }

}

void MPU6050_Task(void * arg )
{
    while(1)
    {
        MPU6050_GetData(&AccX, &AccY, &AccZ, &GyroX, &GyroY, &GyroZ);

        vTaskDelay(40);
    }
}

void blueToosTask(void * arg )
{
    while(1)
    {
        /* 阻塞等待蓝牙 ISR 发来的信号量（收到一帧数据后唤醒） */
        if (xSemaphoreTake(xBluetoothSemaphore, portMAX_DELAY) == pdTRUE)
        {
            /* 复制接收缓冲区，避免 ISR 覆盖，复制后清零长度 */
            uint8_t rx_copy[UART2_RX_BUF_SIZE];
            taskENTER_CRITICAL();
            memcpy(rx_copy, uart2_rx_buf, uart2_rx_len);
            uart2_rx_len = 0;   /* 关键：复制完立刻清零，下一帧从头开始 */
            taskEXIT_CRITICAL();

            /* ---- 在这里处理蓝牙接收的数据 ---- */
            if (strstr((char *)rx_copy, "ON") != NULL || strstr((char *)rx_copy, "on") != NULL)
            {
                led_on();
                bt_send("on\r\n");
            }
            else if (strstr((char *)rx_copy, "OFF") != NULL || strstr((char *)rx_copy, "off") != NULL)
            {
                led_off();
                bt_send("off\r\n");
            }
            /* 可在此扩展更多指令处理 */
        }
    }
}