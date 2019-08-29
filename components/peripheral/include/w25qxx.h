/*
 * @Descripttion: 
 * @version: 
 * @Author: donghang
 * @Date: 2019-08-20 06:39:45
 * @LastEditors: donghang
 * @LastEditTime: 2019-08-25 21:55:17
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

/**
 * @brief: initialize w25qxx
 * @param: spi the handle of SPI 
 * @return: ESP_OK on success
 *          ESP_FAIL on fail
 */
esp_err_t W25QXX_Init(spi_t *spi);

/**
 * @brief: read status register
 * @param: `spi` the handle of SPI
 * @return: the value of status register
 */
uint8_t W25QXX_ReadSR(spi_t *spi);

/**
 * @brief: write status register
 * @param: `spi` the handle of SPI
 *         `sr` the status register
 * @return: ESP_OK on success
 *          ESP_FAIL on fail
 */
esp_err_t W25QXX_Write_SR(spi_t *spi, uint8_t sr);

/**
 * @brief: set the write enable latch bit int the status register 
 * @param: `spi` the handle of SPI
 * @return: ESP_OK on success
 *          ESP_FAIL on fail
 */
esp_err_t W25QXX_Write_Enable(spi_t *spi);

/**
 * @brief: set the write disenable latch bit int the status register 
 * @param: `spi` the handle of SPI
 * @return: ESP_OK on success
 *          ESP_FAIL on fail
 */
esp_err_t W25QXX_Write_Disable(spi_t *spi);

/**
 * @brief: write SPI FLASH with no check. must confirm the content is about to be written is 0xFF, else the write operation will fail.
 *         this function have auto change page feature. the data could be written in indicate address which should not over border.
 * @param: `spi` the handle of SPI
 *         `pBuffer` data storage area
 *         `WriteAddr` address to write data
 *         `NumByteToWrite` the bytes to write. the number shoule not bigger than the rest bytes of this page
 * @return: ESP_OK on success
 *          ESP_FAIL on fail
 */
esp_err_t W25QXX_Write_NoCheck(spi_t *spi, uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);

/**
 * @brief: Reads the data of the specified length at the specified location. the address is from high to low
 * @param: `pBuffer` the data which has been read to be stored in this array
 *         `ReadAddr` the address that is about to read 
 *         `NunByteToRead` the bytes to read
 * @return: ESP_OK on success
 *          ESP_FAIL on fail
 */
esp_err_t W25QXX_Read(spi_t *spi, uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);

/**
 * @brief: write SPI FLASH in indicate address. this function has erase feature(the least erase size is 4Kb)
 * @param: `spi` the handle of SPI
 *         `pBuffer` data storage area
 *         `WriteAddr` address to write data, the address should be 4k alignment.
 *         `NumByteToWrite` the bytes to write. the number shoule not bigger than the rest bytes of this page
 * @return: ESP_OK on success
 *          ESP_FAIL on fail
 */
esp_err_t W25QXX_Write(spi_t *spi, uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);

/**
 * @brief: erase all chip
 * @param: `spi` the handle of SPI 
 * @return: 
 */
esp_err_t W25QXX_Erase_Chip(spi_t *spi);

/**
 * @brief: erase a sector according the sector address
 * @param: `spi` the handle of SPI
 *         `Dst_Addr` the sector address
 * @return: ESP_OK on success
 *          ESP_FAIL on fail
 */
esp_err_t W25QXX_Erase_Sector(spi_t *spi, uint32_t Dst_Addr);

/**
 * @brief: enter power down mode
 * @param: `spi` the handle of SPI
 * @return: ESP_OK on success
 *          ESP_FAIL on fail
 */
esp_err_t W25QXX_PowerDown(spi_t *spi);

/**
 * @brief: wakeup the chip
 * @param {type} 
 * @return: ESP_OK on success
 *          ESP_FAIL on fail
 */
esp_err_t W25QXX_WAKEUP(spi_t *spi);

/**
 * @brief: add w25qxx from spi bus
 * @param: `spi` the handle of SPI
 *         `devcfg` the pointer of spi_device_interface_config_t
 * @return: ESP_OK success
 *          ESP_FAIL fail
 */
esp_err_t w25qxx_open(spi_t *spi);

/**
 * @brief: remove w25qxx from spi bus
 * @param: `spi` the handle of SPI
 * @return: ESP_OK success
 *          ESP_FAIL fail
 */
esp_err_t w25qxx_close(spi_t *spi);

/**
 * @brief: initialize w25qxx and auto attach with SPI bus
 * @param: spi the handle of SPI 
 * @return: ESP_OK on success
 *          ESP_FAIL on fail
 */
void W25QXX_Init_auto(spi_t *spi);

/**
 * @brief: write SPI FLASH in indicate address. this function has erase feature(the least erase size is 4Kb) and auto attach with SPI bus
 * @param: `spi` the handle of SPI
 *         `pBuffer` data storage area
 *         `WriteAddr` address to write data, the address should be 4k alignment.
 *         `NumByteToWrite` the bytes to write. the number shoule not bigger than the rest bytes of this page
 * @return: ESP_OK on success
 *          ESP_FAIL on fail
 */
void W25QXX_Write_auto(spi_t *spi, uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);

/**
 * @brief: Reads the data of the specified length at the specified location. the address is from high to low and auto attach with SPI bus
 * @param: `pBuffer` the data which has been read to be stored in this array
 *         `ReadAddr` the address that is about to read 
 *         `NunByteToRead` the bytes to read
 * @return: ESP_OK on success
 *          ESP_FAIL on fail
 */
void W25QXX_Read_auto(spi_t *spi, uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);

#endif
















