#include "DMA.h"
#include "stdio.h"

#define BUD_SIZE 16

DMA_HandleTypeDef dma_handle = {0};

uint32_t new_buf[BUD_SIZE]={0};
uint32_t old_buf[BUD_SIZE]={
	0x00000000,0x11111111,0x22222222,0x33333333,
	0x44444444,0x55555555,0x66666666,0x77777777,
	0x88888888,0x99999999,0xaaaaaaaa,0xbbbbbbbb,
	0xcccccccc,0xdddddddd,0xeeeeeeee,0xffffffff
};

void dma_init(void)
{
	__HAL_RCC_DMA1_CLK_ENABLE();
	
	dma_handle.Instance = DMA1_Channel1;
	dma_handle.Init.Direction = DMA_MEMORY_TO_MEMORY;
	
	//内存相关配置
	dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	dma_handle.Init.MemInc = DMA_MINC_ENABLE;
	
	//模式
	dma_handle.Init.Mode = DMA_NORMAL;
	dma_handle.Init.Priority = DMA_PRIORITY_MEDIUM;

	//外设相关
	dma_handle.Init.PeriphInc = DMA_PINC_ENABLE;
	dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	
	HAL_DMA_Init(&dma_handle);

}

void dma_transmit_start(void)
{
	HAL_DMA_Start(&dma_handle,(uint32_t)old_buf,(uint32_t)new_buf, BUD_SIZE);
	while(__HAL_DMA_GET_FLAG(&dma_handle,DMA_FLAG_TC1)==RESET);
	int i=0;
	for(i=0;i<BUD_SIZE;i++)
		printf("buf[%d]:%X\r\n",i,new_buf[i]);
}
