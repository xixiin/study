#include "FreeRTOS_demo.h"
#include "app_port.h"

#include <stdio.h>

/* ============================================================
 *  FreeRTOS_demo.c : multi-task environment monitor skeleton.
 *
 *  Tasks : Sensor / Display / Comm / Alarm / Key
 *  IPC    : 2x queue, 1x event group, 1x mutex, 1x software timer,
 *           1x task notification (ISR -> KeyTask)
 *
 *  Requires (see README): configUSE_TIMERS = 1 in FreeRTOSConfig.h
 * ============================================================ */

/*--------------------- threshold values (shared) ---------------------*/
float gTempTh  = 35.0f;
float gHumiTh  = 80.0f;
float gSmokeTh = 1.8f;

/*--------------------- kernel objects -------------------------------*/
static QueueHandle_t    xSampleQueue;   /* sensor -> display  */
static QueueHandle_t    xUploadQueue;   /* sensor -> comm     */
static SemaphoreHandle_t xOledMutex;    /* protects OLED write */
static EventGroupHandle_t xAlarmEventGroup;
static TimerHandle_t    xHeartTimer;    /* MQTT heartbeat     */

static TaskHandle_t xSensorHandle, xDisplayHandle, xCommHandle, xAlarmHandle, xKeyHandle;

/*--------------------- task prototypes ------------------------------*/
static void SensorTask(void *pv);
static void DisplayTask(void *pv);
static void CommTask(void *pv);
static void AlarmTask(void *pv);
static void KeyTask(void *pv);

/*--------------------- software timer callback -----------------------*/
static void vHeartTimerCallback(TimerHandle_t xTimer)
{
    (void)xTimer;
    CLOUD_Heartbeat();
}

/*--------------------- static memory providers -----------------------*/
/* configSUPPORT_STATIC_ALLOCATION = 1 -> FreeRTOS needs idle + timer
 * task storage even if app tasks use dynamic allocation.            */
static StaticTask_t xIdleTCB;
static StackType_t  xIdleStack[128];
static StaticTask_t xTimerTCB;
static StackType_t  xTimerStack[256];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxTCB, StackType_t **ppxStack, uint32_t *pulSize)
{
    *ppxTCB   = &xIdleTCB;
    *ppxStack = xIdleStack;
    *pulSize  = 128;
}

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTCB, StackType_t **ppxStack, uint32_t *pulSize)
{
    *ppxTCB   = &xTimerTCB;
    *ppxStack = xTimerStack;
    *pulSize  = 256;
}

/*--------------------- startup ---------------------------------------*/
void FreeRTOS_Start(void)
{
    /* 1) create IPC objects */
    xSampleQueue   = xQueueCreate(4, sizeof(Sample_t));
    xUploadQueue   = xQueueCreate(4, sizeof(Sample_t));
    xOledMutex     = xSemaphoreCreateMutex();
    xAlarmEventGroup = xEventGroupCreate();
    xHeartTimer    = xTimerCreate("Heart", pdMS_TO_TICKS(3000), pdTRUE, NULL, vHeartTimerCallback);

    /* 2) create tasks (priority 0..4, idle = 0) */
    xTaskCreate(SensorTask,  "Sensor",  256, NULL, 3, &xSensorHandle);
    xTaskCreate(DisplayTask, "Display", 256, NULL, 2, &xDisplayHandle);
    xTaskCreate(CommTask,    "Comm",    384, NULL, 1, &xCommHandle);
    xTaskCreate(AlarmTask,   "Alarm",   128, NULL, 2, &xAlarmHandle);
    xTaskCreate(KeyTask,     "Key",     128, NULL, 1, &xKeyHandle);

    if (xHeartTimer != NULL)
        xTimerStart(xHeartTimer, 0);

    /* 3) start scheduler (never returns on success) */
    vTaskStartScheduler();

    for (;;) { }   /* only reached if scheduler failed to start */
}

/*--------------------- Sensor task -----------------------------------*/
static void SensorTask(void *pv)
{
    (void)pv;
    TickType_t xLastWake = xTaskGetTickCount();
    Sample_t s = {0};

    for (;;)
    {
        vTaskDelayUntil(&xLastWake, pdMS_TO_TICKS(500));   /* fixed 500ms period */

        if (SENSOR_Read(&s) == 0)
        {
            /* set/clear alarm event bits based on thresholds */
            EventBits_t bits = 0;
            if (s.temp  > gTempTh)  bits |= EVENT_TEMP_HIGH;
            if (s.humi  > gHumiTh)  bits |= EVENT_HUMI_HIGH;
            if (s.smoke > gSmokeTh) bits |= EVENT_SMOKE_HIGH;

            if (bits != 0)
                xEventGroupSetBits(xAlarmEventGroup, bits);
            else
                xEventGroupClearBits(xAlarmEventGroup, EVENT_ALL);

            /* decouple: pass one sample to display and one to upload */
            xQueueSend(xSampleQueue, &s, 0);
            xQueueSend(xUploadQueue, &s, 0);
        }
    }
}

/*--------------------- Display task ----------------------------------*/
static void DisplayTask(void *pv)
{
    (void)pv;
    Sample_t s;

    for (;;)
    {
        if (xQueueReceive(xSampleQueue, &s, portMAX_DELAY) == pdTRUE)
        {
            if (xSemaphoreTake(xOledMutex, pdMS_TO_TICKS(100)) == pdTRUE)
            {
                DISPLAY_Show(&s, 0);               /* page 0; switch via Key task */
                xSemaphoreGive(xOledMutex);
            }
        }
    }
}

/*--------------------- Comm task (cloud upload) ----------------------*/
static void CommTask(void *pv)
{
    (void)pv;
    Sample_t s;

    for (;;)
    {
        if (xQueueReceive(xUploadQueue, &s, portMAX_DELAY) == pdTRUE)
        {
            CLOUD_Upload(&s);                      /* MQTT / OneNet publish */
        }
    }
}

/*--------------------- Alarm task ------------------------------------*/
static void AlarmTask(void *pv)
{
    (void)pv;
    for (;;)
    {
        /* wait up to 500ms for any alarm bit; clear it once consumed */
        EventBits_t bits = xEventGroupWaitBits(xAlarmEventGroup, EVENT_ALL,
                                               pdTRUE, pdFALSE, pdMS_TO_TICKS(500));
        if (bits != 0)
        {
            ALARM_Set(1);
            vTaskDelay(pdMS_TO_TICKS(150));
            ALARM_Set(0);
            vTaskDelay(pdMS_TO_TICKS(150));
        }
        else
        {
            ALARM_Set(0);
        }
    }
}

/*--------------------- Key task (task notification) ------------------*/
static void KeyTask(void *pv)
{
    (void)pv;
    uint8_t key = 0;

    for (;;)
    {
        /* wake only when the EXTI ISR notifies us */
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        KEY_Read(&key);
        if (key != 0)
        {
            if (xSemaphoreTake(xOledMutex, pdMS_TO_TICKS(100)) == pdTRUE)
            {
                /* adjust display page / thresholds as you like */
                if (key == 1) gTempTh += 1.0f;     /* example: raise temp limit */
                else if (key == 2) gTempTh -= 1.0f;
                xSemaphoreGive(xOledMutex);
            }
        }
    }
}

/*--------------------- ISR -> task notification ----------------------*/
void FreeRTOS_KeyISR(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    /* notify KeyTask from the key EXTI ISR */
    vTaskNotifyGiveFromISR(xKeyHandle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
