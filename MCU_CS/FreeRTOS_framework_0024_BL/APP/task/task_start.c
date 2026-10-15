#include "task_start.h"



TaskHandle_t Task_start;
void Task_Start(void *arg);


void creat_task(void)
{
	
	xTaskCreate(Task_Start, (const char *)"Task_Start", 1024, NULL, 2, &Task_start);

}

static void peripheral_deinit(void)
{
	//关闭Bootloader中的FreeRTOS所使用的系统滴答
	SysTick->CTRL = 0;
	SysTick->VAL = 0;
	
	//串口关闭
	uart->close( _e_uart0 );
	NVIC_DisableIRQ( UART0_IRQn );
		

}

static void jump_to_app( uint32_t addr )
{
	__disable_irq();
	
	//获取栈顶指针和复位向量
	uint32_t sp = *((volatile  uint32_t*)(addr));
	uint32_t pc = *( (volatile uint32_t*)(addr +4) );

	typedef void (*func_void_void)(void);
	
	func_void_void ResetHandler = (func_void_void)pc;
	
	//设置向量表的偏移寄存器
	SCB->VTOR = addr;
	//设置主堆栈指针 MSP
	__set_MSP( sp );
	
	//调用新的app的复位向量
	ResetHandler();
	
	while(1) __NOP();	
	

}

static void ru_by_config(void)
{
	uint8_t boot_mode = _e_boot_mode_ota;
	if((wdat->storage->bootcode == BootCode) &&(wdat->storage->ota_req ==1))
	{
		boot_mode = _e_boot_mode_ota;
	
	}
	else
	{
		boot_mode = _e_boot_mode_app;
	}
	
	
	switch(boot_mode)
	{
		case _e_boot_mode_ota:
			myprintf("--->_e_boot_mode_ota\r\n");
		
			break;
		case _e_boot_mode_app:
			myprintf("--->_e_boot_mode_app\r\n");
		thread_delay_ms(100);	
		//关闭已开启的外设(尤其是产生中断的外设)
		peripheral_deinit();
		//跳转到app的地址
		jump_to_app(_f_app_start);
			break;
		
		default:
			break;
	
	
	
	}
}

void Task_Start(void *arg)
{
	//初始化uart0
	uart->close( _e_uart0 );
	uart->uart_init( _e_uart0,115200 );
	uart->open( _e_uart0 );

	//延时1s
	vTaskDelay( pdMS_TO_TICKS( 1000 ) );

	
	//初始化打印锁
	cslock_init();

	//加载工作数据
	work_data_init();
	ru_by_config();
	//继电器打开
	gpio->swm_pin_mode(IO_ELC0,PIN_MODE_OUTPUT);
	gpio->swm_pin_mode(IO_ELC1,PIN_MODE_OUTPUT);
	gpio->swm_pin_write(IO_ELC0,PIN_HIGH);
	gpio->swm_pin_write(IO_ELC1,PIN_HIGH);
	
	

	led_board_opt->init();
	led_board_opt->set_net(__OFF);
	led_board_opt->set_fuse(__ON);
	utc_t utc={ 2026,1,1,0,0,0 };
	rtc->stop( );
	rtc->init( &utc );
	rtc->start();

	//锁初始化、数据加载完成后再创建其他任务，避免打印未上锁
	host_task();
	lisen_task();
	ctrl_task();
	

	

	while(1)
	{
		network_entry();

		vTaskDelay( pdMS_TO_TICKS( 1 ) );
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
xTaskCreate(Task_Start, (const char *)"Task_Start", 1024, NULL, 3, &Task_start);
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
		myprintf("err%s\r\n",__func__);
	}
	flag_init = 1;
}

void cslock_get(void)
{
	if(flag_init == 0) return;
	int ret=xSemaphoreTake(cslock,0xFFFFFFFF);
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
