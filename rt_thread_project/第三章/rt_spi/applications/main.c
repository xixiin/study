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
#include "drv_spi.h"
// 1. 绑定SPI总线和CS引脚，然后重新名为一个新设备
// 2. 查找绑定后的新设备
// 3. 配置SPI的总线模式，数据宽度，还有速率
// 4. 读取SPI Flash的JEDEC ID

#define SPI_BUS_NAME       "spi2"
#define spi_CS_PIN           GET_PIN(C, 9)

#define SPI_DEVICE_NAME    "spi_flash"

#define SPI_FLASH_READ_ID_CMD 0x9F

int spi_test(void)
{
    int ret = 0;
  

    ret = rt_hw_spi_device_attach(SPI_BUS_NAME, SPI_DEVICE_NAME, spi_CS_PIN);
    if (ret != RT_EOK)
    {
        rt_kprintf("spi device attach failed, ret=%d\n", ret);
        return -1;
    }
	  struct rt_spi_device *spi_dev = RT_NULL;
    spi_dev = (struct rt_spi_device *)rt_device_find(SPI_DEVICE_NAME);
    if (spi_dev == RT_NULL)
    {
        rt_kprintf("spi device not found\n");
        return -1;
    }

    struct rt_spi_configuration  cfg;
    cfg.data_width = 8;
    cfg.mode = RT_SPI_MODE_0 | RT_SPI_MSB;
    cfg.max_hz = 50000000;
    rt_spi_configure(spi_dev, &cfg);

    rt_uint8_t jedge_cmd = SPI_FLASH_READ_ID_CMD;
    rt_uint8_t recv_data[3] = {0};

   ret = rt_spi_send_then_recv(spi_dev, &jedge_cmd, 1, recv_data, 3);
   rt_thread_mdelay(10);
   if(ret == RT_EOK)
   {
       rt_kprintf("The flash device manufacturer ID is 0x%02X, memory type ID is 0x%02X, capacity ID is 0x%02X\r\n",
                    recv_data[0], recv_data[1], recv_data[2]);
   }
   else
   {
       rt_kprintf("spi flash read jedec id failed, ret=%d\n", ret);
   }

    return 0;
}

MSH_CMD_EXPORT(spi_test, spi test);
