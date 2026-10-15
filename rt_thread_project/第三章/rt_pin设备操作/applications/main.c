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

#define LED_R  GET_PIN(B,15)

#define LED_B  GET_PIN(B,13)

#define KEY_1  GET_PIN(A,6)

void led_entry(void *param)
{
	while(1)
	{
		rt_pin_write(LED_R, PIN_HIGH);
		rt_kprintf("led off\r\n");
		rt_thread_mdelay(500);
		rt_pin_write(LED_R, PIN_LOW);
		rt_kprintf("led on\r\n");
		rt_thread_mdelay(500);
	}
}
void key1_IRQ(void *arg)
{
	int level = rt_pin_read(LED_B);
	
	rt_pin_write(LED_B,!level);

}

int	test_pin(void)
{
//	rt_pin_mode(LED_R,PIN_MODE_OUTPUT);
	rt_pin_mode(LED_B,PIN_MODE_OUTPUT);
	rt_pin_mode(KEY_1,PIN_MODE_INPUT_PULLUP);
	
	rt_pin_attach_irq(KEY_1, PIN_IRQ_MODE_FALLING,key1_IRQ,NULL);
	rt_pin_irq_enable(KEY_1, PIN_IRQ_ENABLE);

	
	rt_thread_t tid = rt_thread_create("led",led_entry,NULL,1024,10,10);
	
//	rt_thread_startup(tid);
	
	return 0;
}
INIT_APP_EXPORT(test_pin);




