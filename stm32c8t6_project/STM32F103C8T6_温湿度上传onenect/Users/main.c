#include "sys.h"
#include "uart1.h"
#include "delay.h"
#include "onenet.h"
#include "esp8266.h"
#include "OLED.h"
#include "dht11.h"
#include "string.h"

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */                       
    uart1_init(115200);
    printf("running...\r\n");
	esp8266_init(115200);
    printf("MQTT初始化...\r\n");
    mqtt_init();
    printf("MQTT连接服务器...\r\n");
    if(mqtt_connect(MQTT_ClientID,MQTT_UserName,MQTT_PassWord) == 0)
        printf("MQTT连接服务器成功\r\n");
    else
        printf("MQTT连接服务器失败\r\n");
    uint8_t dht11_data[4],data_send[512];
    while(1)
    { 
        memset(dht11_data, 0, 4);
        dht11_read(dht11_data);
        sprintf((char *)data_send, "{\"id\":\"123\",\"version\":\"1.0\",\"params\":{\"CurrentHumidity\":{\"value\":%d.%d},\"CurrentTemperature\":{\"value\":%d.%d}}}", 
                        dht11_data[2], dht11_data[3], dht11_data[0], dht11_data[1]);
        mqtt_publish_data(POST_TOPIC, (char *)data_send, 0);
        delay_ms(3000);
        
        printf("------------发送心跳包------------\r\n");
        mqtt_send_heart();
        printf("------------发送完成------------\r\n");
        
    }
}

