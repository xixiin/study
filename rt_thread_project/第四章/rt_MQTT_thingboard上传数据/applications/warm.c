#include "warm.h"

// 创建一个事件集，用于发送警告事件状态
// 创建一个线程，接收警告事件，然后统计警告次数，然后打包对应的警告值的JSON格式的数据包，通过MQTT发布到服务器上。

rt_event_t warm_event = RT_NULL;
static rt_thread_t warm_thread = RT_NULL;

void warm_set(rt_uint32_t state)
{
    rt_event_send(warm_event, state);
}

uint32_t temp_warm_count = 0;
uint32_t humi_warm_count = 0;
uint32_t curr_warm_count = 0;
uint32_t vol_warm_count = 0;

static void warm_thread_entry(void *parameter)
{
    while(1)
    {
        rt_uint32_t state;
        rt_event_recv(warm_event, 0xffffffff,
                      RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, &state);
        cJSON *root = cJSON_CreateObject();

        if(state & TEMP_WARM)
        {
            temp_warm_count++;
            cJSON_AddNumberToObject(root, "temp_warm", temp_warm_count);
        }


        if(state & HUMI_WARM)
        {
            humi_warm_count++;
            cJSON_AddNumberToObject(root, "humi_warm", humi_warm_count);
        }


        if(state & CURRENT_WARM)
        {
            curr_warm_count++;
            cJSON_AddNumberToObject(root, "curr_warm", curr_warm_count);
        }


        if(state & VOLTAGE_WARM)
        {
            vol_warm_count++;
            cJSON_AddNumberToObject(root, "vol_warm", vol_warm_count);
        }

        // 将JSON数据包转换为字符串并发布到MQTT服务器
        char *json_string = cJSON_Print(root);
        extern void mqtt_pub(void *msg);
        mqtt_pub(json_string);
        rt_free(json_string);

        cJSON_Delete(root);
    }
}

int warm_init(void)
{
    warm_event = rt_event_create("warm_event", RT_IPC_FLAG_FIFO);
    if (warm_event == RT_NULL)
    {
        return RT_ERROR;
    }
    warm_thread = rt_thread_create("warm_thread", warm_thread_entry, RT_NULL, 1024, 10, 10);
    rt_thread_startup(warm_thread);
     cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "temp_warm", temp_warm_count);
    cJSON_AddNumberToObject(root, "humi_warm", humi_warm_count);
    cJSON_AddNumberToObject(root, "curr_warm", curr_warm_count);
    cJSON_AddNumberToObject(root, "vol_warm", vol_warm_count);
    // 将JSON数据包转换为字符串并发布到MQTT服务器
    char *json_string = cJSON_Print(root);
    extern void mqtt_pub(void *msg);
    mqtt_pub(json_string);
    rt_free(json_string);

    cJSON_Delete(root);
    return 0;
}
