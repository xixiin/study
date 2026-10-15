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



int main(void)
{
    int count = 1;



    while (count++)
    {
	
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
		rt_timer_stop(&timer1);
	}
	else
		count++;
}

void timer2_timerout(void* parameter)
{
	FlagStatus state = gpio_input_bit_get(GPIOB,GPIO_PIN_15);
	if(state == SET)
		gpio_bit_reset(GPIOB,GPIO_PIN_15);
	else if(state == RESET)
		gpio_bit_set(GPIOB,GPIO_PIN_15);
}		

int test_timer(void)
{
	rcu_periph_clock_enable(RCU_GPIOB);
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_15);
	gpio_bit_set(GPIOB,GPIO_PIN_15);

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
		500,
		RT_TIMER_FLAG_PERIODIC|RT_TIMER_FLAG_SOFT_TIMER);
	rt_timer_start(&timer1);//静态启动
	rt_timer_start(timer2);//动态启动
	

	return 0;
}

INIT_APP_EXPORT(test_timer);
