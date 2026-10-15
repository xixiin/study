
#include "framework.h"

/*
 * Copyright (c) 2006-2022, Synwit Technology Co.,Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-05-31     ZYH          first version
 * 2018-12-10     Zohar_Lee    fix bug
 * 2020-07-10     lik          rewrite
 */

//使用宏加输入参数填充数组

#define __SWM_PIN(index, gpio, pin_index)                    \
    {                                                        \
        index, GPIO##gpio, PIN##pin_index, GPIO##gpio##_IRQn \
    }

//使用宏+输入参数，填充数组，提高阅读性
/*
a.引脚位
b.字符串，引脚名字
c.GPIO组
d.GPIO组中的第几号引脚
	#在宏里面用于记号串化，##用于记号粘连
	  GPIOA_IRQn          = 51,
  GPIOB_IRQn          = 52,
  GPIOC_IRQn          = 53,
  GPIOM_IRQn          = 54,
  GPION_IRQn          = 55,
  GPIOP_IRQn          = 56,
	*/	
//填充 SWM32_PIN(1,b,c,d)
#define SWM32_PIN( a,b,c,d ) 				\
	{																	\
		a,#b,GPIO##c,d,GPIO##c##_IRQn		\
	}	

#define GPIO0 ((GPIO_TypeDef *)(0))
#define GPIO0_IRQn (GPIOA0_IRQn)






typedef struct pin{
	 uint32_t index;   			//引脚号
	char* name;       			//引脚名字
	GPIO_TypeDef *group;		//引脚是那个组
	uint32_t  vpin; 			//GPIO组的第几号引脚
	IRQn_Type irq;     			//中断开关序号
	uint32_t irq_mode; 			//触发中断的方式，下降沿触发，上升沿触发，双边沿触发
	pin_callback_t callback;	//中断回调函数
	void *callback_arg;     	//携带中断时，回调函数的输入参数
}pin_t;


//swm320RET7的引脚表
volatile static pin_t swm320_pin_map[] =
{
	//虚拟引脚
	SWM32_PIN(0,NOPin,0,0),
	//真实引脚
	SWM32_PIN( 1, 3V3   , 0, 0 ),   SWM32_PIN( 2, CAP0  , 0, 0 ),   SWM32_PIN( 3, B12   , B, 12),   SWM32_PIN( 4, 3V3   , 0, 0 ),
	SWM32_PIN( 5, 3V3   , 0, 0 ),   SWM32_PIN( 6, GND   , 0, 0 ),   SWM32_PIN( 7, CAP2  , 0, 0 ),   SWM32_PIN( 8, N9    , N, 9 ),
	SWM32_PIN( 9, N10   , N, 10),   SWM32_PIN(10, CAP1  , 0, 0 ),   SWM32_PIN(11, GND   , 0, 0 ),   SWM32_PIN(12, 3V3   , 0, 0 ),
	SWM32_PIN(13, N2    , N, 2 ),   SWM32_PIN(14, N1    , N, 1 ),   SWM32_PIN(15, N0    , N, 0 ),   SWM32_PIN(16, 3V3   , 0, 0 ),
	SWM32_PIN(17, C4    , C, 4 ),   SWM32_PIN(18, C5    , C, 5 ),   SWM32_PIN(19, C6    , C, 6 ),   SWM32_PIN(20, C7    , C, 7 ),
	SWM32_PIN(21, C2    , C, 2 ),   SWM32_PIN(22, C3    , C, 3 ),   SWM32_PIN(23, XI    , 0, 0 ),   SWM32_PIN(24, XO    , 0, 0 ),
	SWM32_PIN(25, RESET , 0, 0 ),   SWM32_PIN(26, M2    , M, 2 ),   SWM32_PIN(27, M3    , M, 3 ),   SWM32_PIN(28, M4    , M, 4 ),
	SWM32_PIN(29, M5    , M, 5 ),   SWM32_PIN(30, M6    , M, 6 ),   SWM32_PIN(31, M7    , M, 7 ),   SWM32_PIN(32, GND   , 0, 0 ),
	SWM32_PIN(33, M1    , M, 1 ),   SWM32_PIN(34, M0    , M, 0 ),   SWM32_PIN(35, P0    , P, 0 ),   SWM32_PIN(36, P1    , P, 1 ),
	SWM32_PIN(37, P2    , P, 2 ),   SWM32_PIN(38, P3    , P, 3 ),   SWM32_PIN(39, P4    , P, 4 ),   SWM32_PIN(40, P5    , P, 5 ),
	SWM32_PIN(41, P6    , P, 6 ),   SWM32_PIN(42, P7    , P, 7 ),   SWM32_PIN(43, P8    , P, 8 ),   SWM32_PIN(44, P9    , P, 9 ),
	SWM32_PIN(45, P10   , P, 10),   SWM32_PIN(46, P11   , P, 11),   SWM32_PIN(47, P12   , P, 12),   SWM32_PIN(48, P13   , P, 13),
	SWM32_PIN(49, B0    , B, 0 ),   SWM32_PIN(50, A0    , A, 0 ),   SWM32_PIN(51, A1    , A, 1 ),   SWM32_PIN(52, A2    , A, 2 ),
	SWM32_PIN(53, A3    , A, 3 ),   SWM32_PIN(54, A4    , A, 4 ),   SWM32_PIN(55, A5    , A, 5 ),   SWM32_PIN(56, N7    , N, 7 ),
	SWM32_PIN(57, N6    , N, 6 ),   SWM32_PIN(58, N5    , N, 5 ),   SWM32_PIN(59, N4    , N, 4 ),   SWM32_PIN(60, N3    , N, 3 ),
	SWM32_PIN(61, A9    , A, 9 ),   SWM32_PIN(62, A10   , A, 10),   SWM32_PIN(63, A11   , A, 11),   SWM32_PIN(64, A12   , A, 12),
		
		
		
};



