#include "sys.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

char BlueSerial_RxPacket[100];		//定义接收数据包数组，数据包格式"[MSG]"
uint8_t BlueSerial_RxFlag;			//定义蓝牙串口接收的标志位变量
UART_HandleTypeDef uart2_handle; 
/**
  * 函    数：蓝牙串口初始化
  * 参    数：无
  * 返 回 值：无
  */
void BlueSerial_Init(void)
{
    /*UART1 初始化设置*/
    uart2_handle.Instance = USART2;                                         /* USART1 */
    uart2_handle.Init.BaudRate = 115200;                                  /* 波特率 */
    uart2_handle.Init.WordLength = UART_WORDLENGTH_8B;                      /* 字长为8位数据格式 */
    uart2_handle.Init.StopBits = UART_STOPBITS_1;                           /* 一个停止位 */
    uart2_handle.Init.Parity = UART_PARITY_NONE;                            /* 无奇偶校验位 */
    uart2_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;                      /* 无硬件流控 */
    uart2_handle.Init.Mode = UART_MODE_TX_RX;                               /* 收发模式 */
    HAL_UART_Init(&uart2_handle);                                           /* HAL_UART_Init()会使能UART1 */
}

/**
  * 函    数：蓝牙串口发送一个字节
  * 参    数：Byte 要发送的一个字节
  * 返 回 值：无
  */
void BlueSerial_SendByte(uint8_t Byte)
{
	HAL_UART_Transmit(&uart2_handle, &Byte, 1, 100);
	/*下次写入数据寄存器会自动清除发送完成标志位，故此循环后，无需清除标志位*/
}

/**
  * 函    数：蓝牙串口发送一个数组
  * 参    数：Array 要发送数组的首地址
  * 参    数：Length 要发送数组的长度
  * 返 回 值：无
  */
void BlueSerial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)			//遍历数组
	{
		BlueSerial_SendByte(Array[i]);		//依次调用BlueSerial_SendByte发送每个字节数据
	}
}

/**
  * 函    数：蓝牙串口发送一个字符串
  * 参    数：String 要发送字符串的首地址
  * 返 回 值：无
  */
void BlueSerial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)	//遍历字符数组（字符串），遇到字符串结束标志位后停止
	{
		BlueSerial_SendByte(String[i]);		//依次调用BlueSerial_SendByte发送每个字节数据
	}
}

/**
  * 函    数：次方函数（内部使用）
  * 返 回 值：返回值等于X的Y次方
  */
uint32_t BlueSerial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;	//设置结果初值为1
	while (Y --)			//执行Y次
	{
		Result *= X;		//将X累乘到结果
	}
	return Result;
}

/**
  * 函    数：蓝牙串口发送数字
  * 参    数：Number 要发送的数字，范围：0~4294967295
  * 参    数：Length 要发送数字的长度，范围：0~10
  * 返 回 值：无
  */
void BlueSerial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)		//根据数字长度遍历数字的每一位
	{
		BlueSerial_SendByte(Number / BlueSerial_Pow(10, Length - i - 1) % 10 + '0');	//依次调用BlueSerial_SendByte发送每位数字
	}
}

/**
  * 函    数：自己封装的prinf函数
  * 参    数：format 格式化字符串
  * 参    数：... 可变的参数列表
  * 返 回 值：无
  */
void BlueSerial_Printf(char *format, ...)
{
	char String[100];				//定义字符数组
	va_list arg;					//定义可变参数列表数据类型的变量arg
	va_start(arg, format);			//从format开始，接收参数列表到arg变量
	vsprintf(String, format, arg);	//使用vsprintf打印格式化字符串和参数列表到字符数组中
	va_end(arg);					//结束变量arg
	BlueSerial_SendString(String);	//蓝牙串口发送字符数组（字符串）
}

/**
  * 函    数：USART2中断函数
  * 参    数：无
  * 返 回 值：无
  * 注意事项：此函数为中断函数，无需调用，中断触发后自动执行
  *           函数名为预留的指定名称，可以从启动文件复制
  *           请确保函数名正确，不能有任何差异，否则中断函数将不能进入
  */
void USART2_IRQHandler(void)
{
	static uint8_t RxState = 0;		//定义表示当前状态机状态的静态变量
	static uint8_t pRxPacket = 0;	//定义表示当前接收数据位置的静态变量
	if (__HAL_UART_GET_FLAG(&uart2_handle, UART_FLAG_RXNE) != RESET)	//判断是否是USART2的接收事件触发的中断
	{
		uint8_t RxData = (uint8_t)(uart2_handle.Instance->DR & 0xFF);	//读取数据寄存器，存放在接收的数据变量（读DR自动清除RXNE标志）
		
		/*使用状态机的思路，依次处理数据包的不同部分*/
		
		/*当前状态为0，接收数据包包头*/
		if (RxState == 0)
		{
			if (RxData == '[' && BlueSerial_RxFlag == 0)		//如果数据确实是包头，并且上一个数据包已处理完毕
			{
				RxState = 1;			//置下一个状态
				pRxPacket = 0;			//数据包的位置归零
			}
		}
		/*当前状态为1，接收数据包数据，同时判断是否接收到了包尾*/
		else if (RxState == 1)
		{
			if (RxData == ']')			//如果收到包尾
			{
				RxState = 0;			//状态归0
				BlueSerial_RxPacket[pRxPacket] = '\0';			//将收到的字符数据包添加一个字符串结束标志
				BlueSerial_RxFlag = 1;	//接收数据包标志位置1，成功接收一个数据包
			}
			else						//接收到了正常的数据
			{
				BlueSerial_RxPacket[pRxPacket] = RxData;		//将数据存入数据包数组的指定位置
				pRxPacket ++;			//数据包的位置自增
			}
		}
		//HAL库中读取DR寄存器会自动清除RXNE标志位，无需手动清除
	}
	if (__HAL_UART_GET_FLAG(&uart2_handle, UART_FLAG_IDLE) != RESET)	//处理空闲中断，防止IDLE标志未清除导致中断卡死
	{
		__HAL_UART_CLEAR_IDLEFLAG(&uart2_handle);		//清除UART总线空闲中断标志
	}
}
