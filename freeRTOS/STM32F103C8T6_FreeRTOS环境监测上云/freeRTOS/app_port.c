#include "app_port.h"

/* ------------------------------------------------------------------
 *  app_port.c : thin wrapper over YOUR existing drivers.
 *  Adjust function names / includes to match the driver files you
 *  copied into this project (see README table). Each block is small.
 * ------------------------------------------------------------------ */

/* ---- include your existing drivers ---- */
#include "sys.h"
#include "delay.h"
#include "uart1.h"
#include "led.h"
#include "key.h"
#include "OLED.h"
#include "dht11.h"     /* from STM32F103C8T6_DHT11            */
#include "adc.h"       /* from STM32F103C8T6_烟雾报警系统       */
#include "esp8266.h"   /* WiFi module                         */
#include "onenet.h"    /* from STM32F103C8T6_温湿度上传onenect  */

/* Threshold values shared with the RTOS layer (see FreeRTOS_demo.c). */
extern float gTempTh;
extern float gHumiTh;
extern float gSmokeTh;

/*---------------------------------------------------------------------------
 * init : power on sensors / display / key / esp8266 + MQTT
 *---------------------------------------------------------------------------*/
void APPPORT_Init(void)
{
    led_init();
    OLED_Init();
    OLED_Clear();
    ADC_Init();              /* configure ADC channel used by smoke sensor */
    key_init();

    esp8266_init(115200);    /* if you do not want networking yet, comment out */
    /* mqtt_init();          -> mqtt_connect(MQTT_ClientID, MQTT_UserName, MQTT_PassWord); */
}

/*---------------------------------------------------------------------------
 * sensor : read DHT11 (temp/humi) + ADC (smoke concentration / volts)
 *---------------------------------------------------------------------------*/
int SENSOR_Read(Sample_t *s)
{
    uint8_t d[4];
    if (dht11_read(d) != 0)          /* 0 = read OK; adjust to your driver */
        return -1;

    /* Typical DHT11: data[0..1]=humidity (int*100? adjust scale),
       data[2..3]=temperature. Adjust to your real decode. */
    s->humi = (float)(d[0]) + (float)(d[1]) * 0.1f;
    s->temp = (float)(d[2]) + (float)(d[3]) * 0.1f;

    /* ADC 12-bit -> volts (3.3V reference). Matches your 烟雾报警系统. */
    s->smoke = (float)ADC_GetValue(ADC_CHANNEL_0) / 4096.0f * 3.3f;

    s->id++;
    return 0;
}

/*---------------------------------------------------------------------------
 * display : OLED show page 0 = temp, page 1 = humi, page 2 = smoke
 *---------------------------------------------------------------------------*/
void DISPLAY_Show(const Sample_t *s, uint8_t page)
{
    OLED_Clear();
    switch (page)
    {
        case 0:
            OLED_ShowString(1, 1, "Temp:");
            OLED_ShowNum(2, 1, (uint8_t)s->temp, 2);          /* integer part */
            OLED_ShowChar(2, 3, '.');
            OLED_ShowNum(2, 4, (uint8_t)(s->temp * 10) % 10, 1);
            break;
        case 1:
            OLED_ShowString(1, 1, "Humi:");
            OLED_ShowNum(2, 1, (uint8_t)s->humi, 2);
            break;
        case 2:
            OLED_ShowString(1, 1, "Smoke:");
            OLED_ShowNum(2, 1, (uint8_t)(s->smoke * 10), 3);  /* scale to fit */
            break;
        default:
            break;
    }
    OLED_ShowString(3, 1, "ID:");      /* show sample id for debugging */
    OLED_ShowNum(3, 4, s->id, 2);
}

/*---------------------------------------------------------------------------
 * key : return a key code (0 = none). Matches your key driver.
 *---------------------------------------------------------------------*/
void KEY_Read(uint8_t *key)
{
    *key = key_scand();        /* replace with your key scan function name */
}

/*---------------------------------------------------------------------------
 * cloud : pulse one MQTT publish (JSON) to the cloud platform.
 *---------------------------------------------------------------------------*/
void CLOUD_Upload(const Sample_t *s)
{
    char data[192];
    /* JSON body for OneNet; adjust field names as needed. */
    sprintf(data,
        "{\"id\":\"123\",\"version\":\"1.0\",\"params\":{"
        "\"Temperature\":{\"value\":%d.%d},"
        "\"Humidity\":{\"value\":%d.%d},"
        "\"Smoke\":{\"value\":%d.%d}}}",
        (int)s->temp, (int)(s->temp * 10) % 10,
        (int)s->humi, (int)(s->humi * 10) % 10,
        (int)(s->smoke * 10), (int)(s->smoke * 100) % 10);

    mqtt_publish_data(POST_TOPIC, data, 0);   /* from your onenet / mqtt wrapper */
    mqtt_send_heart();                        /* keep-alive */
}

/*---------------------------------------------------------------------------
 * heartbeat : software-timer callback body (MQTT keep-alive).
 *---------------------------------------------------------------------------*/
void CLOUD_Heartbeat(void)
{
    mqtt_send_heart();
}

/*---------------------------------------------------------------------------
 * alarm : turn on/off LED + buzzer.
 *---------------------------------------------------------------------------*/
void ALARM_Set(int on)
{
    if (on)
    {
        led_on();
        /* buzzer_on(); -- add your buzzer control */
    }
    else
    {
        led_off();
        /* buzzer_off(); */
    }
}
