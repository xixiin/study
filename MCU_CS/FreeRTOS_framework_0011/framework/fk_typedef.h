//不用头文件卫士
#define mem_get_free() 					xPortGetFreeHeapSize()			//例如 int vfree = mem_get_free();
#define mem_alloc(size_bytes) 	pvPortMalloc(size_bytes)		//eg. u8* buf = (u8*)mem_alloc(100);
#define mem_free(pv) 						vPortFree(pv)								//eg. mem_free(buf);

#define get_sys_ticks() ( (uint32_t)(xTaskGetTickCount() * ( 1000.0/configTICK_RATE_HZ ) ) )

#define thread_delay_ms( ms )  do{vTaskDelay( pdMS_TO_TICKS( ms ) );}while(0);

#include "utc.h"
#include "loopbuf.h"

#include "fal.h"

#include "fk_drvp_typdef.h"

#include "fk_drv_typdef.h"




