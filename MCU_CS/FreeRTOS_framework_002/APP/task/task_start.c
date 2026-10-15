#include "task_start.h"

TaskHandle_t Task_start;
void Task_Start(void *arg);


void creat_task(void)
{
	
	xTaskCreate(Task_Start, (const char *)"Task_Start", 512, NULL, 3, &Task_start);

}

void Task_Start(void *arg)
{
	cslock_init();
	myprintf("a");
	host_task();
	lisen_task();
	ctrl_task();
//	printf("task_start:%s\r\n",__FILE__);
	
	//设置GPIO模式	M2(26) B12(3)
	gpio ->swm_pin_mode(3,PIN_MODE_OUTPUT);
	gpio ->swm_pin_mode(26,PIN_MODE_OUTPUT);
	while(1)
	{
//		uint32_t tick = xTaskGetTickCount();
//		printf("tick = %d\r\n",tick);
		gpio->swm_pin_write(3,PIN_HIGH);
		gpio->swm_pin_write(26,PIN_HIGH);
		vTaskDelay(1000);
		
		gpio->swm_pin_write(3,PIN_LOW);
		gpio->swm_pin_write(26,PIN_LOW);
		vTaskDelay(1000);
	}
}

/*
(1)创建任务的方法
//静态创建句柄
static TaskHandle_t Task_start;
//实现任务的入口
void Task_Start(void *arg)
{
	while(1);
}
//创建任务
xTaskCreate(Task_Start, (const char *)"Task_Start", 512, NULL, 3, &Task_start);
(2)获取系统滴答计数器的值
uint32_t tick = xTaskGetTickCount();
(3)任务延时函数
vTaskDelay();数值为毫秒ms
(4)tick转ms
tick = ms*configTICK_RATE_HZ/1000.0
pdMS_TO_TICKS
(5)构建互斥量/锁
	解决多任务打印紊乱的问题
	
*/
static SemaphoreHandle_t cslock;
static int flag_init = 0;

void cslock_init(void)
{
	if(flag_init == 1) return;
	cslock = xSemaphoreCreateMutex();
	if(cslock == NULL)
	{
		printf("err%s\r\n",__func__);
	}
	flag_init = 1;
}

void cslock_get(void)
{
	if(flag_init == 0) return;
	int ret=xSemaphoreTake(cslock,100);
	if(ret == pdTRUE)
	{//成功获取
		
	}
	else
	{//失败
		
	
	}
}

void cslock_free(void)
{
	if(flag_init == 0) return;
	xSemaphoreGive(cslock);
}

void myprintf( char* format,...)
{
	char buf[256];
	
	va_list v_args;
	va_start(v_args,format);
	vsnprintf(buf,sizeof(buf),format,v_args);
	
	va_end(v_args);
	
	cslock_get();//获取锁
	printf("%s",buf);
	cslock_free();//释放锁
}
