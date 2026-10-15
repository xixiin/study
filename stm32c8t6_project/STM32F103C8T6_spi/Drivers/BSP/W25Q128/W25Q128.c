#include "W25Q128.h"


SPI_HandleTypeDef spi_handle = {0};

void w25q128_spi_init(void)
{
	spi_handle.Instance = SPI1;
	spi_handle.Init.Mode = SPI_MODE_MASTER;
	spi_handle.Init.Direction = SPI_DIRECTION_2LINES;
	spi_handle.Init.DataSize = SPI_DATASIZE_8BIT;
	spi_handle.Init.CLKPolarity = SPI_POLARITY_LOW;
	spi_handle.Init.CLKPhase = SPI_PHASE_1EDGE;
	spi_handle.Init.NSS = SPI_NSS_SOFT;
	spi_handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
	spi_handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
	spi_handle.Init.TIMode = SPI_TIMODE_DISABLE;
	spi_handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	spi_handle.Init.CRCPolynomial = 7;
	
	HAL_SPI_Init(&spi_handle);
}


void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
	if(hspi->Instance == SPI1)
	{
		__HAL_RCC_SPI1_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		
		GPIO_InitTypeDef gpio_init={0};
		gpio_init.Pin = GPIO_PIN_4;
		gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
		gpio_init.Pull = GPIO_PULLUP;
		gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
		
		HAL_GPIO_Init(GPIOA, &gpio_init);

		gpio_init.Pin = GPIO_PIN_5 | GPIO_PIN_7;
		gpio_init.Mode = GPIO_MODE_AF_PP;
		
		HAL_GPIO_Init(GPIOA, &gpio_init);
		
		gpio_init.Pin = GPIO_PIN_6;
		gpio_init.Mode = GPIO_MODE_INPUT;
		
		HAL_GPIO_Init(GPIOA, &gpio_init);
	}
}

uint8_t w25q128_spi_swap_byte(uint8_t data)
{
	uint8_t rec_data = 0;
	HAL_SPI_TransmitReceive(&spi_handle, &data,&rec_data,1,100);
	return rec_data;
}

void w25q128_init(void)
{
	w25q128_spi_init();

}

uint16_t w25q128_read_id(void)
{
	uint16_t device_id = 0;
	w25q128_CS(0);
	w25q128_spi_swap_byte(FLASH_ManufactDeviceID);
	w25q128_spi_swap_byte(0x00);
	w25q128_spi_swap_byte(0x00);
	w25q128_spi_swap_byte(0x00);
	device_id = w25q128_spi_swap_byte(FLASH_DummyByte)<<8;
	device_id |= w25q128_spi_swap_byte(FLASH_DummyByte);
	
	w25q128_CS(1);
	return device_id;
}

void w25q128_write_endble(void)
{
	w25q128_CS(0);
	w25q128_spi_swap_byte(FLASH_WriteEnable);
	w25q128_CS(1);

}

uint8_t w25q128_read_srl(void)
{
	uint8_t recv_data = 0;
	w25q128_CS(0);
	w25q128_spi_swap_byte(FLASH_ReadStatusReg1);
	recv_data = w25q128_spi_swap_byte(FLASH_DummyByte);
	w25q128_CS(1);
	return recv_data;
}

void w25q128_wait_busy()
{
	while((w25q128_read_srl()&0x01)==0x01);
}

void w25q128_send_addr(uint32_t addr)
{
	w25q128_spi_swap_byte(addr>>16);
	w25q128_spi_swap_byte(addr>>8);
	w25q128_spi_swap_byte(addr);

}

void w25q128_read_data(uint32_t addr,uint8_t *data,uint32_t size)
{
	uint32_t i=0;
	w25q128_CS(0);
	w25q128_spi_swap_byte(FLASH_ReadData);
	w25q128_send_addr(addr);
	
	for(i = 0; i < size ;i++)
		data[i] = w25q128_spi_swap_byte(FLASH_DummyByte);
	w25q128_CS(1);

}

void w25q128_write_page(uint32_t addr , uint8_t *data , uint16_t size)
{
	uint16_t i=0;
	w25q128_write_endble();
	w25q128_CS(0);
	w25q128_spi_swap_byte(FLASH_PageProgram);
	w25q128_send_addr(addr);
	for(i = 0; i < size ;i++)
		w25q128_spi_swap_byte(data[i]);
	w25q128_CS(1);
	//忙等待
	w25q128_wait_busy();
}

void w25q128_erase_sector(uint32_t adddr)
{
	//写使能
	w25q128_write_endble();
	//等待空闲
	w25q128_wait_busy();
	//拉低片选
	w25q128_CS(0);
	//发送扇区檫除指令
	w25q128_spi_swap_byte(FLASH_SectorErase);
	//发送地址
	w25q128_send_addr(adddr);
	//拉高片选
	w25q128_CS(1);
	//等待空闲
	w25q128_wait_busy();
	
	

}
