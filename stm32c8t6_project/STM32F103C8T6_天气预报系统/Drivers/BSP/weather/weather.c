#include "weather.h"
#include "esp8266.h"
#include "string.h"
#include "cJSON.h"
#include "stdio.h"
#include "delay.h"
#include "OLED.h"
#include "stdlib.h"

  
extern uint8_t weather_chinese[][32];        

char weatherdata[ESP8266_RX_BUF_SIZE] = {0};

typedef struct
{
    char date[20];      //日期
    char code_day[10];   //白天天气现象代码
    char high[10];       //最高温度
    char low[10];        //最低温度
} WeatherInfo;

WeatherInfo three_day_weather[3]={0};  //存储三天的天气信息

void get_3dayweather(void)
{
    uint8_t i;
    for(i = 0; i < 3; i++)
    {
        memset(weatherdata, 0, sizeof(weatherdata));
        esp8266_get_weather(i);
        esp8266_copy_rxdata(weatherdata);
        
        //1. 将JSON字符串转换成JSON结构体
        cJSON *cjson = cJSON_Parse((const char *)weatherdata);
        
        //2. 解析JSON各个字段
            //2.1 获取根对象里的results字段的值（数组1）
        cJSON *results = cJSON_GetObjectItem(cjson, "results");
            //2.2 获取数组1的第一个元素（对象1）
        cJSON *first_result = cJSON_GetArrayItem(results, 0);
            //2.3 获取对象1中的daily字段的值（数组2）
        cJSON *daily = cJSON_GetObjectItem(first_result, "daily");
            //2.4 获取数组2中的第一个元素（对象2）
        cJSON *first_daily = cJSON_GetArrayItem(daily, 0);
        
            //2.5 获取对象2中的data字段的值
        cJSON *date = cJSON_GetObjectItem(first_daily, "date");
        const char *date_str = date->valuestring;
        printf("date: %s\r\n", date_str);
        strcpy(three_day_weather[i].date, date_str);
        
            //2.6 获取对象2中的code_day字段的值
        cJSON *code_day = cJSON_GetObjectItem(first_daily, "code_day");
        const char *code_day_str = code_day->valuestring;
        printf("code_day: %s\r\n", code_day_str);
        strcpy(three_day_weather[i].code_day, code_day_str);
        
            //2.7 获取对象2中的high字段的值
        cJSON *high = cJSON_GetObjectItem(first_daily, "high");
        const char *high_str = high->valuestring;
        printf("high: %s\r\n", high_str);
        strcpy(three_day_weather[i].high, high_str);
        
            //2.8 获取对象2中的low字段的值
        cJSON *low = cJSON_GetObjectItem(first_daily, "low");
        const char *low_str = low->valuestring;
        printf("low: %s\r\n\r\n", low_str);
        strcpy(three_day_weather[i].low, low_str);
        
        //3. 清除JSON结构体
        cJSON_Delete(cjson);
        
        delay_ms(1000);
    }
}

void show_weather(uint8_t day)
{
    OLED_ShowString(1, 6, three_day_weather[day].date);
    switch (atoi(three_day_weather[day].code_day))
    {
        case 0:

        case 1:

        case 2:

        case 3:
            {
                OLED_ShowChinese(2, 1, 8, weather_chinese);
                break;
            }
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
         {
              OLED_ShowChinese(2, 1, 9,     weather_chinese);
              OLED_ShowChinese(2, 3, 10, weather_chinese);
            break;
           } 
        case 9:
           {
                OLED_ShowChinese(2, 1, 11, weather_chinese);    
                break;

           }
        case 10:
        case 11:
        case 12:
        case 13:    
        case 14:
        case 15:
        case 16:
        case 18:
            {
                OLED_ShowChinese(2, 1, 12, weather_chinese);
                break;
            }
        default:
            break;
    }
    OLED_ShowString(3, 12, three_day_weather[day].high);
    OLED_ShowChinese(3, 14, 7, weather_chinese);
    OLED_ShowString(4, 12, three_day_weather[day].low);
    OLED_ShowChinese(4, 14, 7, weather_chinese);
}
