/*
 * @Descripttion: 
 * @version: 
 * @Author: donghang
 * @Date: 2019-08-20 06:39:45
 * @LastEditors: donghang
 * @LastEditTime: 2019-08-23 08:26:55
 */
#include "w25qxx.h"
#include "stdlib.h"
#include "string.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define ONE_PAGE_BYTES 256
#define ONE_SECTOR_SIZE 4096

// uint8_t W25QXX_BUFFER[4096] = {}; // a seator 4Kb

spi_device_interface_config_t w25qxxcfg  = {
    .command_bits = CONFIG_COMMAND_BITS,
    .address_bits = CONFIG_ADDRESS_BITS,
    .clock_speed_hz = CONFIG_CLOCK_SPEED_HZ,
    .mode = CONFIG_MODE,
    .spics_io_num = 18,
    .queue_size = CONFIG_QUEUE_SIZE,
};

uint16_t W25QXX_TYPE;

/**
 * @brief: read and write buffer via standard SPI
 * @param: `spi` the handle of SPI
 *         `buffer` the data to be tramsmitted to device attached
 *         `len` the length of data 
 *         `rxbuffer` the data read from peer device 
 * @return: ESP_OK on success
 *          ESP_FAIL on fail
 */
static esp_err_t w25qxx_read_write_buf(spi_t *spi, uint8_t *buffer, size_t len, uint8_t* rxbuffer)
{
    return read_write_buff(spi, buffer, len, NULL, 0, 0, rxbuffer);
}

/**
 * @brief: read and write on byte via standard SPI
 * @param `spi` the handle of SPI 
 *        `data` the one byte data to be tramsmitted to device attached
 *        `rxdata` the data read from peer device
 * @return: ESP_OK on success
 *          ESP_FAIL on fail
 */
static esp_err_t w25qxx_write_byte(spi_t *spi, uint8_t data)
{
    return write_byte(spi, data, NULL, 0, 0);
}

/**
 * @brief: initialize w25qxx
 * @param: spi the handle of SPI 
 * @return: ESP_OK on success
 *          ESP_FAIL on fail
 */
esp_err_t W25QXX_Init(spi_t *spi)
{
    spi->ret = openport(spi, &w25qxxcfg);
    W25QXX_TYPE = W25QXX_ReadID(spi);
    printf("w25qxx Manufacturer ID is %x\n", (W25QXX_TYPE >> 8) & 0x00FF);
    printf("w25qxx Device ID is %x\n", W25QXX_TYPE & 0x00FF);
    return spi->ret;
}

/**
 * @brief: read the Manufacturer ID and Device ID
 * @param: `spi` the handle of SPI
 * @return: Manufacturer ID and Device ID, the high is Manufacturer ID and low id Device ID
 */
uint16_t W25QXX_ReadID(spi_t *spi)
{
    uint8_t cmd_set[6] = {0, 0, 0, 0, 0xFF, 0xFF};
    uint8_t receive[6] = {0, 0, 0, 0, 0, 0};
    cmd_set[0] = W25X_ManufactDeviceID;
    spi->ret = w25qxx_read_write_buf(spi, cmd_set, 6, receive);
    assert(spi->ret == ESP_OK);
    return (receive[4] << 8) | receive[5];
}

/**
 * @brief: read status register
 * @param: `spi` the handle of SPI
 * @return: the value of status register
 */
uint8_t W25QXX_ReadSR(spi_t *spi)
{
    uint8_t cmd_set[2] = {0, 0xFF};
    uint8_t receive[2] = {0, 0};
    cmd_set[0] = W25X_ReadStatusReg;
    spi->ret = w25qxx_read_write_buf(spi, cmd_set, 2, receive);
    assert(spi->ret == ESP_OK);
    return receive[1];
}

/**
 * @brief: write status register
 * @param: `spi` the handle of SPI
 *         `sr` the status register
 * @return: ESP_OK on success
 *          ESP_FAIL on fail
 */
esp_err_t W25QXX_Write_SR(spi_t *spi, uint8_t sr)
{
    uint8_t cmd_set[2] = {0, 0};
    cmd_set[0] = W25X_WriteStatusReg;
    cmd_set[1] = sr;
    spi->ret = w25qxx_read_write_buf(spi, cmd_set, 2, NULL);
    assert(spi->ret == ESP_OK);
    return spi->ret;
}

/**
 * @brief: set the write enable latch bit int the status register 
 * @param: `spi` the handle of SPI
 * @return: ESP_OK on success
 *          ESP_FAIL on fail
 */
esp_err_t W25QXX_Write_Enable(spi_t *spi)
{
    uint8_t cmd_set = W25X_WriteEnable;
    spi->ret = w25qxx_write_byte(spi, cmd_set);
    assert(spi->ret == ESP_OK);
    return spi->ret;
}

