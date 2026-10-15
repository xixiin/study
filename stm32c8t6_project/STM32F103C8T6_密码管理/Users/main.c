#include "sys.h"
#include "uart1.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "OLED.h"
#include "W25Q128.h"
#include "string.h"

uint16_t pwd_init=123,pwd,count1,count10,count100,pwd_in,count=0;
uint8_t receive_data[2] = {0},flag=1;

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */                       
    uart1_init(115200);
    printf("running...\r\n");
	key_init();
	OLED_Init();
	OLED_Clear();
	w25q128_init();
	uint16_t devie_id = w25q128_read_id();
	printf("device_id:%X\r\n",devie_id);

	w25q128_read_data(PWD_FLASH_ADDR,receive_data,2);
	if(receive_data[0]==0xFF&&receive_data[1]==0xFF)
	{
		w25q128_erase_sector(PWD_FLASH_ADDR);
		w25q128_write_3digit_pwd(pwd_init,PWD_FLASH_ADDR);
		pwd=w25q128_read_3digit_pwd(PWD_FLASH_ADDR);
	}
	else
	{
		pwd=w25q128_read_3digit_pwd(PWD_FLASH_ADDR);
	}

	OLED_ShowString(1,1,"password:");

	uint8_t key_value = 0;

    while(1)
    { 
		key_value = key_scand();
		if(key_value == 1)
		{
			if(flag==1)
			{	
				count1++;
				if(count1==10)
					count1=0;
			}

			else if(flag==2)
			{
				count10++;
				if(count1==10)
					count10=0;	
			}
			else if(flag==3)
			{
				count100++;
				if(count1==10)
					count100=0;			
			}
		}
		else if(key_value == 2)
		{		
			flag++;
			if(flag>3)
				flag=1;
				
		}	
		else if(key_value == 3)
		{		
			if(pwd_in==pwd)
			{
				OLED_ShowString(2,1,"on");
				count1=0;
				count10=0;
				count100=0;
				flag=1;
				pwd_in=0;
				delay_ms(2000);
				OLED_ShowString(2,1,"  ");
			}	
			else 
			{
				count++;

				
				if(count>2)
				{
					OLED_ShowString(2,1,"off");
						count1=0;
						count10=0;
						count100=0;
						count=0;
						flag=1;
						pwd_in=0;
						delay_ms(3000);
					OLED_ShowString(2,1,"   ");
				}
				else
				{
					OLED_ShowString(2,1,"countinue");
					delay_ms(1000);
				}
				OLED_ShowString(2,1,"         ");
			}
   		 }
		pwd_in=count1+count10*10+count100*100;
		OLED_ShowNum(1,11,pwd_in,3);

	}	
}

