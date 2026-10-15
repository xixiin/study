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
uint8_t motor_flg=1;
float Kp,Ki,Kd;
float Target,Actual,Out;
float err0,err1,errint;


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


		key_num=Key_GetNum();
		if(key_num==1)
		{
			motor_flg=!motor_flg;  // 按键1切换电机启停

		}
		if(motor_flg==1)
		{
			led_on();
		}
		else
		{
			led_off();
		}
		Kp = (float)ADC_GetValue(1)/4095.0*4.0;  // 0~4
		Ki = (float)ADC_GetValue(2)/4095.0*4.0;   // 0~4
		Kd = (float)ADC_GetValue(3)/4095.0*4.0;		
		Target = (float)ADC_GetValue(4)/4095.0*400.0-200.0;  // 0~200
		delay_ms(10);
		OLED_Printf(0,0,OLED_8X16,"Tar:%+4.2f",Target);
		OLED_Printf(0,16,OLED_8X16,"Act:%+4.2f",Actual);
		OLED_Printf(0,32,OLED_8X16,"Out:%+4.2f",Out);
		OLED_Printf(64,0,OLED_8X16,"Kp:%+4.2f",Kp);
		OLED_Printf(64,16,OLED_8X16,"Ki:%+4.2f",Ki);
		OLED_Printf(64,32,OLED_8X16,"Kd:%+4.2f",Kd);
		OLED_Update();

		printf("%f,%f,%f,%f\r\n",Target,Actual,Out,errint);
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
			Actual = Encoder_Get(1);             // 直接获取编码器增量      // 当前速度
			err1 = err0;                                 // 保存上一次误差
			err0 = Target - Actual;                      // 计算当前误差
			if(Ki!=0)
			{
				errint+=err0;                              // 累加误差
			}
			else
			{
				errint=0;                                  // Ki=0时，清零累加误差
			}
			if(errint>100) errint=100;                       // 限幅
			if(errint<-100) errint=-100;
			Out = Kp * err0 + Ki * errint + Kd * (err0 - err1);  // PID控制输出
			if(Out>100) Out=100;                         // 限幅
			if(Out<-100) Out=-100;
			if(motor_flg) Motor_SetPWM(Out);                           // 设置电机速度
			else Motor_SetPWM(0);                           // 电机停止

		}
		Key_Tick();
	
	}
}
