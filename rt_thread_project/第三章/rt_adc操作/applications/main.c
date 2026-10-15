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

    }

    return RT_EOK;
}

//rt_device_t adc;

rt_adc_device_t	adc;

void tid_entry(void *param)
{
	uint32_t value;
	while(1)
	{
		value = rt_adc_read(adc, 4);
		rt_kprintf("adc 4 value: %d\n", value);
		value = rt_adc_read(adc, 5);
		rt_kprintf("adc 5 value: %d\n", value);
		rt_thread_mdelay(1000);

	}
}

int adc_test(void)
{
	adc = (rt_adc_device_t )rt_device_find("adc0");
	
	rt_adc_enable(adc, 4);
	rt_adc_enable(adc, 5);

	rt_thread_t tid = rt_thread_create("tid",tid_entry,NULL,1024,10,10);
	rt_thread_startup(tid);
	return 0;
}

INIT_APP_EXPORT(adc_test);
