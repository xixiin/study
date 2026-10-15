#include "FreeRTOS_demo.h"
#include "delay.h"

void Delay_us(uint32_t us);  
//Task1特定类型的函数
void sendTask( void * arg );
void receiveTask( void * arg );

void Key(void * arg);
#define SEND_TASK_STACK_SIZE 128
#define SEND_TASK_PRIORITY 3

TaskHandle_t keyTaskHandle = NULL;

void OLED(void * arg );
#define OLED_TASK_STACK_SIZE 1024
#define OLED_TASK_PRIORITY 3

TaskHandle_t OLEDTaskHandle = NULL;


//receiveTask的堆栈大小
#define receiveTask_STACK_SIZE 256
//receiveTask的优先级
#define receiveTask_PRIORITY 4
//sendTask的堆栈大小
#define sendTask_STACK_SIZE 128
//sendTask的优先级
#define sendTask_PRIORITY 3

TaskHandle_t sendTaskHandle = NULL;
TaskHandle_t receiveTaskHandle = NULL;

void FreeRTOS_Start(void)
{

    // Create tasks, queues, and other FreeRTOS objects here
    xTaskCreate(sendTask, "SEND", sendTask_STACK_SIZE, NULL, sendTask_PRIORITY, &sendTaskHandle);
    xTaskCreate(receiveTask, "RECEIVE", receiveTask_STACK_SIZE, NULL, receiveTask_PRIORITY, &receiveTaskHandle);
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
        //   xQueueSend(xQueue, &pwm, portMAX_DELAY); // Send the pwm value to the queue
            
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

void sendTask( void * arg )
{
    while(1)
    {
        Kp = (float)ADC_GetValue(1)/4095.0*2.0;  
		Ki = (float)ADC_GetValue(2)/4095.0*2.0;   
		Kd = (float)ADC_GetValue(3)/4095.0*2.0;		
		Target = (float)ADC_GetValue(4)/4095.0*400.0-200.0;  // 0~200
        vTaskDelay(10); 
    }

}
void receiveTask( void * arg )
{
    while(1)
    {
			Actual = Encoder_Get(1);                     // 获取编码器增量（当前速度）

			/* ====== PID 计算 ====== */
			err1 = err0;                                 // 保存上一次误差
			err0 = Target - Actual;                      // 计算当前误差

			/* 积分累加 + 抗饱和钳位 */
			errint += err0;
			if (errint >  99.0f) errint =  99.0f;        // 积分上限钳位
			if (errint < -99.0f) errint = -99.0f;        // 积分下限钳位
			if (Ki == 0) errint = 0;                     // Ki=0 时清零积分

			/* 微分先行（Derivative on Measurement）：避免微分 kick */
			Out = Kp * err0 + Ki * errint + Kd * (ActualPrev - Actual);
			ActualPrev = Actual;                         // 保存本次实际值

			/* 输出一阶低通滤波：抑制 12V 下的高频振荡 */
			OutFilt = OutFilt * 0.7f + Out * 0.3f;       // 滤波系数 0.3，越大响应越快但越易振荡

			/* 输出限幅：匹配 PWM 有效范围 0~99 */
			if (OutFilt >  99) OutFilt =  99;
			if (OutFilt < -99) OutFilt = -99;

			Motor_SetPWM((int16_t)OutFilt);              // 设置电机 PWM
        vTaskDelay(40);
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
    
		OLED_Printf(0,0,OLED_8X16,"Tar:%+4.2f",Target);
		OLED_Printf(0,16,OLED_8X16,"Act:%+4.2f",Actual);
		OLED_Printf(0,32,OLED_8X16,"Out:%+4.2f",Out);
		OLED_Printf(64,0,OLED_8X16,"Kp:%+4.2f",Kp);
		OLED_Printf(64,16,OLED_8X16,"Ki:%+4.2f",Ki);
		OLED_Printf(64,32,OLED_8X16,"Kd:%+4.2f",Kd);
        OLED_Update();
        vTaskDelay(100);
    }

}