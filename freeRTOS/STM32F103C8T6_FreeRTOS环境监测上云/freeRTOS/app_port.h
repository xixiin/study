#ifndef __APP_PORT__H
#define __APP_PORT__H

/* ============================================================
 *  app_port.h : Driver interface layer for the FreeRTOS project.
 *  The RTOS layer (FreeRTOS_demo.c) only talks to these functions,
 *  so you can swap sensors / cloud platform without touching RTOS.
 * ============================================================ */

#include <stdint.h>

/* One frame of sampled sensor data. */
typedef struct
{
    float temp;    /* Celsius          */
    float humi;    /* %RH              */
    float smoke;   /* concentration / volts, depends on your ADC scale */
    uint8_t id;    /* sample sequence, for debugging   */
} Sample_t;

/* Event group bits (threshold alarm). */
#define EVENT_TEMP_HIGH   (1UL << 0)
#define EVENT_HUMI_HIGH   (1UL << 1)
#define EVENT_SMOKE_HIGH  (1UL << 2)
#define EVENT_ALL         (EVENT_TEMP_HIGH | EVENT_HUMI_HIGH | EVENT_SMOKE_HIGH)

/* ------------------------------------------------------------------
 *  Initialization (called from main before the scheduler starts)
 * ------------------------------------------------------------------ */
void APPPORT_Init(void);

/* ------------------------------------------------------------------
 *  Sensor: read temperature / humidity / smoke once.
 *  return 0 on success, -1 on read failure (e.g. DHT11 CRC error).
 * ------------------------------------------------------------------ */
int SENSOR_Read(Sample_t *s);

/* Display one sample on OLED; page selects which screen. */
void DISPLAY_Show(const Sample_t *s, uint8_t page);

/* Read one key press (0 = none, 1/2/3 = key codes you define). */
void KEY_Read(uint8_t *key);

/* Upload one sample to the cloud (MQTT / OneNet / ThingBoard). */
void CLOUD_Upload(const Sample_t *s);

/* Send a heartbeat packet (called from a software timer). */
void CLOUD_Heartbeat(void);

/* Turn alarm hardware on (1) or off (0): LED + buzzer. */
void ALARM_Set(int on);

#endif /* __APP_PORT__H */
