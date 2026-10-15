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
#include "PID.h"

int16_t pwm;
uint8_t key_num;
uint16_t adc_value[4];

float Speed, Location;

/* 内环：速度环 PID */
PID_HandleTypeDef gSpeedPID;
/* 外环：位置环 PID */
PID_HandleTypeDef gPosPID;

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

	/* ---- 初始化 PID 控制器 ---- */
	/* 内环速度环：Kp=0.38, Ki=0.28, Kd=0, 滤波系数=0.3, 输出限幅±99 */
	PID_Init(&gSpeedPID, 0.38f, 0.26f, 0.0f, 0.3f, -99.0f, 99.0f, -99.0f, 99.0f);
	/* 外环位置环：Kp=0.2, Ki=0, Kd=0.4, 滤波系数=0.3, 输出限幅±99 */
	PID_Init(&gPosPID,   0.2f,  0.0f,  0.38f, 0.3f, -99.0f, 99.0f, -150.0f, 150.0f);

	timer_init(100-1,720-1);  /* 最后启动定时器！确保所有外设已就绪 */

    while(1)
    { 
		/* 从 ADC 读取外环目标位置（可在线拧电位器调节） */
		PID_SetTarget(&gPosPID,
			(float)ADC_GetValue(4) / 4095.0f * 1000.0f - 500.0f);

		delay_ms(10);

		/* OLED 显示外环信息 */
		OLED_Printf(0,  0, OLED_8X16, "Tar:%+4.2f", gPosPID.Target);
		OLED_Printf(0, 16, OLED_8X16, "Act:%+4.2f", gPosPID.Actual);
		OLED_Printf(0, 32, OLED_8X16, "Out:%+4.2f", gPosPID.Out);
		OLED_Printf(64, 0, OLED_8X16, "Kp:%+4.2f", gPosPID.Kp);
		OLED_Printf(64,16, OLED_8X16, "Ki:%+4.2f", gPosPID.Ki);
		OLED_Printf(64,32, OLED_8X16, "Kd:%+4.2f", gPosPID.Kd);
		OLED_Update();

		/* 串口输出：目标, 实际, 输出 */
		printf("%f,%f,%f\r\n", gPosPID.Target, gPosPID.Actual, gPosPID.Out);
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint16_t count1 = 0, count2 = 0;

	if (htim->Instance == TIM1)
	{
		count1++;
		if (count1 >= 40)               /* 内环速度环：25Hz */
		{
			count1 = 0;
			Speed = Encoder_Get(1);                     /* 获取编码器增量 = 速度 */
			Location += Speed;                            /* 累加得到位置 */

			/* 内环 PID 计算：目标=外环输出，反馈=当前速度 */
			Motor_SetPWM((int16_t)PID_Compute(&gSpeedPID, Speed));
		}

		count2++;
		if (count2 >= 40)               /* 外环位置环：25Hz */
		{
			count2 = 0;

			/* 外环 PID 计算：目标=用户设定，反馈=当前位置 */
			/* 外环输出作为内环速度目标 */
			PID_SetTarget(&gSpeedPID,
				PID_Compute(&gPosPID, Location));
		}

		Key_Tick();
	}
}