/**
 * @brief: set the write disenable latch bit int the status register 
 * @param: `spi` the handle of SPI
 * @return: ESP_OK on success
 *          ESP_FAIL on fail
 */
esp_err_t W25QXX_Write_Disable(spi_t *spi)
{
    uint8_t cmd_set = W25X_WriteDisable;
    spi->ret = w25qxx_write_byte(spi, cmd_set);
    assert(spi->ret == ESP_OK);
    return spi->ret;
}

/**
 * @brief: Reads the data of the specified length at the specified location. the address is from high to low
 * @param: `pBuffer` the data which has been read to be stored in this array
 *         `ReadAddr` the address that is about to read 
 *         `NunByteToRead` the bytes to read
 * @return: ESP_OK on success
 *          ESP_FAIL on fail
 */
esp_err_t W25QXX_Read(spi_t *spi, uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
    uint8_t *cmd_set = (uint8_t*)malloc(sizeof(uint8_t)*(NumByteToRead+4)); // the 4 is address(3) add opcode(1)
    memset(cmd_set, 0xFF, NumByteToRead+4);
    cmd_set[0] = W25X_ReadData;
    cmd_set[1] = (uint8_t)(ReadAddr >> 16);
    cmd_set[2] = (uint8_t)(ReadAddr >> 8);
    cmd_set[3] = (uint8_t)(ReadAddr);
    spi->ret = w25qxx_read_write_buf(spi, cmd_set, NumByteToRead+4, pBuffer);
    assert(spi->ret == ESP_OK);
    free(cmd_set);
    return spi->ret;
}

/**
 * @brief: SPI writes less than 256 bytes of data on one page (0 ≤ 65535). 
 *         Writes up to 256 bytes of data at the specified address.
 * @param: `pBuffer` data storage area 
 *         ` WriteAddr` address to write data
 *         `NumByteToWrite` the bytes to write. the number shoule not bigger than the rest bytes of this page
 * @return: ESP_OK on success
 *          ESP_FAIL on fail
 */
static esp_err_t W25QXX_Write_Page(spi_t *spi, uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    spi->ret = W25QXX_Write_Enable(spi);
    uint8_t *cmd_set = (uint8_t*)malloc(sizeof(uint8_t)*(NumByteToWrite+4)); // the 4 is address(3) add opcode(1)
    cmd_set[0] = W25X_PageProgram;
    cmd_set[1] = (uint8_t)(WriteAddr >> 16);
    cmd_set[2] = (uint8_t)(WriteAddr >> 8);
    cmd_set[3] = (uint8_t)(WriteAddr);
    memcpy(cmd_set+4, pBuffer, NumByteToWrite);
    spi->ret = w25qxx_read_write_buf(spi, cmd_set, NumByteToWrite+4, pBuffer);
    assert(spi->ret == ESP_OK);
    W25QXX_Wait_Busy(spi);
    free(cmd_set);
    return spi->ret;
}

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
esp_err_t W25QXX_Write_NoCheck(spi_t *spi, uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint16_t pageremain = ONE_PAGE_BYTES - WriteAddr % ONE_PAGE_BYTES; // the reamin bytes of this page
    if(NumByteToWrite <= pageremain) {
        pageremain = NumByteToWrite;
    } 
    while(true) {
        spi->ret = W25QXX_Write_Page(spi, pBuffer, WriteAddr, pageremain);
        assert(spi->ret == ESP_OK);
        if(NumByteToWrite == pageremain) {
            break;
        } else {
            pBuffer += pageremain;
            WriteAddr += pageremain;
            NumByteToWrite -= pageremain;
            if(NumByteToWrite > ONE_PAGE_BYTES) {
                pageremain = ONE_PAGE_BYTES;
            } else {
                pageremain = NumByteToWrite;
            }
        }
    }
    return spi->ret;
}

uint8_t W25QXX_BUFFER[4][1024] = {};
static void W25QXX_Read_4K(spi_t *spi)
{
    W25QXX_Read(spi, W25QXX_BUFFER[0], 0, 1024);
    W25QXX_Read(spi, W25QXX_BUFFER[1], 1024, 1024);
    W25QXX_Read(spi, W25QXX_BUFFER[2], 2048, 1024);
    W25QXX_Read(spi, W25QXX_BUFFER[3], 3072, 1024);
}
/**
 * @brief: write SPI FLASH in indicate address. this function has erase feature(the least erase size is 4Kb)
 * @param: `spi` the handle of SPI
 *         `pBuffer` data storage area
 *         `WriteAddr` address to write data, the address should be 4k alignment.
 *         `NumByteToWrite` the bytes to write. the number shoule not bigger than the rest bytes of this page
 * @return: ESP_OK on success
 *          ESP_FAIL on fail
 */
