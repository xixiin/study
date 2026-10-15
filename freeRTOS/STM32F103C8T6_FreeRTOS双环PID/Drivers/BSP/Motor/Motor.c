#include "sys.h"              // Device header
#include "PWM.h"
#include "Motor.h"
/**
  * 函    数：直流电机初始化
  * 参    数：无
  * 返 回 值：无
  */
void Motor_Init(void)
{
	/*开启时钟*/
	__HAL_RCC_GPIOB_CLK_ENABLE();                               //开启GPIOB的时钟

	GPIO_InitTypeDef GPIO_InitStructure = {0};
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pin = GPIO_PIN_12 | GPIO_PIN_13;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);                  //将PB12、PB13引脚初始化为推挽输出

	/*初始化方向引脚为刹车状态，防止电机上电即转*/
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);

	pwm_init();                                                 //初始化直流电机的底层PWM
}

/**
  * 函    数：直流电机设置PWM
  * 参    数：Duty 要设置的PWM值，范围：-100~100（负数为反转）
  * 返 回 值：无
  */
void Motor_SetPWM(int16_t PWM)
{
	if (PWM >= 0)                                              //如果设置正转
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET); //置方向控制引脚电平
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
		pwm_compare_set(PWM);                                  //设置PWM占空比
	}
	else                                                        //否则，即设置反转
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);   //置方向控制引脚电平
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
		pwm_compare_set(-PWM);                                 //设置PWM占空比
	}
}
