#include "sys.h"
#include "uart1.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "OLED.h"
#include "FreeRTOS_demo.h"
#include "timer.h"
#include "Motor.h"
#include "Encoder.h"
#include "MPU6050.h"
#include "bluetoos.h"

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */                       
    uart1_init(115200);
    printf("running...\r\n");
	printf("你好\r\n");
	led_init();
	OLED_Init();
    Motor_Init();
    Encoder_Init();
    key_init();
    MPU6050_Init();
    bt_init(115200);
    // timer_init(10-1,72-1);
    // Motor_SetPWM(30);
	FreeRTOS_Start();
	
	
    while(1)
    { 

    }
}

