#include "task_ctrl.h"

TaskHandle_t Task_ctrl;
void Task_Ctrl(void *arg);


void ctrl_task(void)
{
	
	xTaskCreate(Task_Ctrl, (const char *)"Task_Ctrl", 512, NULL, 3, &Task_ctrl);
	
}

void Task_Ctrl(void *arg)
{
	myprintf("d\r\n");
	while(1)
	{
		vTaskDelay(500);
	}
}
