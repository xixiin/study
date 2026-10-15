#include "led.h"


// 创建一个事件集，用于系统发送控制灯的状态
// 创建一个线程，接收灯状态事件，然后根据状态事件控制灯，然后打包对应引脚的JSON格式的数据包，通过MQTT发布到服务器上。

#define LED_B  GET_PIN(B, 13)
#define LED_G  GET_PIN(B, 14)
#define LED_R  GET_PIN(B, 15)

static rt_event_t led_event = RT_NULL;
struct rt_thread *led_thread = RT_NULL;

void state_led_set(rt_uint32_t state)
{
    rt_event_send(led_event, state);
}


void led_thread_entry(void *parameter)
{
    rt_uint32_t state;
    while(1)
    {
        rt_event_recv(led_event, LED_RED_ON | LED_GREEN_ON | LED_BLUE_ON | LED_RED_OFF | LED_GREEN_OFF | LED_BLUE_OFF,
                      RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, &state);
        cJSON *root = cJSON_CreateObject();

        if(state & LED_RED_ON)
        {
            rt_pin_write(LED_R, PIN_LOW);
            cJSON_AddTrueToObject(root, "1");
        }
        else if(state & LED_RED_OFF)
        {
            rt_pin_write(LED_R, PIN_HIGH);
            cJSON_AddFalseToObject(root, "1");
        }
        if(state & LED_GREEN_ON)
        {
            rt_pin_write(LED_G, PIN_LOW);
            cJSON_AddTrueToObject(root, "2");
        }
        else if(state & LED_GREEN_OFF)
        {
            rt_pin_write(LED_G, PIN_HIGH);
            cJSON_AddFalseToObject(root, "2");
        }
        if(state & LED_BLUE_ON)
        {
            rt_pin_write(LED_B, PIN_LOW);
            cJSON_AddTrueToObject(root, "3");
        }
        else if(state & LED_BLUE_OFF)
        {
            rt_pin_write(LED_B, PIN_HIGH);
            cJSON_AddFalseToObject(root, "3");
        }

        // 将JSON数据包发布到MQTT服务器
        char *json_string = cJSON_Print(root);
        extern void mqtt_pub(const void *msg);
        mqtt_pub(json_string);
        cJSON_Delete(root);
        cJSON_free(json_string);
    }
}


int led_init(void)
{
	    rt_pin_mode(LED_B, PIN_MODE_OUTPUT);
    rt_pin_mode(LED_G, PIN_MODE_OUTPUT);
    rt_pin_mode(LED_R, PIN_MODE_OUTPUT);
    rt_pin_write(LED_B, PIN_HIGH);
    rt_pin_write(LED_G, PIN_HIGH);
    rt_pin_write(LED_R, PIN_HIGH);
   


    led_event = rt_event_create("led_event", RT_IPC_FLAG_FIFO);
    if (led_event == RT_NULL)
    {
        return RT_ERROR;
    }
    led_thread = rt_thread_create("led_thread", led_thread_entry, RT_NULL, 1024, 10, 10);
    if (led_thread == RT_NULL)
    {
        return RT_ERROR;
    }
	  state_led_set(LED_RED_OFF);
    state_led_set(LED_GREEN_OFF);
    state_led_set(LED_BLUE_OFF);
	 rt_thread_startup(led_thread);

    return RT_EOK;
}