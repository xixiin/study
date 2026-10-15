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
	utc_t dt;
	dt.year = 2026;
	dt.month = 9;
	dt.day = 1;
	dt.hour = 16;
	dt.minute = 20;
	dt.second = 0;
	
	rtc->init( &dt );
	rtc->start();
	
	
	utc_t* nowdt = rtc->get_dt();
	
	rtc->read( nowdt );
		//utc转换为linux时间戳进行运算
	
	utc_t utc1;
	utc_t utc2;
	uint64_t timestamp = 0;
	
	//设置utc1
	utc1.year = 2026;
	utc1.month = 9;
	utc1.day = 1;
	utc1.hour = 16;
	utc1.minute = 30;
	utc1.second = 0;
	
	//将utc1转换为linux时间戳
	timestamp = utc_opt->utc_to_timestamp( &utc1 );
	
	//linux时间戳加上2天1小时1分1秒
	timestamp += ( 2*24*60*60 );			//加2天
	timestamp += (    1*60*60 );			//加1小时
	timestamp += (       1*60 );			//加1分钟
	timestamp += (          1 );			//加1秒
	
	//将修改后的时间戳，转换为utc时间->utc2
	utc_opt->timestamp_to_utc( timestamp,&utc2 );
	
	//原本  2026-09-01 16:30:00
	//后来  2026-09-03 17:31:01
	utc_opt->print_utc_time( &utc2 );
	//-----------------------------------------

	while(1)
	{
//		myprintf("%04d-%02d-%02d %02d:%02d:%02d \r\n",
//			nowdt->Year,nowdt->Month,nowdt->Date,
//			nowdt->Hour,nowdt->Minute,nowdt->Second);
		
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