esp_err_t W25QXX_Write(spi_t *spi, uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    spi->ret = W25QXX_Write_Enable(spi);
    assert(spi->ret == ESP_OK);
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;	   
	uint8_t* W25QXX_BUF = W25QXX_BUFFER;
    uint16_t pos = 0;
   	W25QXX_BUF = W25QXX_BUFFER;	     
 	secpos = WriteAddr / 4096;// the address of sector(the number of sector)
	secoff = WriteAddr % 4096;// the offset in this sector
	secremain = 4096 - secoff;// the rest size of this sector
    // if the bytes smaller than the the rest of this sector
    if(NumByteToWrite <= secremain) {
        secremain = NumByteToWrite;
    }
    while(true) {
        W25QXX_Read_4K(spi); // read the all content of the sector
        assert(spi->ret == ESP_OK);
        
        for(pos = 0; pos < secremain; pos++) {
            if(W25QXX_BUFFER[secoff + pos] != 0xFF) {
                break; // need erase
            }
        }
        if(pos < secremain) {
            // need erase
            spi->ret = W25QXX_Erase_Sector(spi, secpos);
            assert(spi->ret == ESP_OK);
            for(pos = 0; pos < secremain; pos++) {
                W25QXX_BUF[secoff + pos] = pBuffer[pos];
            }
            W25QXX_Write_NoCheck(spi, W25QXX_BUF, secpos, ONE_SECTOR_SIZE);
        } else {
            // do not need erase
            W25QXX_Write_NoCheck(spi, pBuffer, WriteAddr, secremain);
        }
        if(secremain == NumByteToWrite) {
            // end of writing
            break;
        } else {
            secpos++;
            secoff = 0;
            pBuffer += secremain;
            WriteAddr += secremain;
            NumByteToWrite -= secremain;
            if(NumByteToWrite > ONE_SECTOR_SIZE) {
                secremain = ONE_SECTOR_SIZE;
            } else {
                secremain = NumByteToWrite;
            }
        }
    }
    for(int pos = 0; pos < ONE_SECTOR_SIZE; pos++) {
        
    }
    return spi->ret;
}

/**
 * @brief: erase all chip
 * @param: `spi` the handle of SPI 
 * @return: 
 */
esp_err_t W25QXX_Erase_Chip(spi_t *spi)
{
    spi->ret = W25QXX_Write_Enable(spi);
    assert(spi->ret == ESP_OK);
    W25QXX_Wait_Busy(spi);
    spi->ret = w25qxx_write_byte(spi, W25X_ChipErase);
    assert(spi->ret == ESP_OK);
    W25QXX_Wait_Busy(spi);
    return spi->ret;   
}

/**
 * @brief: erase a sector according the sector address
 * @param: `spi` the handle of SPI
 *         `Dst_Addr` the sector address
 * @return: ESP_OK on success
 *          ESP_FAIL on fail
 */
esp_err_t W25QXX_Erase_Sector(spi_t *spi, uint32_t Dst_Addr)
{
    uint8_t cmd_set[4] = {};
 	Dst_Addr *= 4096;
    spi->ret = W25QXX_Write_Enable(spi);
    assert(spi->ret == ESP_OK);
    W25QXX_Wait_Busy(spi);
    cmd_set[0] = W25X_SectorErase;
    cmd_set[1] = (uint8_t)(Dst_Addr >> 16);
    cmd_set[2] = (uint8_t)(Dst_Addr >> 8);
    cmd_set[3] = (uint8_t)(Dst_Addr);
    spi->ret = w25qxx_read_write_buf(spi, cmd_set, 4, NULL);
    assert(spi->ret == ESP_OK);
    W25QXX_Wait_Busy(spi);
    return spi->ret;
}

/**
 * @brief: wait to enter standby state
 * @param: `spi` the handle of SPI 
 * @return: ESP_OK on success
 *          ESP_FAIL on fail
 */
void W25QXX_Wait_Busy(spi_t *spi)
{
    while((W25QXX_ReadSR(spi) & 0x01)==0x01) // wait BUST bit to be cleared
        continue;
}

/**
 * @brief: enter power down mode
 * @param: `spi` the handle of SPI
 * @return: ESP_OK on success
 *          ESP_FAIL on fail
 */
esp_err_t W25QXX_PowerDown(spi_t *spi)
{
    spi->ret = w25qxx_write_byte(spi, W25X_PowerDown);
    assert(spi->ret == ESP_OK);
    vTaskDelay(1 / portTICK_RATE_MS );
    return spi->ret;
}

/**
 * @brief: wakeup the chip
 * @param {type} 
 * @return: ESP_OK on success
 *          ESP_FAIL on fail
 */
esp_err_t W25QXX_WAKEUP(spi_t *spi)
{
    spi->ret = w25qxx_write_byte(spi, W25X_ReleasePowerDown);
    assert(spi->ret == ESP_OK);
    vTaskDelay(1 / portTICK_RATE_MS );
    return spi->ret;
}