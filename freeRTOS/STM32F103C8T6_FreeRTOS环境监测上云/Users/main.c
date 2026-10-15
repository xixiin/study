#include "sys.h"
#include "uart1.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "OLED.h"
#include "esp8266.h"
#include "FreeRTOS_demo.h"
#include "app_port.h"

int main(void)
{
    HAL_Init();                          /* init HAL */
    stm32_clock_init(RCC_PLL_MUL9);      /* 72MHz */
    uart1_init(115200);

    printf("running...\r\n");

    /* init sensors, OLED, key, esp8266 + MQTT (app_port.c) */
    APPPORT_Init();

    /* create tasks/queues/event group/timer, then start scheduler */
    FreeRTOS_Start();

    while (1)
    {
        /* scheduler is running; main never really reaches here */
    }
}