//根据引脚号获取引脚结构体信息

static pin_t* get_pin(int pin)
{
	volatile pin_t* gpio_obj;

    if (pin < ITEM_NUM(swm320_pin_map))
    {
        gpio_obj = &swm320_pin_map[ pin ];
    }
    else
    {
        gpio_obj = NULL;
    }

    return (pin_t*)gpio_obj;
}



static void swm_pin_mode(int pin, int mode)
{
    volatile pin_t* gpio_obj;
    int dir = 0;			//方向
    int pull_up = 0;		//上拉
    int pull_down = 0;		//下拉

    gpio_obj = get_pin(pin);
    if (gpio_obj == NULL)
    {
        return;
    }
    /* Configure GPIO_InitStructure */
    switch (mode)
    {
    case PIN_MODE_OUTPUT:
        /* output setting */
        dir = 1;
        break;
    case PIN_MODE_INPUT:
        /* input setting: not pull. */
        dir = 0;
        break;
    case PIN_MODE_INPUT_PULLUP:
        /* input setting: pull up. */
        dir = 0;
        pull_up = 1;
        break;
    case PIN_MODE_INPUT_PULLDOWN:
        /* input setting: pull down. */
        dir = 0;
        pull_down = 1;
        break;
    case PIN_MODE_OUTPUT_OD:
        /* output setting: od. */
        dir = 1;
        pull_up = 1;
        break;
    }

    GPIO_Init(gpio_obj->group, gpio_obj->vpin, dir, pull_up, pull_down);
}

static void swm_pin_write( int pin, int value)
{
    pin_t* gpio_obj;

    gpio_obj = get_pin(pin);
	
    if (gpio_obj == NULL)
    {
        return;
    }
    if (value)
    {
        GPIO_AtomicSetBit(gpio_obj->group, gpio_obj->vpin);
    }
    else
    {
        GPIO_AtomicClrBit(gpio_obj->group, gpio_obj->vpin);
    }
}

static int swm_pin_read( int pin)
{
    pin_t* gpio_obj;

    gpio_obj = get_pin(pin);
    if (gpio_obj == NULL)
    {
        return 1;//不合法直接返回1
    }
    return (int)GPIO_GetBit(gpio_obj->group, gpio_obj->vpin);
}

//给一个引脚附加(外部)中断
/*
	pin,引脚序号
	mode,触发外部中断的方式,↑,↓,双边沿
	cb,回调函数
	args,回调函数的输入参数

*/

static int swm_pin_attach_irq(int pin,int mode,pin_callback_t cb,void *args)
{
    pin_t* gpio_obj;

    gpio_obj = get_pin(pin);
    if (gpio_obj == NULL)
    {
        return 1;//不合法直接返回1
    }
	gpio_obj->callback = cb;
	gpio_obj->callback_arg = args;
	gpio_obj->irq_mode = mode;
	
	return 0;
}




static int swm_pin_detach_irq( int pin)
{
    pin_t* gpio_obj;

    gpio_obj = get_pin(pin);
    if (gpio_obj == NULL)
    {
        return 1;//不合法直接返回1
    }
	gpio_obj->callback = 0;
	gpio_obj->callback_arg = 0;
	gpio_obj->irq_mode = 0;
	
	return 0;
}

