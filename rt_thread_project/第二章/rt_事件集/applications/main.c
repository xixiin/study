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

//struct rt_event event;
rt_event_t event;
#define test_event (0x01<<0)

void thread1_entry(void* parameter)
{
	while(1)
	{
		rt_event_send(event, test_event);
		rt_kprintf("thread1_entry\r\n");
		rt_thread_delay(3000);
		
	}
}
void thread2_entry(void* parameter)
{
	uint32_t receive_event;
	while(1)
	{
		 rt_event_recv(event,test_event,
						RT_EVENT_FLAG_OR|RT_EVENT_FLAG_CLEAR,
						RT_WAITING_FOREVER,
						&receive_event);
		if(receive_event & test_event)
			rt_kprintf("thread2_entry\r\n");
//		rt_thread_delay(1000);
		
	}

}




int test_thread(void)
{
	//静态创建事件集
//	rt_event_init(&event, "event", RT_IPC_FLAG_FIFO);
	//动态创建事件集
	event =  rt_event_create("event", RT_IPC_FLAG_FIFO);
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

