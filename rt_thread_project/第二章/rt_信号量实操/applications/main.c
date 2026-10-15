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
		rt_thread_delay(500);
    }

    return RT_EOK;
}

struct rt_thread thread1; 
char thread1_stack[1024];

struct rt_semaphore sem;

void thread1_entry(void* parameter)
{
	while(1)
	{
		rt_sem_take (&sem, RT_WAITING_FOREVER);
		FlagStatus state = gpio_input_bit_get(GPIOB, GPIO_PIN_13);
		if(state == SET)	
			gpio_bit_reset(GPIOB,GPIO_PIN_13);
		else if(state == RESET)
			gpio_bit_set(GPIOB,GPIO_PIN_13);
		rt_thread_delay(100);
	}
}
void thread2_entry(void* parameter)
{
	while(1)
	{
		FlagStatus state = gpio_input_bit_get(GPIOA, GPIO_PIN_6);
		if(state == RESET)
			rt_sem_release(&sem);

		rt_thread_delay(200);
	}

}




int test_thread(void)
{
	rcu_periph_clock_enable(RCU_GPIOA);
	gpio_init(GPIOA, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
	
	rcu_periph_clock_enable(RCU_GPIOB);
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
	
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
	rt_sem_init(&sem,
			"sem",
			1,
			RT_IPC_FLAG_FIFO);

	rt_thread_startup(&thread1);	
	rt_thread_startup(thread2);
	
	return 0;
}
INIT_APP_EXPORT(test_thread);

