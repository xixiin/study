#ifndef __W25Q128_H
#define __W25Q128_H

#include "sys.h"

#define FLASH_ManufactDeviceID                  0x90
#define FLASH_WriteEnable                       0x06
#define FLASH_ReadStatusReg1                    0x05
#define FLASH_ReadData                          0x03
#define FLASH_PageProgram                       0x02
#define FLASH_SectorErase                       0x20
#define FLASH_DummyByte                         0xFF
#define PWD_FLASH_ADDR     0x000000

#define w25q128_CS(x)  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, (GPIO_PinState)(x))
void w25q128_init(void);
uint16_t w25q128_read_id(void);
void w25q128_read_data(uint32_t addr,uint8_t *data,uint32_t size);
void w25q128_write_page(uint32_t addr , uint8_t *data , uint16_t size);
void w25q128_erase_sector(uint32_t adddr);
void w25q128_write_3digit_pwd(uint16_t pwd, uint32_t flash_addr);
uint16_t w25q128_read_3digit_pwd(uint32_t flash_addr);

#endif
