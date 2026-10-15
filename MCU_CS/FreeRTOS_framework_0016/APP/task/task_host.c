#include "task_host.h"
#include "comdline.h"

#include "led_board.h"

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
		led_board_opt->work(200);
		cmd_line_work();

		vTaskDelay( pdMS_TO_TICKS( 5 ) );
	}
}


