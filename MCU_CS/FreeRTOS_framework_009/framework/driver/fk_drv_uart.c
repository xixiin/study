#include "framework.h"

//环形缓冲区
static loopbuf_t* lb_uart_rx[_e_max_uart];

//接收中断回调函数，drvp调用，根据串口号，输入index用于判断是哪一个串口

static void recv_callback( int index,uint8_t dat )
{
	loopbuf_write(lb_uart_rx[index],&dat,1);

}

extern drvp_uart_t* drvp_uart ;

static void irq_enable(int index , int enable)
{
		drvp_uart->irq_enable(index,enable);
}

static void write(int index,uint8_t dat)
{
	drvp_uart->write(index,dat);
}

static void close( int index)
{
	drvp_uart->close(index);
}

static void open( int index)
{

	drvp_uart->open(index);
}

static void uart_init(int index , uint32_t baudrate)
{
	
		int err = 0;
	loopbuf_t* tlib = NULL;
	if(index >= _e_max_uart)
	{
		err=1;
		goto __deal_err;//index不合法
	}
		tlib = loopbuf_init(1024);
	if(!tlib)
	{
		err = 2;//缓冲区分配错误
		goto __deal_err;
	}
	lb_uart_rx[index] = tlib;
	
	
	drvp_uart->set_int_rxfunc(index,recv_callback);
	drvp_uart->uart_init( index ,  baudrate);
	
	return;
	__deal_err:
	printf("err[%d]:%s(%d)\r\n",err,__func__,index );
	while(1);
}

static int read(int index ,void* buf , int len )
{
	return loopbuf_read(lb_uart_rx[index],buf,len);
}

static void clr_rxbuf(int index)
{
	loopbuf_reset(lb_uart_rx[index]);

}

static drv_uart_t do_drv_uart = {
	.uart_init = uart_init,
	.open = open,
	.close = close,
	.write = write,
	.irq_enable = irq_enable,
	.read = read,
	.clr_rxbuf = clr_rxbuf,
};

drv_uart_t* uart =  &do_drv_uart;

