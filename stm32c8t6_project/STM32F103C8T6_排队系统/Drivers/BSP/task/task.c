#include "task.h"
#include "led.h"
#include "beep.h"
#include "alarm.h"
#include "exti.h"
#include "lcd1602.h"
#include "string.h"
#include "stdio.h"

enum 
{
	PASS_STATE,
	WAIT_STATE
};

uint32_t led1_cnt_led1 = 0;
uint32_t led2_cnt_led2 = 0;
uint32_t wait_cnt = 0;

uint8_t led1_task_flag = 0;
uint8_t led2_task_flag = 0;
uint8_t passenger = 0;

uint8_t state = PASS_STATE;
uint8_t pass_flag=0;
uint8_t wait_flag=0;

void systick_task(void)
{
	if(state==PASS_STATE)
	{

		if(led1_cnt_led1<1000)
			led1_cnt_led1++;
		else 
		{
			led1_cnt_led1=0;
			led1_task_flag=1;
		}
		led_off1();
		beep_off();
		alarm_off();
		
	}
	else if(state==WAIT_STATE)
	{
		if(led2_cnt_led2<200)
			led2_cnt_led2++;
		else 
		{
			led2_cnt_led2=0;
			led2_task_flag=1;
		}
			led_off();
			beep_on();
			alarm_on();
		if(wait_cnt<3000)
			wait_cnt++;
		else 
		{
			wait_cnt=0;
			state=PASS_STATE;
			lcd_showline(1,1,"PASS...");
		}
	}


}

void led1_task(void)
{
	if(led1_task_flag==0)
		return;
	
	led1_task_flag=0;
	led_toggle();
}

void led2_task(void)
{
	if(led2_task_flag==0)
		return;
	led2_task_flag=0;
	led_toggle1();
}
char temp[20]={0};
void sensor_take(void)
{
	//检测到人
	if(vibrate_flag_get()==TRUE&&state==PASS_STATE)
	{
		//cnt加一
		passenger++;
		//lcd显示
		sprintf(temp,"PASS...%02d/05",passenger);
		lcd_showline(1,1,temp);
	}
	if(passenger>=5)
	{
	//人数>=5
	//cnt=0
		passenger=0;
	//进入不允许通入
		state=WAIT_STATE;
		sprintf(temp,"WAIT...00/05");
		lcd_showline(1,1,temp);
	}
	

}
