#include "DMA.h"
#include "stdio.h"
#include "uart1.h"


extern UART_HandleTypeDef uart1_handle;  
extern uint8_t uart1_rx_buf[UART1_RX_BUF_SIZE];     /* 接收数据缓冲区 */
DMA_HandleTypeDef dma_handle = {0};


void dma_init(void)
{
	__HAL_RCC_DMA1_CLK_ENABLE();
	
	dma_handle.Instance = DMA1_Channel5;
	dma_handle.Init.Direction = DMA_PERIPH_TO_MEMORY; //外设到内存
	
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
	
	__HAL_LINKDMA(&uart1_handle, hdmarx, dma_handle);

	HAL_UART_Receive_DMA(&uart1_handle, uart1_rx_buf, UART1_RX_BUF_SIZE);

}


