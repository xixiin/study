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
	//Flash编程
	//SWM320RET7 512KB
	//SECTOR 大小为 4KB		,每次擦除操作，尺寸为4KB
	//FLASH 位宽为 128bit ,每次编程操作，尺寸为16B，必须保证 4 个 WORD（128 位）对齐
	//void FLASH_Erase(uint32_t addr);
	//int FLASH_Write(uint32_t addr, uint32_t buff[], uint32_t count);
	
	//flash的特性:读/写/擦除
	//读:在swm320里面，直接用指针读取就可以了,按字去读
	//写:称为编程操作，只能写0，不能写1。
	//擦除:擦除一个扇区，会把扇区里面的数据，全部变成0xFF 
	
	//读取 _f_conf_start 的数据4*4 = 16 (对齐到4字节) 
	uint8_t buf[16];
	
	uint32_t* dst = ( uint32_t* )buf;
	volatile uint32_t* src = ( uint32_t* )(_f_conf_start);
	uint32_t len = sizeof( buf )/4;	//复制多少个字
	
	int i = 0;
	for( i=0;i<len;i++ )
	{
		*dst = *(src+i);
		dst++;
	}
	for( i=0;i<sizeof( buf );i++ )
	{
		myprintf("buf[%d] = %c \r\n",i,buf[i] );
	}
	
	//修改数据
	for( i=0;i<sizeof( buf );i++ )
	{
		buf[i] = 'a'+i;
	}
	
	//擦除这一个扇区 4096 B
	FLASH_Erase( _f_conf_start );
	//重新写回flash
	FLASH_Write( _f_conf_start,(uint32_t*)buf,len );	

	while(1)
	{
		
		
		vTaskDelay(10);
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
		myprintf("err%s\r\n",__func__);
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
