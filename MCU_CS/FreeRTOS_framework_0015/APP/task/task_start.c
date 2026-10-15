#include "task_start.h"
#include "led_board.h"


TaskHandle_t Task_start;
void Task_Start(void *arg);


void creat_task(void)
{
	
	xTaskCreate(Task_Start, (const char *)"Task_Start", 1024, NULL, 3, &Task_start);

}



void Task_Start(void *arg)
{
	//初始化uart0
	uart->close( _e_uart0 );
	uart->uart_init( _e_uart0,115200 );
	uart->open( _e_uart0 );

	//延时1s
	vTaskDelay( pdMS_TO_TICKS( 1000 ) );

	//初始化打印锁
	cslock_init();

	//加载工作数据
	work_data_load();
	


	led_board_opt->init();
	led_board_opt->set_net(__ON);
	led_board_opt->set_fuse(__ON);
	
	led_board_opt->set_sock(0,1);
	led_board_opt->set_sock(1,1);
	//锁初始化、数据加载完成后再创建其他任务，避免打印未上锁
	host_task();
	lisen_task();
	ctrl_task();

	while(1)
	{
//		network_entry();
		vTaskDelay( pdMS_TO_TICKS( 10 ) );
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
xTaskCreate(Task_Start, (const char *)"Task_Start", 1024, NULL, 3, &Task_start);
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
		myprintf("err%s\r\n",__func__);
	}
	flag_init = 1;
}

void cslock_get(void)
{
	if(flag_init == 0) return;
	int ret=xSemaphoreTake(cslock,0xFFFFFFFF);
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