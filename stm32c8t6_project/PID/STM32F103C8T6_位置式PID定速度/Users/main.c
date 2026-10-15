#include "sys.h"
#include "uart1.h"
#include "delay.h"
#include "key.h"
#include "OLED.h"
#include "led.h"
#include "timer.h"
#include "RT.h"
#include "Motor.h"
#include "Encoder.h"

int16_t pwm;
uint8_t key_num;
uint16_t adc_value[4];

float Kp,Ki,Kd;
float Target,Actual,Out;
float err0,err1,errint;
float ActualPrev;                // 上一次实际速度（用于微分先行）
float OutFilt;                   // 输出低通滤波（抑制 12V 下的高频摆动）


int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */                       
    uart1_init(115200);
	Key_Init();
    printf("running...\r\n");
	printf("你好\r\n");
	OLED_Init();
	OLED_Clear();
	RT_Init();
	Motor_Init();       /* 先初始化电机PWM，否则ISR里 Motor_SetPWM 写空指针 */
	Encoder_Init();     /* 先初始化编码器，否则ISR里 Encoder_Get 读未初始化定时器 */
	led_init();
	timer_init(100-1,720-1);  /* 最后启动定时器！确保所有外设已就绪 */

    while(1)
    { 


		// key_num=Key_GetNum();
		// if(key_num==1)
		// {
		// 	Target+=20;

		// }
		// else if(key_num==2)
		// {
		// 	Target-=20;
		// }
		// else if(key_num==3)
		// {
		// 	Target=0;
		// }
		// else if(key_num==4)
		// {
		// 	Target=location
		// }
		Kp = (float)ADC_GetValue(1)/4095.0*2.0;  
		Ki = (float)ADC_GetValue(2)/4095.0*2.0;   
		Kd = (float)ADC_GetValue(3)/4095.0*2.0;		
		Target = (float)ADC_GetValue(4)/4095.0*400.0-200.0;  // 0~200
		delay_ms(10);
		OLED_Printf(0,0,OLED_8X16,"Tar:%+4.2f",Target);
		OLED_Printf(0,16,OLED_8X16,"Act:%+4.2f",Actual);
		OLED_Printf(0,32,OLED_8X16,"Out:%+4.2f",Out);
		OLED_Printf(64,0,OLED_8X16,"Kp:%+4.2f",Kp);
		OLED_Printf(64,16,OLED_8X16,"Ki:%+4.2f",Ki);
		OLED_Printf(64,32,OLED_8X16,"Kd:%+4.2f",Kd);
		OLED_Update();

		printf("%f,%f,%f\r\n",Target,Actual,Out);
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint16_t count=0;

	if(htim->Instance == TIM1)
	{
		
		count++;
		if(count>=40)
		{
			count=0;
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

		}
		Key_Tick();
	
	}
}
