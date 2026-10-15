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

struct rt_semaphore can_rx_sem;
rt_device_t can;
rt_err_t can_rx_ind(rt_device_t dev,rt_size_t size)
{
    rt_sem_release(&can_rx_sem);
    return RT_EOK;
}

void can_rx_thread_entry(void *parameter)
{
    struct rt_can_msg rx_msg;
    while(1)
    {
        rt_sem_take(&can_rx_sem, RT_WAITING_FOREVER);
        
        rt_device_read(can, 0, &rx_msg, sizeof(rx_msg));
        rt_kprintf("can receive: id=0x%X, len=%d, data=", rx_msg.id, rx_msg.len);
        for(int i=0; i<rx_msg.len; i++)
        {
            rt_kprintf("%02X ", rx_msg.data[i]);
        }
        rt_kprintf("\n");
    }

}
// 1. 初始化can0总线
// 2. 使能can0的tx中断和rx中断，使能can的过滤
// 3. 创建一个信号量，用与can接收中断与线程的同步
// 4. 设置can中断服务程序
// 5. 创建一个线程用于读取can数据
// 6. 创建一个命令行，用于发送数据
int rt_can_init(void)
{
	// 1. 初始化can0总线
	can = rt_device_find("can0");
	if (can == RT_NULL)
	{
		rt_kprintf("can0 device not found!\n");
		return -1;
	}


	// 2. 使能can0的tx中断和rx中断，使能can的过滤
	rt_device_control(can, RT_DEVICE_CTRL_SET_INT, (void*)RT_DEVICE_FLAG_INT_TX);
    rt_device_control(can, RT_DEVICE_CTRL_SET_INT, (void*)RT_DEVICE_FLAG_INT_RX);
    rt_device_control(can, RT_CAN_CMD_SET_FILTER, NULL);


	// 3. 创建一个信号量，用与can接收中断与线程的同步
	rt_sem_init(&can_rx_sem, "can_rx_sem", 0, RT_IPC_FLAG_FIFO);
    rt_device_open(can, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX|RT_DEVICE_FLAG_INT_TX);
    // 4. 设置can中断服务程序
    rt_device_set_rx_indicate(can, can_rx_ind);

    //5. 创建一个线程用于读取can数据
    rt_thread_t can_rx_thread = rt_thread_create("can_rx", can_rx_thread_entry, RT_NULL, 1024, 10, 10);
    rt_thread_startup(can_rx_thread);
	return 0;
}

INIT_APP_EXPORT(rt_can_init);

int can_send(void)
{
    struct rt_can_msg tx_msg;
    tx_msg.id = 0x123;
    tx_msg.ide = RT_CAN_STDID;
    tx_msg.rtr = RT_CAN_DTR;
    tx_msg.len = 8;
    for(int i=0; i<tx_msg.len; i++)
    {
        tx_msg.data[i] = i;
    }
    rt_device_write(can, 0, &tx_msg, sizeof(tx_msg));

}

MSH_CMD_EXPORT(can_send,can_send);