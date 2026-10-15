#include "sys.h"
#include "uart1.h"
#include "delay.h"
#include "key.h"
#include "OLED.h"
#include "led.h"
#include "timer.h"
#include "Motor.h"
#include "Encoder.h"
#include "PID.h"
#include "MPU6050.h"
#include "bluetoos.h"
#include "math.h"

int16_t Ax,Ay,Az,Gx,Gy,Gz;

int16_t pwm,PWMR,PWML;
int16_t speed1,speed2;
uint8_t key_num;
uint16_t adc_value[4];

float AngleAcc;
float AngleGyro;
float Angle;

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
	bt_init(115200);
	Key_Init();
    printf("running...\r\n");
	printf("你好\r\n");
	MPU6050_Init();
	OLED_Init();
	OLED_Clear();
	Motor_Init();       /* 先初始化电机PWM，否则ISR里 Motor_SetPWM 写空指针 */
	Encoder_Init();     /* 先初始化编码器，否则ISR里 Encoder_Get 读未初始化定时器 */
	led_init();

	/* ---- 初始化 PID 控制器 ---- */
	/* 内环速度环：Kp=0.38, Ki=0.28, Kd=0, 滤波系数=0.3, 输出限幅±99 */
	PID_Init(&gSpeedPID, 0.36f, 0.26f, 0.0f, 0.3f, -99.0f, 99.0f, -99.0f, 99.0f);
	/* 外环位置环：Kp=0.2, Ki=0, Kd=0.4, 滤波系数=0.3, 输出限幅±99 */
	PID_Init(&gPosPID,   0.25f,  0.0f,  0.36f, 0.3f, -99.0f, 99.0f, -150.0f, 150.0f);

	timer_init(100-1,720-1);  /* 最后启动定时器！确保所有外设已就绪 */

    while(1)
    { 
		// key_num = Key_GetNum();
		// if (key_num == 1)
		// {
		// 	PWMR += 10;   /* 右电机PWM +10 */
		// }
		// else if (key_num == 2)
		// {
		// 	PWMR -= 10;   /* 右电机PWM -10 */
		// }
		// else if (key_num == 3)
		// {
		// 	PWML += 10;   /* 左电机PWM +10 */
		// }
		// else if (key_num == 4)
		// {
		// 	PWML -= 10;   /* 左电机PWM -10 */
		// }
		// Motor_SetPWM(1,PWMR);
		// Motor_SetPWM(2,PWML);
		// PID_SetTarget(&gPosPID,200);
		
		// delay_ms(10);
		// OLED_Printf(0,0,OLED_8X16, "%+06d", Ax);
		// OLED_Printf(0,16,OLED_8X16, "%+06d", Ay);
		// OLED_Printf(0,32,OLED_8X16, "%+06d", Az);
		// OLED_Printf(64,0,OLED_8X16, "%+06d", Gx);
		// OLED_Printf(64,16,OLED_8X16, "%+06d", Gy);
		// OLED_Printf(64,32,OLED_8X16, "%+06d", Gz);
		OLED_Printf(0, 0, OLED_8X16, "%+06d", Ax);		//显示AX
		OLED_Printf(0, 16, OLED_8X16, "%+06d", Ay);		//显示AY
		OLED_Printf(0, 32, OLED_8X16, "%+06d", Az);		//显示AZ
		OLED_Printf(64, 0, OLED_8X16, "%+06d", Gx);		//显示GX
		OLED_Printf(64, 16, OLED_8X16, "%+06d", Gy);	//显示GY
		OLED_Printf(64, 32, OLED_8X16, "%+06d", Gz);	//显示GZ
		// OLED_Printf(0,0,OLED_8X16,"%+05d",speed1);
		// OLED_Printf(64,0,OLED_8X16,"%+05d",speed2);
		OLED_Printf(0,48,OLED_8X16,"%+06.2f",AngleAcc);
		OLED_Printf(64,48,OLED_8X16,"%+06.2f",Angle);
		/* OLED 显示外环信息 */
//		OLED_Printf(0,  0, OLED_8X16, "Tar:%+4.2f", gPosPID.Target);
//		OLED_Printf(0, 16, OLED_8X16, "Act:%+4.2f", gPosPID.Actual);
//		OLED_Printf(0, 32, OLED_8X16, "Out:%+4.2f", gPosPID.Out);
//		OLED_Printf(64, 0, OLED_8X16, "Kp:%+4.2f", gPosPID.Kp);
//		OLED_Printf(64,16, OLED_8X16, "Ki:%+4.2f", gPosPID.Ki);
//		OLED_Printf(64,32, OLED_8X16, "Kd:%+4.2f", gPosPID.Kd);
		
		OLED_Update();

		/* 串口输出：目标, 实际, 输出 */
		// printf("%f,%f,%f\r\n", gPosPID.Target, gPosPID.Actual, gPosPID.Out);
		// bt_send("speed1:%d,speed2:%d\r\n", speed1, speed2);
		delay_ms(10);
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint16_t count1 = 0, count2 = 0;

	if (htim->Instance == TIM1)
	{
		MPU6050_GetData(&Ax,&Ay,&Az,&Gx,&Gy,&Gz);
		Gy-=24;
		AngleGyro = Angle + Gy * 0.001f /32768.0*2000;   /* 积分计算角速度，单位：度 */
		
		AngleAcc = atan2(Ax,Az)*180/3.1415926;   /* 计算俯仰角，单位：度 */
		float Alpha = 0.98;   /* 互补滤波系数 */
		Angle = Alpha * Angle + (1.0 - Alpha) * AngleAcc;   /* 互补滤波 */
		count1++;
		if (count1 >= 50)               /* 内环速度环：25Hz */
		{
			count1 = 0;
			// Speed = Encoder_Get(1);                     /* 获取编码器增量 = 速度 */
			// Location += Speed;                            /* 累加得到位置 */

			/* 内环 PID 计算：目标=外环输出，反馈=当前速度 */
			// Motor_SetPWM(1,(int16_t)PID_Compute(&gSpeedPID, Speed));
			speed1 = Encoder_Get(1);
			speed2 = Encoder_Get(2);
		}

		count2++;
		if (count2 >= 40)               /* 外环位置环：25Hz */
		{
			count2 = 0;

			/* 外环 PID 计算：目标=用户设定，反馈=当前位置 */
			/* 外环输出作为内环速度目标 */
			// PID_SetTarget(&gSpeedPID,
			// 	PID_Compute(&gPosPID, Location));
		}

		Key_Tick();
	}
}
