#include "sys.h"
#include "uart1.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "OLED.h"
#include "RTC.h"
uint8_t time_data[6] = {26,6,26,16,11,10};
uint8_t alaem_data[3] = {16,11,10};
int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */                       
    uart1_init(115200);
    printf("running...\r\n");
	key_init();
	OLED_Init();
	OLED_Clear();
	rtc_init();
	uint8_t key_num=0,set_flag=0,shif_flag=1,slect=0,flag=0;
    while(1)
    { 
		key_num = key_scand();
		if(key_num==KEY_SET)
		{
			led_on();
			set_flag=!set_flag;
			if(set_flag)
			{
					slect = year_slect;
					flag  = OFF;
			}
			else 
			{
				shif_flag=1;
				slect=null_slect;
				flag=ON;
				rtc_set_time(time_data);
				rct_set_alaem(alaem_data);
			}
		
		}
		else if(key_num==KEY_SHIFT&&set_flag)
		{
			led_toggle();
			shif_flag++;
			if(shif_flag>alarm_second_slect)
				shif_flag=year_slect;
					switch(shif_flag)
						{
							case year_slect: 
							{
								slect = year_slect;
								flag  = OFF;
								break;
							}
							case mon_slect:
							{
								slect = mon_slect;
								flag  = OFF;
								break;
							}
							case day_slect: 
							{
								slect = day_slect;
								flag  = OFF;
								break;
							}
							case hour_slect:
							{
								slect = hour_slect;
								flag  = OFF;
								break;
								
							}
							case min_slect: 
							{
								slect = min_slect;
								flag  = OFF;
								break;
							}
							case second_slect:
							{
								slect = second_slect;
								flag  = OFF;
									break;
							}
							case alarm_hour_slect: 
							{				
								slect = alarm_hour_slect;
								flag  = OFF;
								break;
							}
							case alarm_min_slect:
							{
								slect = alarm_min_slect;
								flag  = OFF;
									break;
							}
							case alarm_second_slect: 
							{			
								slect = alarm_second_slect;
								flag  = OFF;
								break;
							}

						}
		
		}
		else if(key_num==KEY_UP&&set_flag)
		{
			switch(shif_flag)
			{
				case year_slect: 
				{
					time_data[0]++;
					if(time_data[0]>=100)
						time_data[0]=0;
						break;
				}
				case mon_slect:
				{
					time_data[1]++;
					if(time_data[1]>12)
						time_data[1]=1;
						break;
				}
				case day_slect: 
				{
					time_data[2]++;
					if(time_data[1]==2)
					{
						if(((time_data[0]+2000)%4==0 && (time_data[0]+2000)%100!=0) || ((time_data[0]+2000)%400==0))
						{
							if(time_data[2]>29)time_data[2]=1;break;
						}
						else 
						{
							if(time_data[2]>28)time_data[2]=1;break;
						}
					}
					else 
					{
						switch (time_data[1])
						{
							case 1: {if(time_data[2]>31)time_data[2]=1;break;}
							case 3: {if(time_data[2]>31)time_data[2]=1;break;}
							case 4: {if(time_data[2]>30)time_data[2]=1;break;}
							case 5: {if(time_data[2]>31)time_data[2]=1;break;}
							case 6: {if(time_data[2]>30)time_data[2]=1;break;}
							case 7: {if(time_data[2]>31)time_data[2]=1;break;}
							case 8: {if(time_data[2]>31)time_data[2]=1;break;}
							case 9: {if(time_data[2]>30)time_data[2]=1;break;}
							case 10: {if(time_data[2]>31)time_data[2]=1;break;}
							case 11: {if(time_data[2]>30)time_data[2]=1;break;}
							case 12: {if(time_data[2]>31)time_data[2]=1;break;}		
						}
							
					}

					
						break;
				}
				case hour_slect:
				{
					time_data[3]++;
					if(time_data[3]>23)
						time_data[3]=0;
						break;
				}
				case min_slect: 
				{

						time_data[4]++;
						if(time_data[4]>60)
							time_data[4]=0;					


							break;
				}
				case second_slect:
				{
					time_data[5]++;
						if(time_data[5]>60)
							time_data[5]=0;	
					break;
				}
				case alarm_hour_slect: 
				{	
					alaem_data[0]++;
					if(alaem_data[0]>23)
						alaem_data[0]=0;	
					
						break;
				}
				case alarm_min_slect:
				{
						alaem_data[1]++;
						if(alaem_data[1]>60)
							alaem_data[1]=0;	
					break;
				}
				case alarm_second_slect: 
				{	
						alaem_data[2]++;
						if(alaem_data[2]>60)
							alaem_data[2]=0;	
					
						break;
				}

			}
		}
		else if(key_num==KEY_DOWN&&set_flag)
		{
			switch(shif_flag)
			{
				case year_slect: 
				{
					time_data[0]--;
					if(time_data[0]==255)
						time_data[0]=99;
						break;
				}
				case mon_slect:
				{
					time_data[1]--;
					if(time_data[1]<1)
						time_data[1]=12;
						break;
				}
				case day_slect: 
				{
					time_data[2]--;
					if(time_data[1]==2)
					{
						if(((time_data[0]+2000)%4==0 && (time_data[0]+2000)%100!=0) || ((time_data[0]+2000)%400==0))
						{
							if(time_data[2]<1)time_data[2]=29;break;
						}
						else 
						{
							if(time_data[2]<1)time_data[2]=28;break;
						}
					}
					else 
					{
						switch (time_data[1])
						{
							case 1: {if(time_data[2]<1)time_data[2]=31;break;}
							case 3: {if(time_data[2]<1)time_data[2]=31;break;}
							case 4: {if(time_data[2]<1)time_data[2]=30;break;}
							case 5: {if(time_data[2]<1)time_data[2]=31;break;}
							case 6: {if(time_data[2]<1)time_data[2]=30;break;}
							case 7: {if(time_data[2]<1)time_data[2]=31;break;}
							case 8: {if(time_data[2]<1)time_data[2]=31;break;}
							case 9: {if(time_data[2]<1)time_data[2]=30;break;}
							case 10: {if(time_data[2]<1)time_data[2]=31;break;}
							case 11: {if(time_data[2]<1)time_data[2]=30;break;}
							case 12: {if(time_data[2]<1)time_data[2]=31;break;}		
						}
							
					}

					
						break;
				}
				case hour_slect:
				{
					time_data[3]--;
					if(time_data[3]==255)
						time_data[3]=23;
						break;
				}
				case min_slect: 
				{

						time_data[4]--;
						if(time_data[4]==255)
							time_data[4]=60;					


							break;
				}
				case second_slect:
				{
					time_data[5]--;
						if(time_data[5]==255)
							time_data[5]=60;	
					break;
				}
				case alarm_hour_slect: 
				{	
					alaem_data[0]--;
					if(alaem_data[0]==255)
						alaem_data[0]=23;	
					
						break;
				}
				case alarm_min_slect:
				{
						alaem_data[1]--;
						if(alaem_data[1]==255)
							alaem_data[1]=60;	
					break;
				}
				case alarm_second_slect: 
				{	
						alaem_data[2]--;
						if(alaem_data[2]==255)
							alaem_data[2]=60;	
					
						break;
				}

			}
		}
		oled_show();
		if(!set_flag)
		{
			rtc_get_time(time_data);
			rtc_get_alarm(alaem_data);
		}


		show_element(time_data,alaem_data,slect,flag);
		
    }
}

