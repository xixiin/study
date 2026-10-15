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

// 1. 获取I2C总线
// 2. 向AT24C32的0地址写入一个字节
// 3. 从AT24C32的0地址读取一个字节
// 4. 查看写入的值和读取的值是否一致
#define I2C_BUS_NAME      "i2c1" /* I2C总线设备名称，根据实际情况修改 */

#define AT24C32_ADDR      0x50   /* AT24C32的I2C地址，根据实际情况修改 */

int ii2_test(int argc,char *argv[])
{
    struct rt_i2c_bus_device *i2c_bus = RT_NULL;
    i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(I2C_BUS_NAME);
    if (i2c_bus == RT_NULL)
    {
        rt_kprintf("i2c bus device %s not found!\n", I2C_BUS_NAME);
        return -RT_ERROR;
    }

    struct rt_i2c_msg msgs;
	rt_uint8_t write_buff[3] = {0x00,0x00,0x5A}; //高地址 低地址 要写入的数据
    msgs.addr = AT24C32_ADDR;
    msgs.flags = RT_I2C_WR;
    msgs.buf = write_buff;
    msgs.len = 3;

    int ret;
    ret = rt_i2c_transfer(i2c_bus, &msgs, 1); // 写入数据
    if(ret != 1)
    {
        rt_kprintf("i2c write failed!\n");
        return -RT_ERROR;
    }
	
	rt_thread_delay(10);
	
    rt_uint8_t read_buff[2] = {0x00,0x00}; // 用于存储读取的地址
    rt_uint8_t read_data[2] = {0}; // 要读取的数据
	
    struct rt_i2c_msg msgs_read[2];
    msgs_read[0].addr = AT24C32_ADDR;
    msgs_read[0].flags = RT_I2C_WR;
    msgs_read[0].buf = read_buff;
    msgs_read[0].len = 2;
	
    msgs_read[1].addr = AT24C32_ADDR;
    msgs_read[1].flags = RT_I2C_RD;
    msgs_read[1].buf = read_data;
    msgs_read[1].len = 1;

    ret = rt_i2c_transfer(i2c_bus, msgs_read, 2); // 读取数据
    if(ret != 2)
    {
        rt_kprintf("i2c read failed!\n");
        return -RT_ERROR;
    }

    rt_kprintf("write data: 0x%02X, read data: 0x%02X\n", write_buff[2], read_data[0]);

	return 0;
}

MSH_CMD_EXPORT(ii2_test, ii2_test function);
