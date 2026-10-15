#include "framework.h"

#define wide_rck 50
#define wide_sck 20

static void init(hc595_pin_t* bus)
{
	gpio->swm_pin_mode( bus->dat,PIN_MODE_OUTPUT );
	gpio->swm_pin_mode( bus->clk,PIN_MODE_OUTPUT );
	gpio->swm_pin_mode( bus->ud,PIN_MODE_OUTPUT );
	gpio->swm_pin_mode( bus->cs,PIN_MODE_OUTPUT );

}

static void write_byte(hc595_pin_t* bus,uint8_t dat)
{
	volatile uint32_t delay = 0;
	
	uint8_t i =0;
	for(i=0;i<8;i++)
	{	
		//根据数据进行输出
		if(dat & 0x80)
			gpio->swm_pin_write( bus->dat,PIN_HIGH );
		else
			gpio->swm_pin_write( bus->dat,PIN_LOW  );
		dat<<=1;
		//产生上升沿,移位寄存器会读出1bit数据，并推出1bit数据
		gpio->swm_pin_write( bus->clk,PIN_LOW  );
		delay = wide_sck; while(delay--); 
		gpio->swm_pin_write( bus->clk,PIN_HIGH  );
		delay = wide_sck; while(delay--); 
		
	}
	
}

//输出多个字节
//len = 2
static void write(hc595_pin_t* bus,uint8_t* buf,uint8_t len)
{
	for(int i = 0;i<len ;i++)
	{
//		write_byte( bus,buf[i] );		//先送近端数据
		write_byte(bus,buf[len-1-i]);			//先送远端数据
	}
	

}

static void update(hc595_pin_t* bus)
{
	volatile uint32_t delay = 0;
		gpio->swm_pin_write( bus->ud,PIN_LOW  );
		delay = wide_sck; while(delay--); 
		gpio->swm_pin_write( bus->ud,PIN_HIGH  );
		delay = wide_sck; while(delay--); 
}

static hc595_opt_t do_hc595_opt={
	.init = init,
	.write = write,
	.update = update,

};

hc595_opt_t* hc595_opt = &do_hc595_opt;
