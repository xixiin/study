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
#include "string.h"



int main(void)
{
    int count = 1;



    while (count++)
    {

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
	char msg_send[16] = "key down";
	while(1)
	{
		FlagStatus key1 = gpio_input_bit_get(GPIOA, GPIO_PIN_6);
		if(key1 == RESET)
		{
				rt_mq_send_wait(mq,
			msg_send,
				8,
			RT_WAITING_FOREVER);
			rt_kprintf("key down\r\n");
		}

		rt_thread_delay(200);
		
	}
}
void thread2_entry(void* parameter)
{

	char msg_receive[16];
	while(1)
	{
		rt_mq_recv (mq, msg_receive,
				16, RT_WAITING_FOREVER);
	
		if(strcmp(msg_receive,"key down")==0)
		{
			FlagStatus led = gpio_input_bit_get(GPIOB, GPIO_PIN_15);
			if(led == RESET)
			{
					gpio_bit_set(GPIOB,GPIO_PIN_15);
			}
			else if(led == SET)
				gpio_bit_reset(GPIOB,GPIO_PIN_15);
		}	
	}

}




int test_thread(void)
{
	//静态创建
//	rt_mq_init(&mq,"mq",
//		mq_pool,16,
//		64, RT_IPC_FLAG_FIFO);
	//动态创建
	rcu_periph_clock_enable(RCU_GPIOA);
	gpio_init(GPIOA, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ,GPIO_PIN_6);
		rcu_periph_clock_enable(RCU_GPIOB);
		gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_15);
			

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

