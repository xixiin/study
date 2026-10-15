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
#include "math.h"
#include "BlueSerial.h"
#include "string.h"
#include "stdlib.h"

int16_t Ax,Ay,Az,Gx,Gy,Gz;

int16_t AvePWM,DifPWM;
int16_t LeftPWM,RightPWM;
int16_t speed1,speed2;
uint8_t key_num,RUNflag;
uint16_t adc_value[4];

float AngleAcc;
float AngleGyro;
float Angle;
float AngleZero;    /* 机械平衡点的角度偏移，开机自动校准 */

float Speed, Location;


/* 内环：角度环 PID */
PID_t anglePID;
float TargetRaw;       /* 用户设定的原始目标值 */
float TargetSmooth;    /* 平滑后的目标值，送入PID避免微分冲击 */


int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */                       
    uart1_init(115200);
	BlueSerial_Init();
	Key_Init();
    printf("running...\r\n");
	printf("你好\r\n");
	MPU6050_Init();
	
	/* ---- 初始化角度基准：静置时采样多帧取平均，自动校准平衡零点 ---- */
	/* 小车必须静置在平衡位置，开机后自动记录零点偏移 */
	{
		float sum = 0;
		for (int i = 0; i < 50; i++)
		{
			MPU6050_GetData(&Ax, &Ay, &Az, &Gx, &Gy, &Gz);
			sum += atan2(Ax, Az) * 180.0f / 3.1415926f;
			delay_ms(2);
		}
		AngleZero = sum / 50.0f;   /* 记录当前角度作为机械平衡零点 */
		AngleGyro = 0;              /* 陀螺仪积分从0开始 */
		Angle = 0;                  /* 最终角度归零 */
	}
	
	OLED_Init();
	OLED_Clear();
	Motor_Init();       /* 先初始化电机PWM，否则ISR里 Motor_SetPWM 写空指针 */
	Encoder_Init();     /* 先初始化编码器，否则ISR里 Encoder_Get 读未初始化定时器 */
	led_init();

	/* ---- 初始化 PID 控制器 ---- */
	/* 内环角度环：从 P 开始调，12V电机力矩大，Kp 从小起步 */
	/* 调参顺序：先调 Kp 让小车有"抵抗感" → 再加 Kd 抑制振荡 → 最后加 Ki 消除静差 */
	PID_Init(&anglePID);
	anglePID.Kp = 1.0f;    /* 降低比例项，减少超调引起的震荡 */
	anglePID.Ki = 0.0f;   /* 降低积分项，防止低频摆动 */
	anglePID.Kd = 0.5f;    /* 增大微分项，增强阻尼抑制震荡 */
	anglePID.OutMax = 50.0f;
	anglePID.OutMin = -50.0f;


	timer_init(100-1,720-1);  /* 最后启动定时器！确保所有外设已就绪 */

    while(1)
    { 
		if (RUNflag){led_on();}else{led_off();}
		key_num = Key_GetNum();
		if (key_num == 1)
		{
			RUNflag = !RUNflag;   /* 按键1：切换运行/停止 */
		}
		// else if (key_num == 2)
		// {
		
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
		

		// OLED_Printf(0, 0, OLED_8X16, "%+06d", Ax);		//显示AX
		// OLED_Printf(0, 16, OLED_8X16, "%+06d", Ay);		//显示AY
		// OLED_Printf(0, 32, OLED_8X16, "%+06d", Az);		//显示AZ
		// OLED_Printf(64, 0, OLED_8X16, "%+06d", Gx);		//显示GX
		// OLED_Printf(64, 16, OLED_8X16, "%+06d", Gy);	//显示GY
		// OLED_Printf(64, 32, OLED_8X16, "%+06d", Gz);	//显示GZ
		// OLED_Printf(0,0,OLED_8X16,"%+05d",speed1);
		// OLED_Printf(64,0,OLED_8X16,"%+05d",speed2);

		/* OLED 显示外环信息 */
//		OLED_Printf(0,  0, OLED_8X16, "Tar:%+4.2f", gPosPID.Target);
//		OLED_Printf(0, 16, OLED_8X16, "Act:%+4.2f", gPosPID.Actual);
//		OLED_Printf(0, 32, OLED_8X16, "Out:%+4.2f", gPosPID.Out);
//		OLED_Printf(64, 0, OLED_8X16, "Kp:%+4.2f", gPosPID.Kp);
//		OLED_Printf(64,16, OLED_8X16, "Ki:%+4.2f", gPosPID.Ki);
//		OLED_Printf(64,32, OLED_8X16, "Kd:%+4.2f", gPosPID.Kd);
		OLED_Clear();
		OLED_Printf(0,  0, OLED_6X8, "Angle" );
		OLED_Printf(0,  8, OLED_6X8, "P:%05.2f", anglePID.Kp);
		OLED_Printf(0,  16, OLED_6X8, "I:%05.2f", anglePID.Ki);
		OLED_Printf(0,  24, OLED_6X8, "D:%05.2f", anglePID.Kd);
		OLED_Printf(0,  32, OLED_6X8, "Target:%+4.2f", anglePID.Target);
		OLED_Printf(0,  40, OLED_6X8, "Actual:%+4.2f", Angle);
		OLED_Printf(0,  48, OLED_6X8, "Out:%+4.2f", anglePID.Out);
		OLED_Update();
		if (BlueSerial_RxFlag == 1)		//如果收到数据包
		{
			char *Tag = strtok(BlueSerial_RxPacket, ",");	//提取数据1，定义为标签Tag
			if (strcmp(Tag, "key") == 0)					//Tag为key，收到按键数据包
			{
				char *Name = strtok(NULL, ",");				//提取数据2，定义为按键名称
				char *Action = strtok(NULL, ",");			//提取数据3，定义为按键动作
				
				/*此处可执行按键操作，目前程序暂时没用到按键*/
			}
			else if (strcmp(Tag, "slider") == 0)			//Tag为slider，收到滑杆数据包
			{
				char *Name = strtok(NULL, ",");				//提取数据2，定义为滑杆名称
				char *Value = strtok(NULL, ",");			//提取数据3，定义为滑杆值
				
				/*执行滑杆操作*/
				if (strcmp(Name, "AngleKp") == 0)			//如果滑杆名称是AngleKp
				{
					anglePID.Kp = atof(Value);				//则把滑杆值赋值给角度环Kp
				}
				else if (strcmp(Name, "AngleKi") == 0)		//如果滑杆名称是AngleKi
				{
					anglePID.Ki = atof(Value);				//则把滑杆值赋值给角度环Ki
				}
				else if (strcmp(Name, "AngleKd") == 0)		//如果滑杆名称是AngleKd
				{
					anglePID.Kd = atof(Value);				//则把滑杆值赋值给角度环Kd
				}
			}
			else if (strcmp(Tag, "joystick") == 0)			//Tag为joystick，收到摇杆数据包
			{
				int8_t LH = atoi(strtok(NULL, ","));		//提取数据2，定义为摇杆值LH
				int8_t LV = atoi(strtok(NULL, ","));		//提取数据3，定义为摇杆值LV
				int8_t RH = atoi(strtok(NULL, ","));		//提取数据4，定义为摇杆值RH
				int8_t RV = atoi(strtok(NULL, ","));		//提取数据5，定义为摇杆值RV
				
				/*执行摇杆操作*/
			TargetRaw = LV / 10.0f;		/* 摇杆值LV缩放后，控制角度环目标值，前后行进控制 */
			DifPWM = RH / 2;			/* 摇杆值RH缩放后，控制差分PWM，左右转弯控制 */
			}
			
			BlueSerial_RxFlag = 0;				//处理完成后，标志位置0，允许接收下一个数据包
		}
		
		/*蓝牙串口打印波形，需配合蓝牙串口小程序实现波形绘制*/
		BlueSerial_Printf("[plot,%f,%f]", TargetSmooth, Angle);	/* 打印平滑目标值和实际角度波形 */
		/* 串口输出：目标, 实际, 输出 */
		// printf("%f,%f,%f\r\n", gPosPID.Target, gPosPID.Actual, gPosPID.Out);
		// bt_send("speed1:%d,speed2:%d\r\n", speed1, speed2);
		delay_ms(10);
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint16_t Count0;

	if (htim->Instance == TIM1)
	{


		
		Key_Tick();				//调用按键的Tick函数
		
		/*计次分频*/
		Count0 ++;				//计次自增
		if (Count0 >= 10)		//如果计次10次，则if成立，即if每隔10ms进一次
		{
			Count0 = 0;			//计次清零，便于下次计次
			
			/*在中断里读取MPU6050，可以保证读取间隔严格为1ms*/
			/*但要保证MPU6050_GetData执行时间不超过1ms*/
			MPU6050_GetData(&Ax, &Ay, &Az, &Gx, &Gy, &Gz);
			
			/*校准陀螺仪Y轴零漂*/
			/*此值需实测确定，不同的设备零漂一般不同*/
			/*实测方法是，在完全静止时，观察OLED显示的GY值，即为零漂值*/
			/*然后在此处将零漂值减去，使得完全静止时，GY值为0*/
			Gy -= 22;
			
			/*由加速度计计算得到角度值*/
			/*atan2计算反正切，得到角度（弧度制）， / 3.14159 * 180可将弧度制转为角度值*/
			AngleAcc = -atan2(Ax, Az) / 3.14159 * 180;
			
			/*校准中心角度*/
			/*此值需实测确定，不同的设备中心角度一般不同*/
			/*实测方法是，使平衡车绝对竖直，观察OLED显示的角度环实际值，即为中心角度偏移值*/
			/*然后在此处将偏移值减去，使得绝对竖直时，角度环实际值为0*/
			AngleAcc -= 8;
			
			/*由陀螺仪积分得到角度值*/
			/*互补滤波下，角度积分要在上次滤波后的Angle上进行*/
			/*公式中32768是int16_t变量的最大值，2000是陀螺仪配置的满量程2000度每秒，0.01是间隔时间10ms*/
			AngleGyro = Angle + Gy / 32768.0 * 2000 * 0.01;
			
			/*执行互补滤波*/
			float Alpha = 0.01;		//互补滤波系数，值越大，越偏向于加速度计，值越小，越偏向于陀螺仪
			Angle = Alpha * AngleAcc + (1 - Alpha) * AngleGyro;		//互补滤波计算，得到稳定的角度值
			
			/*平衡车倒地后自动停止PID程序*/
			if (Angle > 50 || Angle < -50)	//角度超过-50度~50度的范围，认为平衡车倒地了
			{
				RUNflag = 0;				//RunFlag置0，停止PID程序
			}
			
			/*执行PID调控程序*/
			if (RUNflag)					//RunFlag非0时，启动PID程序
			{

				/*角度环PID控制*/
				anglePID.Actual = Angle;	//角度环实际值为Angle
				PID_Update(&anglePID);		//调用封装好的函数，一步完成PID计算和更新
				AvePWM = -anglePID.Out;		//角度环的输出值给到电机平均PWM，用于控制前后行进
				
				/*控制量转换*/
				LeftPWM = AvePWM + DifPWM / 2;		//由平均PWM和差分PWM计算得到左轮PWM
				RightPWM = AvePWM - DifPWM / 2;		//由平均PWM和差分PWM计算得到右轮PWM
				
				/*PWM限幅*/
				/*上式计算后，LeftPWM和RightPWM可能会超出电机允许的PWM范围，此处将PWM值范围限制在-100~100之内*/
				if (LeftPWM > 50) {LeftPWM = 50;} else if (LeftPWM < -50) {LeftPWM = -50;}
				if (RightPWM > 50) {RightPWM = 50;} else if (RightPWM < -50) {RightPWM = -50;}
				
				/*PWM输出给电机*/
				Motor_SetPWM(1, LeftPWM);		//LeftPWM输出给左轮电机
				Motor_SetPWM(2, RightPWM);		//RightPWM输出给右轮电机
			}
			else		//RunFlag为0时，停止PID程序
			{
				/*左右电机PWM均设置为0*/
				Motor_SetPWM(1, 0);
				Motor_SetPWM(2, 0);
			}
		}
		
	
		
	}
}
