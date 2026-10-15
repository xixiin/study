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

struct rt_thread thread1; 
char thread1_stack[1024];

void thread1_entry(void* parameter)
{
	while(1)
	{
		rt_kprintf("thread1_entry\r\n");
		rt_thread_delay(1000);
		//线程挂起
		rt_thread_suspend(&thread1);
		//挂起后必须执行该函数
		rt_schedule();
	}
}
//void thread2_entry(void* parameter)
//{
//	while(1)
//	{
//		rt_kprintf("thread2_entry\r\n");
//		rt_thread_delay(1000);
//	}

//}




int test_thread(void)
{
				rt_thread_init(&thread1,
							"thread1",
						thread1_entry, NULL,
						 thread1_stack,1024,
							10, 10);
//	rt_thread_t thread2 = rt_thread_create("thread2",
//					thread2_entry,
//					NULL,
//					1024,
//					  11,
//					  10);
	//线程创建
	rt_thread_startup(&thread1);
	rt_thread_delay(5000);
	//线程恢复
	 rt_thread_resume(&thread1);
//	rt_thread_startup(thread2);
//	rt_thread_delay(5000);
	//线程删除
//	rt_thread_detach(&thread1);
//	rt_thread_delete(thread2);
	return 0;
}
INIT_APP_EXPORT(test_thread);

