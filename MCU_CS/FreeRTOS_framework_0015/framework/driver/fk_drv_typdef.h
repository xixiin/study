



typedef struct{
	//设置引脚模式
	void (*swm_pin_mode)(int pin, int mode);
	//写入引脚状态
	void (*swm_pin_write)( int pin, int value);
	//读引脚状态
	int (*swm_pin_read)( int pin);
	//给一个引脚加外部中断
	int (*swm_pin_attach_irq)(int pin,int mode,pin_callback_t cb,void *args);
	//给一个引脚移除中断
	int (*swm_pin_detach_irq)( int pin);
	//使能中断
	int (*swm_pin_irq_enable)(int pin,int enabled);

}drv_gpio_t;

extern drv_gpio_t* gpio;

typedef struct{
	//uart初始化
	void (*uart_init)(int index , uint32_t baudrate);
	//打开uart
	void (*open)( int index);
	//关闭uart
	int (*close)( int index);
	//写一个字
	int (*write)(int index,uint8_t dat);
	//使能中断
	int (*irq_enable)(int index , int enable);
	//读取数据
	int (*read)(int index ,void* buf , int len );
	//清除数据
	void (*clr_rxbuf)(int index);
}drv_uart_t;

extern drv_uart_t* uart;

/*-------------------------------------------------------------------------*/
//RTC
typedef struct{
	void (*init)( utc_t* dt );				//初始化函数
	void (*start)(void);								//启动RTC
	void (*stop)(void);									//停止RTC
	utc_t* (*get_dt)(void);					//获取rtc_dt变量
	int (*read)( utc_t* dt );				//从硬件读取rtc_dt

}drv_rtc_t;

extern drv_rtc_t* rtc;
/*-------------------------------------------------------------------------*/
//WDT
typedef struct{
	void (*init)( uint32_t tick );
	void (*feed)(void);
	void (*start)(void);
	void (*stop)(void);
}drv_wdt_t;

extern drv_wdt_t* wdt;
