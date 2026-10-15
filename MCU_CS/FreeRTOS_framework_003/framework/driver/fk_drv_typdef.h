



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
