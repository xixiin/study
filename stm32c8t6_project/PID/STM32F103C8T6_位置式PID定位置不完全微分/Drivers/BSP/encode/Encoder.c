#include "sys.h"                  // Device header
#include "Encoder.h"

/* TIM3和TIM4的HAL句柄 */
TIM_HandleTypeDef htim3_Encoder;
TIM_HandleTypeDef htim4_Encoder;

/**
  * 函    数：编码器MSP初始化（时钟、GPIO）
  * 参    数：htim 定时器句柄
  * 返 回 值：无
  * 说    明：由 HAL_TIM_Encoder_Init() 内部自动调用
  */
void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	if (htim->Instance == TIM3)
	{
		/*开启时钟*/
		__HAL_RCC_TIM3_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();

		/*GPIO初始化：PA6(TIM3_CH1), PA7(TIM3_CH2) 上拉输入*/
		GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
	else if (htim->Instance == TIM4)
	{
		/*开启时钟*/
		__HAL_RCC_TIM4_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();

		/*GPIO初始化：PB6(TIM4_CH1), PB7(TIM4_CH2) 上拉输入*/
		GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
}

/**
  * 函    数：编码器初始化
  * 参    数：无
  * 返 回 值：无
  */
void Encoder_Init(void)
{
	TIM_Encoder_InitTypeDef sEncoderConfig = {0};

	/*==================== TIM3用于编码器1 ====================*/

	/*TIM3时基单元配置*/
	htim3_Encoder.Instance = TIM3;
	htim3_Encoder.Init.Prescaler = 1 - 1;                       //预分频器，不分频
	htim3_Encoder.Init.CounterMode = TIM_COUNTERMODE_UP;        //向上计数
	htim3_Encoder.Init.Period = 65536 - 1;                      //自动重装载值
	htim3_Encoder.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;  //时钟不分频
	htim3_Encoder.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	/*TIM3编码器接口配置*/
	sEncoderConfig.EncoderMode = TIM_ENCODERMODE_TI12;          //编码器模式：TI1和TI2均计数
	sEncoderConfig.IC1Polarity = TIM_ICPOLARITY_RISING;         //IC1极性（编码器模式下代表是否反相）
	sEncoderConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;     //IC1输入选择：直连TI1
	sEncoderConfig.IC1Prescaler = TIM_ICPSC_DIV1;               //IC1预分频，不分频
	sEncoderConfig.IC1Filter = 0xF;                             //IC1输入滤波器
	sEncoderConfig.IC2Polarity = TIM_ICPOLARITY_RISING;        //IC2极性（编码器模式下代表是否反相）
	sEncoderConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;     //IC2输入选择：直连TI2
	sEncoderConfig.IC2Prescaler = TIM_ICPSC_DIV1;               //IC2预分频，不分频
	sEncoderConfig.IC2Filter = 0xF;                             //IC2输入滤波器

	HAL_TIM_Encoder_Init(&htim3_Encoder, &sEncoderConfig);      //内部会调用 HAL_TIM_Encoder_MspInit

	/*启动编码器模式，使能所有通道*/
	HAL_TIM_Encoder_Start(&htim3_Encoder, TIM_CHANNEL_ALL);

	/*==================== TIM4用于编码器2 ====================*/

	/*TIM4时基单元配置*/
	htim4_Encoder.Instance = TIM4;
	htim4_Encoder.Init.Prescaler = 1 - 1;
	htim4_Encoder.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim4_Encoder.Init.Period = 65536 - 1;
	htim4_Encoder.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim4_Encoder.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	/*TIM4编码器接口配置*/
	sEncoderConfig.EncoderMode = TIM_ENCODERMODE_TI12;
	sEncoderConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
	sEncoderConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
	sEncoderConfig.IC1Prescaler = TIM_ICPSC_DIV1;
	sEncoderConfig.IC1Filter = 0xF;
	sEncoderConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
	sEncoderConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
	sEncoderConfig.IC2Prescaler = TIM_ICPSC_DIV1;
	sEncoderConfig.IC2Filter = 0xF;

	HAL_TIM_Encoder_Init(&htim4_Encoder, &sEncoderConfig);      //内部会调用 HAL_TIM_Encoder_MspInit

	/*启动编码器模式，使能所有通道*/
	HAL_TIM_Encoder_Start(&htim4_Encoder, TIM_CHANNEL_ALL);
}

/**
  * 函    数：获取编码器的增量值（CNT值）
  * 参    数：n 指定哪个编码器，范围1~2
  * 返 回 值：编码器的增量值（CNT值）
  */
int16_t Encoder_Get(uint8_t n)
{
	int16_t cnt_value = 0;
	if (n == 1)			//指定编码器1
	{
		cnt_value = __HAL_TIM_GET_COUNTER(&htim3_Encoder);	//读取TIM3的CNT
		__HAL_TIM_SET_COUNTER(&htim3_Encoder, 0);	//清零CNT
	}
	else if (n == 2)	//指定编码器2
	{
		cnt_value = __HAL_TIM_GET_COUNTER(&htim4_Encoder);	//读取TIM4的CNT
		__HAL_TIM_SET_COUNTER(&htim4_Encoder, 0);	//清零CNT
	}
	return cnt_value;	//返回编码器的增量值
}

/**
  * 函    数：设置编码器的位置值（CNT值）
  * 参    数：n 指定哪个编码器，范围1~2
  * 参    数：Location 编码器的位置值（CNT值）
  * 返 回 值：无
  */
void Encoder_SetLocation(uint8_t n, int16_t Location)
{
	if (n == 1)			//指定编码器1
	{
		__HAL_TIM_SET_COUNTER(&htim3_Encoder, Location);	//设置TIM3的CNT
	}
	else if (n == 2)	//指定编码器2
	{
		__HAL_TIM_SET_COUNTER(&htim4_Encoder, Location);	//设置TIM4的CNT
	}
}
