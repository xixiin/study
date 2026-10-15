#include "task_host.h"


TaskHandle_t Task_host;
void Task_Host(void *arg);


void host_task(void)
{
	
	xTaskCreate(Task_Host, (const char *)"Task_Host", 1024, NULL, 3, &Task_host);
	
}

void Task_Host(void *arg)
{
	myprintf("c\r\n");
	while(1)
	{
		vTaskDelay(500);
	}
}


