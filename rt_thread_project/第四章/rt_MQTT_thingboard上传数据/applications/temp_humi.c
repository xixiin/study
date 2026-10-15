#include "temp_humi.h"
#include "warm.h"

#define UART_485_ENABLE_PIN  GET_PIN(A, 1)
rt_device_t uart_485 = RT_NULL;
struct rt_semaphore uart_485_sem;
rt_thread_t recv_thread = RT_NULL;
rt_thread_t send_thread = RT_NULL;

rt_uint8_t recv_buffer[128] = {0};

rt_err_t uart_rx_ind(rt_device_t dev, rt_size_t size)
{
    rt_sem_release(&uart_485_sem);

    return RT_EOK;

}

void recv_thread_entry(void *parameter)
{
    rt_uint8_t recv_ch = 0;
    int idx = 0;
    int state = 0;   /* 0:等地址 1:功能码 2:字节数 3:数据 4:CRC */
    int data_len = 0;
    while(1)
    {
        while(rt_device_read(uart_485, 0, &recv_ch, 1) != 1)
        {
            rt_sem_take(&uart_485_sem, RT_WAITING_FOREVER);
        }

        switch(state)
        {
        case 0:  /* 等待地址字节 0x01，丢弃错位字节 */
            if(recv_ch == 0x01)
            {
                recv_buffer[0] = recv_ch;
                idx = 1;
                state = 1;
            }
            break;
        case 1:  /* 功能码 */
            recv_buffer[1] = recv_ch;
            if(recv_ch == 0x03)
            {
                idx = 2;
                state = 2;
            }
            else
                state = 0;
            break;
        case 2:  /* 字节数 */
            recv_buffer[2] = recv_ch;
            data_len = recv_ch;
            if(data_len > 0 && data_len <= 32)
            {
                idx = 3;
                state = 3;
            }
            else
                state = 0;
            break;
        case 3:  /* 数据区 */
            recv_buffer[idx++] = recv_ch;
            if(idx == 3 + data_len)
                state = 4;
            break;
        case 4:  /* CRC 2字节 */
            recv_buffer[idx++] = recv_ch;
            if(idx == 3 + data_len + 2)
            {
                state = 0;
                {
                    rt_uint16_t humi = ((rt_uint16_t)recv_buffer[3] << 8) | recv_buffer[4];
                    rt_uint16_t temp_raw = ((rt_uint16_t)recv_buffer[5] << 8) | recv_buffer[6];
                    rt_kprintf("humi: %d, temp: %d\r\n", humi, (rt_int16_t)temp_raw);
                    float humidity = humi / 10.0f;
                    float temperature = (rt_int16_t)temp_raw / 10.0f;
                    if(humidity > 80.0f)
                        warm_set(HUMI_WARM);
                    if(temperature > 35.0f)
                        warm_set(TEMP_WARM);
                    cJSON *root = cJSON_CreateObject();
                    cJSON_AddNumberToObject(root, "humi", humidity);
                    cJSON_AddNumberToObject(root, "temp", temperature);
                    char *json_string = cJSON_Print(root);
                    extern void mqtt_pub(void *msg);
                    mqtt_pub(json_string);
                    cJSON_Delete(root);
                    rt_free(json_string);
                }
            }
            break;
        }
    }
}

void send_thread_entry(void *parameter)
{
    char send_data[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x10, 0x44, 0x06};
    while (1)
    {
        rt_pin_write(UART_485_ENABLE_PIN, PIN_HIGH);
        rt_device_write(uart_485, 0, send_data, sizeof(send_data));
        /* 8字节 @9600bps 约需8.3ms，等待发送完成后再切回接收 */
        rt_thread_mdelay(10);
        rt_pin_write(UART_485_ENABLE_PIN, PIN_LOW);

        rt_thread_mdelay(3000);
    }
}

int temp_humi_init(void)
{   
    rt_sem_init(&uart_485_sem, "uart_485_sem", 0, RT_IPC_FLAG_FIFO);

    uart_485 = rt_device_find("uart1");
    if(uart_485 == RT_NULL)
    {
        rt_kprintf("uart device not found!\n");
        return -1;
    }
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;
    config.baud_rate = BAUD_RATE_9600;
    config.data_bits = DATA_BITS_8;
    config.stop_bits = STOP_BITS_1;
    config.parity    = PARITY_NONE;
    config.bufsz     = 128;
    rt_device_control(uart_485, RT_DEVICE_CTRL_CONFIG, &config);

    rt_device_open(uart_485, RT_DEVICE_FLAG_INT_RX);
    rt_device_set_rx_indicate(uart_485, uart_rx_ind);

    rt_pin_mode(UART_485_ENABLE_PIN, PIN_MODE_OUTPUT);
    rt_pin_write(UART_485_ENABLE_PIN, PIN_LOW);

    recv_thread = rt_thread_create("recv_thread", recv_thread_entry, RT_NULL, 1024, 10, 20);
    if(recv_thread == RT_NULL)
    {
        rt_kprintf("recv thread create failed!\n");
        return -1;
    }
    rt_thread_startup(recv_thread);
    send_thread = rt_thread_create("send_thread", send_thread_entry, RT_NULL, 1024, 10, 20);
    if(send_thread == RT_NULL)
    {
        rt_kprintf("send thread create failed!\n");
        return -1;
    }
    rt_thread_startup(send_thread);
    return RT_EOK;
}