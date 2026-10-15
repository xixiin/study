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

#define MQTT_HOST    "192.168.3.7"
#define MQTT_PORT    "1883"
#define MQTT_CLIENT_ID    "RTU_Study-Board"
#define MQTT_USER_NAME   "ZvfdQKIq4xBbOTR0MMKh"
#define MQTT_PASSWORD    ""

#define MQTT_SUB_TOPIC   "v1/devices/me/rpc/request/+"
#define MQTT_PUB_TOPIC   "v1/devices/me/telemetry"

static mqtt_client_t *client = NULL;

void mqtt_sub_handler(void* client, message_data_t* msg)
{
    rt_kprintf("topic: %s, msg: %s\r\n", msg->topic_name, msg->message->payload);
}

void mqtt_pub_msg(void *payload)
{
    mqtt_message_t msg;

    msg.qos = 0;
    msg.payload = payload;

    mqtt_publish(client, MQTT_PUB_TOPIC, &msg);
}

int mqtt_pub_test(void)
{
    static int count = 0;

    cJSON *root = cJSON_CreateObject();

    cJSON_AddNumberToObject(root, "count", count);
    char *msg = cJSON_Print(root);

    mqtt_pub_msg(msg);

    cJSON_Delete(root);
    cJSON_free(msg);

    count++;

    return 0;
}
MSH_CMD_EXPORT(mqtt_pub_test, mqtt publish test);

int mqtt_client_init(void)
{
    client = mqtt_lease();

    mqtt_set_host(client, MQTT_HOST);
    mqtt_set_port(client, MQTT_PORT);
    mqtt_set_client_id(client, MQTT_CLIENT_ID);
    mqtt_set_user_name(client, MQTT_USER_NAME);
    mqtt_set_password(client, MQTT_PASSWORD);

    rt_kprintf("mqtt start connect...\r\n");

    mqtt_connect(client);

    mqtt_subscribe(client, MQTT_SUB_TOPIC, 0, mqtt_sub_handler);

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
