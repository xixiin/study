#include "input.h"

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "cJSON.h"

// 输入干节点采用中断的形式监听引脚电平
// 如果引脚当前为低电平，则设置为上升沿中断触发，如果引脚为高电平，则设置为下降沿中断触发
// 在中断服务程序中判断引脚当前电平，然后通过消息队列发送引脚状态
// 创建一个线程，接收引脚状态的消息队列，然后打包对应的引脚状态的JSON格式的数据包，通过MQTT发布到服务器上。
#define INPUT_PIN1  GET_PIN(C, 3)
#define INPUT_PIN2  GET_PIN(C, 1)
#define INPUT_PIN3  GET_PIN(C, 2)

int input_pin1 = INPUT_PIN1;
int input_pin2 = INPUT_PIN2;
int input_pin3 = INPUT_PIN3;

static rt_mq_t input_mq = RT_NULL;
static rt_thread_t input_thread = RT_NULL;

typedef struct input_msg
{
    int pin;
    int level;
} input_msg_t;

typedef struct input_data
{
    char *index;
    int pin;
} input_data_t;

static input_data_t input_data[] = {
    {"4", INPUT_PIN1},
    {"5", INPUT_PIN2},
    {"6", INPUT_PIN3},
};

static void input_irq_handler(void *args)
{

    int pin = *(int *)args;

    int level = rt_pin_read(pin);
    if(level == PIN_HIGH)
    {
        rt_pin_mode(pin, PIN_MODE_INPUT_PULLUP);
    }
    else
    {
       rt_pin_mode(pin, PIN_MODE_INPUT_PULLDOWN);
    }
    input_msg_t msg;
    msg.pin = pin;
    msg.level = level;
    rt_mq_send(input_mq, &msg, sizeof(input_msg_t));

}

void input_thread_entry(void *parameter)
{
    input_msg_t msg;
    while(1)
    {
        
        rt_mq_recv(input_mq, &msg, sizeof(input_msg_t), RT_WAITING_FOREVER);
        // 打包JSON数据
        for(int i = 0; i < sizeof(input_data) / sizeof(input_data[0]); i++)
        {
            if (msg.pin == input_data[i].pin)
            {
                cJSON *root = cJSON_CreateObject();
                if(msg.level == PIN_HIGH)
                {
                    cJSON_AddTrueToObject(root, input_data[i].index);
                }
                else
                {
                    cJSON_AddFalseToObject(root, input_data[i].index);
                }
                char *msg_str = cJSON_PrintUnformatted(root);
                extern void mqtt_pub(const void *msg);
                mqtt_pub(msg_str);
                cJSON_Delete(root);
                rt_free(msg_str);
                break;
            }
        }

    }
}


int input_init(void)
{
    input_mq = rt_mq_create("input_mq", sizeof(input_msg_t), 10, RT_IPC_FLAG_FIFO);
    input_thread = rt_thread_create("input_thread", input_thread_entry, RT_NULL, 1024, 10, 10);

    rt_thread_startup(input_thread);

    rt_pin_mode(INPUT_PIN1, PIN_MODE_INPUT);
    rt_pin_mode(INPUT_PIN2, PIN_MODE_INPUT);
    rt_pin_mode(INPUT_PIN3, PIN_MODE_INPUT);

    rt_pin_attach_irq(INPUT_PIN1, PIN_IRQ_MODE_RISING_FALLING, input_irq_handler, &input_pin1);
    rt_pin_attach_irq(INPUT_PIN2, PIN_IRQ_MODE_RISING_FALLING, input_irq_handler, &input_pin2);
    rt_pin_attach_irq(INPUT_PIN3, PIN_IRQ_MODE_RISING_FALLING, input_irq_handler, &input_pin3);

    rt_pin_irq_enable(INPUT_PIN1, PIN_IRQ_ENABLE);
    rt_pin_irq_enable(INPUT_PIN2, PIN_IRQ_ENABLE);
    rt_pin_irq_enable(INPUT_PIN3, PIN_IRQ_ENABLE);

    return 0;
}
