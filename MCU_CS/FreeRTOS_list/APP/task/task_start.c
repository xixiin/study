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
	
	//链表示例
	List_t mylist;  //链表根节点
	ListItem_t node0;
	ListItem_t node1;
	ListItem_t node2;
	//1创建一个链表根节点
	vListInitialise(&mylist);
	//2n个节点的初始化
	vListInitialiseItem(&node0);
	vListInitialiseItem(&node1);
	vListInitialiseItem(&node2);
	node0.xItemValue = 3;
	node1.xItemValue = 5;
	node2.xItemValue = 2;
	
	
	//3把这些节点插入链表
	//升序插入
//	vListInsert(&mylist,&node0);
//	vListInsert(&mylist,&node1);
//	vListInsert(&mylist,&node2);
	//尾插入
	vListInsertEnd(&mylist,&node0);
	vListInsertEnd(&mylist,&node1);
	vListInsertEnd(&mylist,&node2);
	
	ListItem_t* node = listGET_HEAD_ENTRY(&mylist);
	while(1)
	{
		
		
		if(node == listGET_END_MARKER(&mylist)) break;
		myprintf("val = %08x\r\n",node->xItemValue);
		node = node->pxNext;
	}
	
	while(1)
	{
//		uint32_t tick = xTaskGetTickCount();
//		printf("tick = %d\r\n",tick);
		vTaskDelay(1000*configTICK_RATE_HZ/1000.0);
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
