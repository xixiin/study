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

//struct rt_semaphore sem;
rt_sem_t sem1;

void thread1_entry(void* parameter)
{
	while(1)
	{
		rt_sem_take (sem1, RT_WAITING_FOREVER);
		rt_kprintf("thread1_entry\r\n");	
		rt_thread_delay(1000);
	}
}
void thread2_entry(void* parameter)
{
	while(1)
	{
		 
		rt_kprintf("hello world\r\n");
		rt_thread_delay(3000);
		rt_sem_release(sem1);
	}

}




int test_thread(void)
{

	
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
//	rt_sem_init(&sem,
//				"sem",
//					1,
//				RT_IPC_FLAG_PRIO);
	 sem1 = rt_sem_create("sem1",
							1,
				RT_IPC_FLAG_FIFO);

	//线程创建
	rt_thread_startup(&thread1);	
	rt_thread_startup(thread2);
	
	return 0;
}
INIT_APP_EXPORT(test_thread);

