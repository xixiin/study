#include "framework.h"

extern drvp_gpio_t* drvp_gpio;

	//设置引脚模式
static	void swm_pin_mode(int pin, int mode)
{
	drvp_gpio->swm_pin_mode(pin,mode);
}
	//写入引脚状态
static	void swm_pin_write( int pin, int value)
{
	drvp_gpio->swm_pin_write(pin,value);
}
	//读引脚状态
static	int swm_pin_read(int pin)
{
	return drvp_gpio->swm_pin_read(pin);
}
	//给一个引脚加外部中断
static	int swm_pin_attach_irq(int pin,int mode,pin_callback_t cb,void *args)
{
	return drvp_gpio->swm_pin_attach_irq(pin,mode,cb,args);
}
	//给一个引脚移除中断
static	int swm_pin_detach_irq( int pin)
{
	return drvp_gpio->swm_pin_detach_irq(pin);
}
	//使能中断
static	int swm_pin_irq_enable(int pin,int enabled)
{
	return drvp_gpio->swm_pin_irq_enable(pin,enabled);
}

static drv_gpio_t do_drv_gpio={
	.swm_pin_mode = swm_pin_mode,
	.swm_pin_write = swm_pin_write,
	.swm_pin_read = swm_pin_read,
	.swm_pin_attach_irq = swm_pin_attach_irq,
	.swm_pin_detach_irq = swm_pin_detach_irq,
	.swm_pin_irq_enable = swm_pin_irq_enable,

};

drv_gpio_t* gpio = &do_drv_gpio;





