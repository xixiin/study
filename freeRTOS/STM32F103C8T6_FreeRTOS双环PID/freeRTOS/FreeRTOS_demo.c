#include "FreeRTOS_demo.h"
#include "delay.h"

void Delay_us(uint32_t us);  
//Task1特定类型的函数
void PIDIntTask( void * arg );
void speedTask( void * arg );
void locationTask( void * arg );

void Key(void * arg);
#define KEY_TASK_STACK_SIZE 128
#define KEY_TASK_PRIORITY 3

TaskHandle_t keyTaskHandle = NULL;

void OLED(void * arg );
#define OLED_TASK_STACK_SIZE 1024
#define OLED_TASK_PRIORITY 3

TaskHandle_t OLEDTaskHandle = NULL;


#define speedTask_STACK_SIZE 256
//speedTask的优先级
#define speedTask_PRIORITY 5
//PIDIntTask的堆栈大小
#define PIDIntTask_STACK_SIZE 128
//PIDIntTask的优先级
#define PIDIntTask_PRIORITY 3
//locationTask的堆栈大小
#define locationTask_STACK_SIZE 256
//locationTask的优先级
#define locationTask_PRIORITY 4

TaskHandle_t PIDIntTaskHandle = NULL;
TaskHandle_t speedTaskHandle = NULL;
TaskHandle_t locationTaskHandle = NULL;

void FreeRTOS_Start(void)
{

    // Create tasks, queues, and other FreeRTOS objects here
    xTaskCreate(speedTask, "SPEED", speedTask_STACK_SIZE, NULL, speedTask_PRIORITY, &speedTaskHandle);
    xTaskCreate(locationTask, "LOCATION", locationTask_STACK_SIZE, NULL, locationTask_PRIORITY, &locationTaskHandle);
    xTaskCreate(PIDIntTask, "PID_INT", PIDIntTask_STACK_SIZE, NULL, PIDIntTask_PRIORITY, &PIDIntTaskHandle);
    // xTaskCreate(Key, "KEY", SEND_TASK_STACK_SIZE, NULL, SEND_TASK_PRIORITY, &keyTaskHandle);
    xTaskCreate(OLED, "OLED", OLED_TASK_STACK_SIZE, NULL, OLED_TASK_PRIORITY, &OLEDTaskHandle);
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
int16_t pwm=30;
void Key(void * arg)
{
    while(1)
    {
        key_value = key_scand();
        if(key_value==1)
        {
            pwm += 10;
          led_on();
   
            
        }
        else if(key_value==2)
        {
           pwm -= 10;
            led_off();
       
           
        }
        else if(key_value==3)
        {
           
          
        }
        else if(key_value==4)
        {

          
        }
        vTaskDelay(40);
    }

}


float Kp,Ki,Kd;
float Target,Actual,Out;
float err0,err1,errint;
float ActualPrev;                // 上一次实际速度（用于微分先行）
float OutFilt;                   // 输出低通滤波（抑制 12V 下的高频摆动）
float speed, location;
PID_HandleTypeDef SpeedPID={0};  // PID 句柄
PID_HandleTypeDef LocationPID={0};

void PIDIntTask( void * arg )
{
    PID_Init(&SpeedPID, 0.38f, 0.26f, 0.0f, 0.3f, -99.0f, 99.0f, -99.0f, 99.0f);   // 初始化速度环 PID
    PID_Init(&LocationPID, 0.2f, 0.0f, 0.38f, 0.3f, -99.0f, 99.0f, -150.0f, 150.0f); // 初始化位置环 PID
    while(1)
    {
        /* 从 ADC 读取电位器值，映射为位置目标（例如 -500 ~ +500 表示位置） */
        PID_SetTarget(&LocationPID,
            (float)ADC_GetValue(4) / 4095.0f * 1000.0f - 500.0f);
        vTaskDelay(10); 
    }
}
void speedTask( void * arg )
{
    while(1)
    {
        /* 读取编码器速度（增量），并累加得到位置 */
        speed = Encoder_Get(1);
        location += speed;

        /* 内环：速度环 —— 用 SpeedPID 计算，输出 PWM 驱动电机 */
        Motor_SetPWM((int16_t)PID_Compute(&SpeedPID, speed));
        vTaskDelay(40);
    }
}

void locationTask( void * arg )
{
    while(1)
    {
        /* 外环：位置环 —— 用 LocationPID 计算位置误差，
           输出作为速度环的目标值（级联结构） */
        float speedTarget = PID_Compute(&LocationPID, location);
        PID_SetTarget(&SpeedPID, speedTarget);
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


void OLED(void * arg )
{
    while(1)
    {
    
		OLED_Printf(0,0,OLED_8X16,"Tar:%+4.2f",LocationPID.Target);
		OLED_Printf(0,16,OLED_8X16,"Act:%+4.2f",LocationPID.Actual);
		OLED_Printf(0,32,OLED_8X16,"Out:%+4.2f",LocationPID.OutFilt);
		OLED_Printf(64,0,OLED_8X16,"Kp:%+4.2f",LocationPID.Kp);
		OLED_Printf(64,16,OLED_8X16,"Ki:%+4.2f",LocationPID.Ki);
		OLED_Printf(64,32,OLED_8X16,"Kd:%+4.2f",LocationPID.Kd);
        OLED_Update();
        vTaskDelay(100);
    }

}
