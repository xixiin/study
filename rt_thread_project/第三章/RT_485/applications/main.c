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
#define RT_USING_UART485   GET_PIN(A,1)
// 1. 初始化485所用到的串口
// 2. 创建一个信号量，用与串口接收中断与线程的同步
// 3. 设置串口中断服务程序
// 4. 创建一个线程用于读取串口数据
// 5. 初始化控制485芯片的RE和DE引脚。默认初始为高低电平
// 6. 创建一个命令行，用于发送数据
rt_device_t uart_485_dev = RT_NULL;
struct rt_semaphore uart_485_rx_sem = RT_NULL;

rt_err_t uart_485_rx_ind(rt_device_t dev,rt_size_t size)
{
    rt_sem_release(&uart_485_rx_sem);
    return RT_EOK;

}

void uart_485_read_entry(void *parameter)
{
    char ch;
    while (1)
    {
        while (rt_device_read(uart_485_dev, 0, &ch, 1) != 1)
        {
            rt_sem_take(&uart_485_rx_sem, RT_WAITING_FOREVER);
        }
        rt_kprintf("%c", ch);
    }
}

int uart_485_init(void)
{
    /* 1. 初始化485所用到的串口 */
    uart_485_dev = rt_device_find("uart1");
    if (uart_485_dev == RT_NULL)
    {
        rt_kprintf("find uart1 failed\n");
        return -1;
    }

    rt_device_open(uart_485_dev, RT_DEVICE_FLAG_INT_RX);
    /* 2. 创建一个信号量，用与串口接收中断与线程的同步 */
    rt_sem_init(&uart_485_rx_sem, "uart485_rx", 0, RT_IPC_FLAG_FIFO);

    rt_device_set_rx_indicate(uart_485_dev, uart_485_rx_ind);

    rt_thread_t tid = rt_thread_create("uart485_read", uart_485_read_entry, RT_NULL, 1024, 20, 10);
    rt_thread_startup(tid);
    rt_pin_mode(RT_USING_UART485, PIN_MODE_OUTPUT);
    rt_pin_write(RT_USING_UART485, PIN_LOW);
	return 0;
}
INIT_APP_EXPORT(uart_485_init);
void uart_485_send(void)
{
    char *ch = "RT_Thread\r\n";
    rt_pin_write(RT_USING_UART485, PIN_HIGH);
    rt_device_write(uart_485_dev, 0, ch, rt_strlen(ch)+1);
    rt_pin_write(RT_USING_UART485, PIN_LOW);
}

MSH_CMD_EXPORT(uart_485_send, send data to uart485);
