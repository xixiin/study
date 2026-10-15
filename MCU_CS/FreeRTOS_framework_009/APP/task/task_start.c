#include "task_start.h"



TaskHandle_t Task_start;
void Task_Start(void *arg);


void creat_task(void)
{
	
	xTaskCreate(Task_Start, (const char *)"Task_Start", 512, NULL, 3, &Task_start);

}

//串口0(调试口)收到的数据，通过串口1发送出去
void swap_rx0_to_tx1(void)
{
	uint8_t buf[8];
	int ret = uart->read( _e_uart0,buf,sizeof( buf ) );
	if( ret == 0 ) return;
	
	for( int i=0;i<ret;i++ )
		uart->write( _e_uart1,buf[i] );
}

//串口1收到的数据，通过串口0发送出去。
void swap_rx1_to_tx0(void)
{
	uint8_t buf[8];
	int ret = uart->read( _e_uart1,buf,sizeof( buf ) );
	if( ret == 0 ) return;
	
	for( int i=0;i<ret;i++ )
		uart->write( _e_uart0,buf[i] );
}

#define NetRST 56
void Task_Start(void *arg)
{

//	work_data_load();
	host_task();
	lisen_task();
	ctrl_task();
	
		uart->close( _e_uart0 );
	uart->uart_init( _e_uart0,115200 );
	uart->open( _e_uart0 );
	uart->irq_enable( _e_uart0,1 );
	cslock_init();
	//调试4G模块
	
	//4G模块的串口 1
	uart->uart_init( _e_uart1,115200 );
	uart->open( _e_uart1 );
	uart->irq_enable( _e_uart1,1 );

	//初始化4G模块的复位引脚,并复位4G模块	
	gpio->swm_pin_mode(NetRST,PIN_MODE_OUTPUT);
	gpio->swm_pin_write(NetRST,0);
	vTaskDelay(pdMS_TO_TICKS( 100 ));
	gpio->swm_pin_write(NetRST,1);
	vTaskDelay(pdMS_TO_TICKS( 100 ));
	gpio->swm_pin_write(NetRST,0);
	//实验之前，需要先接通220V，
	//因为4G模块的5V供电，依赖于220V转12V 转5V。
	//调试器只能提供3.3V，无法驱动4G模块
	
/*
AT

OK
------
AT+RESET
OK
^boot.rom.'v....'!\n
RDY

^MODE: 17,17

+E_UTRAN Service

+CGEV: ME PDN ACT 1

------
读取模块厂家信息
AT+CGMI

+CGMI: "AirM2M"

OK

------
读取模块的固件信息
AT+CGMR

+CGMR: "AirM2M_780EP_V1010_LTE_AT"

OK
------
查询物联网卡/SIM卡有没有插好
AT+CPIN?

+CPIN: READY

OK
------
查询信号质量
AT+CSQ

+CSQ: 19,0

OK
------
查询网络的注册状态
AT+CREG?

+CREG: 0,1

OK
------
查询附着GPRS网络
AT+CGATT?

+CGATT: 1

OK

------
设置透明传输
AT+CIPMODE=1


OK
------
设置模块为单链接
AT+CIPMUX=0

OK
------
自动获取APN

AT+CSTT="","",""

OK
------
激活移动场景,激活之后，可以去获取IP
AT+CIICR

OK
------
查询IP
AT+CIFSR

10.27.11.103
------
连接TCP服务器
AT+CIPSTART="TCP","www.armsoc.cn","9002"

AT+CIPSTART="TCP","www.armsoc.cn","9002"

OK

CONNECT




*/
		

	while(1)
	{
		
		swap_rx0_to_tx1();
		swap_rx1_to_tx0();
		
		vTaskDelay( 10 );
		
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
xTaskCreate(Task_Start, (const char *)"Task_Start", 512, NULL, 3, &Task_start);
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
	int ret=xSemaphoreTake(cslock,100);
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
