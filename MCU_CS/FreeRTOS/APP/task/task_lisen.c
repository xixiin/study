#include "task_lisen.h"

TaskHandle_t Task_lisen;
void Task_Lisen(void *arg);


void lisen_task(void)
{
	
	xTaskCreate(Task_Lisen, (const char *)"Task_Start", 128, NULL, 3, &Task_lisen);
	
}

void Task_Lisen(void *arg)
{
	myprintf("b\r\n");
//	printf("task_lisen:%s\r\n",__FILE__);
	while(1)
	{
		vTaskDelay(200);
	}
}
