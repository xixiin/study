/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-08-20     RiceChen      first implementation
 */

#include <stdio.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

/* defined the LED1 pin: PC13 */
#define LED_PIN GET_PIN(C, 13)

int main(void)
{
    int count = 1;

    /* set LED2 pin mode to output */
    rt_pin_mode(LED_PIN, PIN_MODE_OUTPUT);

    while (count++)
    {
        rt_pin_write(LED_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }

    return RT_EOK;
}

struct rt_timer timer1;

void timer1_timerout(void* parameter)
{
	static int	count=0;
	rt_kprintf("timer1_timerout\r\n");
	if(count==10)
	{
		rt_timer_detach(&timer1);
	}
	else
		count++;
}

void timer2_timerout(void* parameter)
{
	rt_kprintf("timer2_timerout\r\n");
}

int test_timer(void)
{
	//静态创建定时器
	rt_timer_init(&timer1,
	"timer1",
	timer1_timerout,
	RT_NULL,
	1000, RT_TIMER_FLAG_PERIODIC|RT_TIMER_FLAG_HARD_TIMER);
	//动态创建定时器
	rt_timer_t timer2 = rt_timer_create("timer2",
		timer2_timerout,
		RT_NULL,
		2000,
		RT_TIMER_FLAG_ONE_SHOT|RT_TIMER_FLAG_SOFT_TIMER);
	rt_timer_start(&timer1);//静态启动
	rt_timer_start(timer2);//动态启动
	
	rt_thread_delay(5000);
	
//	rt_timer_detach(&timer1);//静态删除
	rt_timer_delete(timer2);//动态删除
	return 0;
}

INIT_APP_EXPORT(test_timer);