static int swm_pin_irq_enable(int pin,int enabled)
{
	pin_t* gpio_obj;

    gpio_obj = get_pin(pin);
    if (gpio_obj == NULL)
    {
        return 1;//不合法直接返回1
    }


    if (enabled ==PIN_IRQ_ENABLE )
    {
        switch (gpio_obj->irq_mode)
        {
        case PIN_IRQ_MODE_RISING:
            GPIO_Init(gpio_obj->group, gpio_obj->vpin, 0, 0, 1);
            EXTI_Init(gpio_obj->group, gpio_obj->vpin, EXTI_RISE_EDGE);
            break;
        case PIN_IRQ_MODE_FALLING:
            GPIO_Init(gpio_obj->group, gpio_obj->vpin, 0, 1, 0);
            EXTI_Init(gpio_obj->group, gpio_obj->vpin, EXTI_FALL_EDGE);
            break;
        case PIN_IRQ_MODE_RISING_FALLING:
            GPIO_Init(gpio_obj->group, gpio_obj->vpin, 0, 1, 1);
            EXTI_Init(gpio_obj->group, gpio_obj->vpin, EXTI_BOTH_EDGE);
            break;
        case PIN_IRQ_MODE_HIGH_LEVEL:
            GPIO_Init(gpio_obj->group, gpio_obj->vpin, 0, 0, 1);
            EXTI_Init(gpio_obj->group, gpio_obj->vpin, EXTI_HIGH_LEVEL);
            break;
        case PIN_IRQ_MODE_LOW_LEVEL:
            GPIO_Init(gpio_obj->group, gpio_obj->vpin, 0, 1, 0);
            EXTI_Init(gpio_obj->group, gpio_obj->vpin, EXTI_LOW_LEVEL);
            break;
        default:
            return -2;
        }

//        level = rt_hw_interrupt_disable();
        /* FreeRTOS: the GPIO ISR callback may call ISR-safe (FromISR) APIs, e.g.
           xTaskGetTickCountFromISR.  Such APIs require the interrupt priority value to be
           >= configMAX_SYSCALL_INTERRUPT_PRIORITY, but the GPIO IRQ is left at the reset
           default 0 (highest), which triggers the assert in RVDS/ARM_CM3/port.c:680.
           Assign a FreeRTOS-legal priority here. */
        NVIC_SetPriority( gpio_obj->irq, ( configMAX_SYSCALL_INTERRUPT_PRIORITY >> ( 8 - __NVIC_PRIO_BITS ) ) );
        NVIC_EnableIRQ(gpio_obj->irq);
        EXTI_Open(gpio_obj->group, gpio_obj->vpin);
//        rt_hw_interrupt_enable(level);
    }
    else if (enabled == PIN_IRQ_DISABLE)
    {
//        level = rt_hw_interrupt_disable();
         NVIC_DisableIRQ(gpio_obj->irq);
        EXTI_Close(gpio_obj->group, gpio_obj->vpin);
//        rt_hw_interrupt_enable(level);
    }
    else
    {
        return -3;
    }
    return 0;
}



void GPIOA_Handler(void)
{
    static int gpio[24];
    int index = 0;
    static int init = 0;
    volatile pin_t* gpio_obj;
	//初始化中断
	 if (init == 0)
    {
        init = 1;
        for (gpio_obj = &swm320_pin_map[0];
             gpio_obj->index < ITEM_NUM(swm320_pin_map);
             gpio_obj++)
        {
            if (gpio_obj->group == GPIOA)
            {
                gpio[index] = gpio_obj->index;
                index++;
//                RT_ASSERT(index <= 24)
            }
        }
    }
	//处理中断
	 for (index = 0; index < 24; index++)
    {
        gpio_obj = get_pin(gpio[index]);
        if (EXTI_State(gpio_obj->group, gpio_obj->vpin))
        {
            EXTI_Clear(gpio_obj->group, gpio_obj->vpin);
            if (gpio_obj->callback)
            {
               gpio_obj->callback(gpio_obj ->callback_arg);
            }
        }
    }
}

void GPIOB_Handler(void)
{
    static int gpio[24];
    int index = 0;
    static int init = 0;
    volatile pin_t* gpio_obj;
	//初始化中断
	 if (init == 0)
    {
        init = 1;
        for (gpio_obj = &swm320_pin_map[0];
             gpio_obj->index < ITEM_NUM(swm320_pin_map);
             gpio_obj++)
        {
            if (gpio_obj->group == GPIOB)
            {
                gpio[index] = gpio_obj->index;
                index++;
//                RT_ASSERT(index <= 24)
            }
        }
    }
	//处理中断
	 for (index = 0; index < 24; index++)
    {
        gpio_obj = get_pin(gpio[index]);
        if (EXTI_State(gpio_obj->group, gpio_obj->vpin))
        {
            EXTI_Clear(gpio_obj->group, gpio_obj->vpin);
            if (gpio_obj->callback)
            {
               gpio_obj->callback(gpio_obj ->callback_arg);
            }
        }
    }
}

