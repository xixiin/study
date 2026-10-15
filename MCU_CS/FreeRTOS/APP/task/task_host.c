#include "task_host.h"
#include "stdarg.h"

TaskHandle_t Task_host;
void Task_Host(void *arg);


void host_task(void)
{
	
	xTaskCreate(Task_Host, (const char *)"Task_Start", 128, NULL, 3, &Task_host);
	
}

void Task_Host(void *arg)
{
	myprintf("c\r\n");
//	printf("task_host:%s\r\n",__FILE__);
	while(1)
	{
		vTaskDelay(200);
	}
}


