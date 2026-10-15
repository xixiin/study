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

//静态创建
//struct rt_messagequeue mq;
//char mq_pool[16*4];
//动态创建
rt_mq_t mq;
void thread1_entry(void* parameter)
{
	char msg_send[16] = "test mq";
	while(1)
	{
		rt_mq_send_wait(mq,
			msg_send,
				7,
			RT_WAITING_FOREVER);
		rt_kprintf("thread1_entry\r\n");
		rt_thread_delay(1000);
		
	}
}
void thread2_entry(void* parameter)
{
	int size;
	char msg_receive[16];
	while(1)
	{
		size = rt_mq_recv (mq, msg_receive,
				16, RT_WAITING_FOREVER);
		rt_kprintf("thread2_msg:%s size:%d\r\n",msg_receive,size);
		
	}

}




int test_thread(void)
{
	//静态创建
//	rt_mq_init(&mq,"mq",
//		mq_pool,16,
//		64, RT_IPC_FLAG_FIFO);
	//动态创建
	mq = rt_mq_create("mq", 16,
				4, RT_IPC_FLAG_PRIO);
				rt_thread_init(&thread1,
							"thread1",
						thread1_entry, NULL,
						 thread1_stack,1024,
							10, 10);
	rt_thread_t thread2 = rt_thread_create("thread2",
					thread2_entry,
					NULL,
					1024,
					  11,
					  10);
	//线程创建
	rt_thread_startup(&thread1);

	rt_thread_startup(thread2);

	return 0;
}
INIT_APP_EXPORT(test_thread);

