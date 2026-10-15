#include "relay.h"

// 创建一个消息队列，将监听到的数据通过消息队列发送给继电器处理线程
// 创建一个线程，解析接收的消息队列的数据，判断哪个开关，及状态，控制继电器

rt_mq_t relay_mq = RT_NULL;
static rt_thread_t relay_thread = RT_NULL;

#define UOT1_PIN GET_PIN(C, 6)
#define UOT2_PIN GET_PIN(C, 7)
#define UOT3_PIN GET_PIN(C, 8)

void relay_send(char *msg)
{
    rt_mq_send(relay_mq, msg, rt_strlen(msg)+1 );
}

static void relay_thread_entry(void *parameter)
{
    char msg[256];
    int len = 0;
    while (1)
    {
        memset(msg, 0, sizeof(msg));
        len = rt_mq_recv(relay_mq, msg, sizeof(msg), RT_WAITING_FOREVER);
        cJSON *json = cJSON_Parse(msg);
        cJSON *method = cJSON_GetObjectItem(json, "method");
        cJSON *params = cJSON_GetObjectItem(json, "params");
        if(rt_strcmp(method->valuestring, "output_1") == 0)
        {
            if(params->valueint == RT_TRUE)
            {
                rt_pin_write(UOT1_PIN, PIN_HIGH);
                rt_kprintf("output_1 is ON\r\n");
            }
            else
            {
                rt_pin_write(UOT1_PIN, PIN_LOW);
            }

        }
        if(rt_strcmp(method->valuestring, "output_2") == 0)
        {
            if(params->valueint == RT_TRUE)
            {
                rt_pin_write(UOT2_PIN, PIN_HIGH);
                rt_kprintf("output_2 is ON\r\n");
            }
            else
            {
                rt_pin_write(UOT2_PIN, PIN_LOW);
            }

        }
        if(rt_strcmp(method->valuestring, "output_3") == 0)
        {
            if(params->valueint == RT_TRUE)
            {
                rt_pin_write(UOT3_PIN, PIN_HIGH);
                rt_kprintf("output_3 is ON\r\n");
            }
            else
            {
                rt_pin_write(UOT3_PIN, PIN_LOW);
            }

        }
    }
}


int relay_init(void)
{
    rt_pin_mode(UOT1_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(UOT2_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(UOT3_PIN, PIN_MODE_OUTPUT);

    relay_mq = rt_mq_create("relay_mq", sizeof(char) * 256, 10, RT_IPC_FLAG_FIFO);
    if (relay_mq == RT_NULL)
    {
        rt_kprintf("relay_mq create failed!\n");
        return -1;
    }
    relay_thread = rt_thread_create("relay_thread", relay_thread_entry, RT_NULL, 1024, 10, 10);
    if (relay_thread == RT_NULL)
    {
        rt_kprintf("relay_thread create failed!\n");
        return -1;
    }
    rt_thread_startup(relay_thread);
    return 0;
}