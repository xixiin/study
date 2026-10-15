#include "task.h"
#include "led.h"

uint32_t cnt_led1 = 0;
uint32_t cnt_led2 = 0;

uint8_t task1_flag = 0;
uint8_t task2_flag = 0;

void systick_task(void)
{
	if(cnt_led1<1000)
		cnt_led1++;
	else 
	{
		cnt_led1=0;
		task1_flag=1;
	}
	if(cnt_led2<200)
		cnt_led2++;
	else 
	{
		cnt_led2=0;
		task2_flag=1;
	}
}

void task1(void)
{
	if(task1_flag==0)
		return;
	
	task1_flag=0;
	led_toggle0();
}

void task2(void)
{
	if(task2_flag==0)
		return;
	task2_flag=0;
	led_toggle1();
}
