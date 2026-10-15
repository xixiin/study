#include "curr_vol.h"
#include "warm.h"

// 电压值计算公式：((Vin/2)/3.3)*4096=adc
// 电流值计算公式：((Iin *120)/3.3)4096=adc
// 我们采集到ADC之后，需要通过公式转换为对应的电压电流值。
// 创建一个线程，定时采集ADC值，然后打包对应的电压电流值的JSON格式的数据包，通过MQTT发布到服务器上。

static struct rt_adc_device *adc_dev = RT_NULL;

static rt_thread_t curr_vol_thread = RT_NULL;

void curr_vol_thread_entry(void *parameter)
{
    while(1)
    {
        int adc_value1 = rt_adc_read(adc_dev, 4);
        int adc_value2 = rt_adc_read(adc_dev, 5);

        float voltage = ((float)adc_value1 *3300* 2) / 4096.0 / 1000.0;
        float current = ((float)adc_value2 * 3300)/4096.0 /120.0;
            rt_kprintf("adc_value1: %d, voltage: %d mV, adc_value2: %d, current: %d mA\r\n",
                       adc_value1, (int)(voltage * 1000.0f), adc_value2, (int)(current * 1000.0f));
        if(voltage > 6.0f)
            warm_set(VOLTAGE_WARM);
        if(current > 12.0f)
            warm_set(CURRENT_WARM);
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, "adc_vol", voltage);
        cJSON_AddNumberToObject(root, "adc_curr", current);

        char *json_str = cJSON_PrintUnformatted(root);
        extern void mqtt_pub(void *msg);
        mqtt_pub(json_str);

        cJSON_Delete(root);
        rt_free(json_str);
        
        rt_thread_mdelay(2000);
    }
}

int curr_vol_init(void)
{
    curr_vol_thread = rt_thread_create("curr_vol_thread",
                                    curr_vol_thread_entry,
                                    RT_NULL,
                                    1024,
                                    20,
                                    10);
    adc_dev = (struct rt_adc_device *)rt_device_find("adc0");
    if(adc_dev == RT_NULL)
    {
        rt_kprintf("adc device not found!\n");
        return -1;
    }
    rt_adc_enable(adc_dev, 4);
    rt_adc_enable(adc_dev, 5);
    rt_thread_startup(curr_vol_thread);
    return RT_EOK;
}