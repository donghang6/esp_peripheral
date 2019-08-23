/*
 * @Descripttion: 
 * @version: 
 * @Author: donghang
 * @Date: 2019-08-20 06:39:45
 * @LastEditors: donghang
 * @LastEditTime: 2019-08-24 00:26:25
 */
#ifndef __FLASH_H
#define __FLASH_H			    
#include <stdio.h>
#include "esp_err.h"
#include "spi.h"
//W25Q80  ID  0XEF13
//W25Q16  ID  0XEF14
//W25Q32  ID  0XEF15
//W25Q64  ID  0XEF16	
//W25Q128 ID  0XEF17	
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17

extern uint16_t W25QXX_TYPE;						   
				 
////////////////////////////////////////////////////////////////////////////
 
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 

esp_err_t W25QXX_Init(spi_t *spi);
uint8_t W25QXX_ReadSR(spi_t *spi);
esp_err_t W25QXX_Write_SR(spi_t *spi, uint8_t sr);
esp_err_t W25QXX_Write_Enable(spi_t *spi);
esp_err_t W25QXX_Write_Disable(spi_t *spi);
esp_err_t W25QXX_Write_NoCheck(spi_t *spi, uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
esp_err_t W25QXX_Read(spi_t *spi, uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
esp_err_t W25QXX_Write(spi_t *spi, uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
esp_err_t W25QXX_Erase_Chip(spi_t *spi);
esp_err_t W25QXX_Erase_Sector(spi_t *spi, uint32_t Dst_Addr);
esp_err_t W25QXX_PowerDown(spi_t *spi);
esp_err_t W25QXX_WAKEUP(spi_t *spi);
esp_err_t w25qxx_open(spi_t *spi);
esp_err_t w25qxx_close(spi_t *spi);

void W25QXX_Init_auto(spi_t *spi);
void W25QXX_Write_auto(spi_t *spi, uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void W25QXX_Read_auto(spi_t *spi, uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);

#endif
