void GPIOC_Handler(void)
{
    static int gpio[24];
    int index = 0;
    static int init = 0;
    volatile pin_t* gpio_obj;
	//初始化中断
	 if (init == 0)
    {
        init = 1;
        for (gpio_obj = &swm320_pin_map[0];
             gpio_obj->index < ITEM_NUM(swm320_pin_map);
             gpio_obj++)
        {
            if (gpio_obj->group == GPIOC)
            {
                gpio[index] = gpio_obj->index;
                index++;
//                RT_ASSERT(index <= 24)
            }
        }
    }
	//处理中断
	 for (index = 0; index < 24; index++)
    {
        gpio_obj = get_pin(gpio[index]);
        if (EXTI_State(gpio_obj->group, gpio_obj->vpin))
        {
            EXTI_Clear(gpio_obj->group, gpio_obj->vpin);
            if (gpio_obj->callback)
            {
               gpio_obj->callback(gpio_obj ->callback_arg);
            }
        }
    }
}

void GPIOM_Handler(void)
{
    static int gpio[24];
    int index = 0;
    static int init = 0;
    volatile pin_t* gpio_obj;
	//初始化中断
	 if (init == 0)
    {
        init = 1;
        for (gpio_obj = &swm320_pin_map[0];
             gpio_obj->index < ITEM_NUM(swm320_pin_map);
             gpio_obj++)
        {
            if (gpio_obj->group == GPIOM)
            {
                gpio[index] = gpio_obj->index;
                index++;
//                RT_ASSERT(index <= 24)
            }
        }
    }
	//处理中断
	 for (index = 0; index < 24; index++)
    {
        gpio_obj = get_pin(gpio[index]);
        if (EXTI_State(gpio_obj->group, gpio_obj->vpin))
        {
            EXTI_Clear(gpio_obj->group, gpio_obj->vpin);
            if (gpio_obj->callback)
            {
               gpio_obj->callback(gpio_obj ->callback_arg);
            }
        }
    }
}

void GPION_Handler(void)
{
    static int gpio[24];
    int index = 0;
    static int init = 0;
    volatile pin_t* gpio_obj;
	//初始化中断
	 if (init == 0)
    {
        init = 1;
        for (gpio_obj = &swm320_pin_map[0];
             gpio_obj->index < ITEM_NUM(swm320_pin_map);
             gpio_obj++)
        {
            if (gpio_obj->group == GPION)
            {
                gpio[index] = gpio_obj->index;
                index++;
//                RT_ASSERT(index <= 24)
            }
        }
    }
	//处理中断
	 for (index = 0; index < 24; index++)
    {
        gpio_obj = get_pin(gpio[index]);
        if (EXTI_State(gpio_obj->group, gpio_obj->vpin))
        {
            EXTI_Clear(gpio_obj->group, gpio_obj->vpin);
            if (gpio_obj->callback)
            {
               gpio_obj->callback(gpio_obj ->callback_arg);
            }
        }
    }
}

void GPIOP_Handler(void)
{
    static int gpio[24];
    int index = 0;
    static int init = 0;
    volatile pin_t* gpio_obj;
	//初始化中断
	 if (init == 0)
    {
        init = 1;
        for (gpio_obj = &swm320_pin_map[0];
             gpio_obj->index < ITEM_NUM(swm320_pin_map);
             gpio_obj++)
        {
            if (gpio_obj->group == GPIOP)
            {
                gpio[index] = gpio_obj->index;
                index++;
//                RT_ASSERT(index <= 24)
            }
        }
    }
	//处理中断
	 for (index = 0; index < 24; index++)
    {
        gpio_obj = get_pin(gpio[index]);
        if (EXTI_State(gpio_obj->group, gpio_obj->vpin))
        {
            EXTI_Clear(gpio_obj->group, gpio_obj->vpin);
            if (gpio_obj->callback)
            {
               gpio_obj->callback(gpio_obj ->callback_arg);
            }
        }
    }
}

static drvp_gpio_t do_drvp_gpio = {
	.swm_pin_mode = swm_pin_mode,
	.swm_pin_write = swm_pin_write,
	.swm_pin_read = swm_pin_read,
	.swm_pin_attach_irq = swm_pin_attach_irq,
	.swm_pin_detach_irq = swm_pin_detach_irq,
	.swm_pin_irq_enable = swm_pin_irq_enable,
};

drvp_gpio_t* drvp_gpio = &do_drvp_gpio;

//int swm_pin_init(void)
//{
//    return rt_device_pin_register("pin", &swm_pin_ops, RT_NULL);
//}

