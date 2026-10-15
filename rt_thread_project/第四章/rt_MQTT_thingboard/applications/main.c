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
#include "mqttclient.h"

#include "netdev_ipaddr.h"
#include "netdev.h"

#include "mqttclient.h"
#include <cJSON.h>

#define MQTT_HOST_IP       "10.163.224.207"
#define MQTT_HOST_PORT     "1883"
#define MQTT_CLIENT_ID     "RTU_Study_BOARD"
#define MQTT_PASSWORD        ""
#define MQTT_USER_NAME       "z6af2ZXIAnkZmUkctR5J"

#define MQTT_SUB_TOPIC        "v1/devices/me/rpc/request/+"
#define MQTT_PUB_TOPIC        "v1/devices/me/telemetry"

static mqtt_client_t *client = RT_NULL;
void MQTT_SUB_handle(void* client, message_data_t* msg)
{
    rt_kprintf("MQTT_SUB_handle: topic=%s, payload=%s\r\n", msg->topic_name, (char*)msg->message->payload);
}

void mqtt_pub(void *msg)
{
    mqtt_message_t message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payloadlen = strlen(msg);
    message.payload = msg;

    mqtt_publish(client, MQTT_PUB_TOPIC, &message);
}

int mqtt_pub_test(void)
{
    static int temp = 25;
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "temp", temp);
    char *msg = cJSON_Print(root);
    mqtt_pub(msg);
    cJSON_Delete(root);
    temp++;
    return 0;
}

MSH_CMD_EXPORT(mqtt_pub_test, mqtt pub test);

int mqtt_client_init(void)
{
   client = mqtt_lease();
   mqtt_set_host(client, MQTT_HOST_IP);
    mqtt_set_port(client, MQTT_HOST_PORT);
    mqtt_set_client_id(client, MQTT_CLIENT_ID);
    mqtt_set_user_name(client, MQTT_USER_NAME);
    mqtt_set_password(client, MQTT_PASSWORD);

    rt_kprintf("mqtt connect to %s:%s\r\n", MQTT_HOST_IP, MQTT_HOST_PORT);

    mqtt_connect(client);

    mqtt_subscribe(client, MQTT_SUB_TOPIC, 0,MQTT_SUB_handle);

    return 0;
}


static rt_bool_t is_conncected(void)
{
    struct netdev *dev = RT_NULL;
    dev = netdev_get_first_by_flags(NETDEV_FLAG_INTERNET_UP);
    if (dev == RT_NULL)
    {
        rt_kprintf("wait netdev internet up...\r\n");
        return RT_FALSE;
    }
    else
    {
        printf("local ip is:%d.%d.%d.%d\r\n", (((dev->ip_addr.addr) >> 0) & 0xFF), (((dev->ip_addr.addr) >> 8) & 0xFF),
               (((dev->ip_addr.addr) >> 16) & 0xFF), (((dev->ip_addr.addr) >> 24) & 0xFF));
        return RT_TRUE;
    }
}

int main(void)
{
    int count = 1;

    while (count++)
    {
        if (is_conncected())
        {
            mqtt_client_init();

            return 0;
        }
        rt_thread_mdelay(500);
    }

    return RT_EOK;
}



