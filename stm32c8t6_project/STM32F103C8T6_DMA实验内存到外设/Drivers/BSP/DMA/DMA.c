#include "DMA.h"
#include "stdio.h"

#define BUD_SIZE 16

extern UART_HandleTypeDef uart1_handle;  

DMA_HandleTypeDef dma_handle = {0};


void dma_init(void)
{
	__HAL_RCC_DMA1_CLK_ENABLE();
	
	dma_handle.Instance = DMA1_Channel4;
	dma_handle.Init.Direction = DMA_MEMORY_TO_PERIPH;
	
	//内存相关配置
	dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	dma_handle.Init.MemInc = DMA_MINC_ENABLE;
	
	//模式
	dma_handle.Init.Mode = DMA_NORMAL;
	dma_handle.Init.Priority = DMA_PRIORITY_MEDIUM;

	//外设相关
	dma_handle.Init.PeriphInc = DMA_PINC_DISABLE;
	dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	
	HAL_DMA_Init(&dma_handle);
	
	__HAL_LINKDMA(&uart1_handle, hdmatx, dma_handle);

}


