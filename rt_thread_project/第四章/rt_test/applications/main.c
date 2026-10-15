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

//int	test1(void)
//{
//	rt_kprintf("test1 init\r\n");
//	return 0;
//}
//INIT_BOARD_EXPORT(test1);

//int test(void)
//{
//	rt_kprintf("test init\r\n");
//	return 0;
//}
//INIT_APP_EXPORT(test);
#include <fcntl.h>
#include <unistd.h>
static void readwrite_sample(void)
{
	int fd, size;
	char s[] = "RT-Thread Programmer!", buffer[80];
		rt_kprintf("Write string %s to test.txt.\n", s);
	/* 以创建和读写模式打开 /text.txt 文件，如果该文件不存在则创建该文件 */
		fd = open("/text.txt", O_WRONLY | O_CREAT);
	if (fd>= 0)
	{
		write(fd, s, sizeof(s));
		close(fd);
		rt_kprintf("Write done.\n");
	}
	/* 以只读模式打开 /text.txt 文件 */
	fd = open("/text.txt", O_RDONLY);
	if (fd>= 0)
	{
		size = read(fd, buffer, sizeof(buffer));
		close(fd);
		rt_kprintf("Read from file test.txt : %s \n", buffer);
	if (size < 0)
		return ;
}
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(readwrite_sample, readwrite sample);


