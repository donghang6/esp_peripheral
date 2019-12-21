/*
 * @Descripttion: 
 * @version: 
 * @Author: donghang
 * @Date: 2019-08-04 21:56:33
 * @LastEditors: donghang
 * @LastEditTime: 2019-08-21 22:39:31
 */
#ifndef SPI_H_
#define SPI_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "driver/spi_master.h"
#include "sdkconfig.h"

#define CONFIG_IO_DC 17
#define CONFIG_MISO_IO_NUM 25
#define CONFIG_MOSI_IO_NUM 23
#define CONFIG_SCLK_IO_NUM 19
#define CONFIG_MAX_TRANSFER_SZ 100
#define CONFIG_SPICS_IO_NUM 22
#define CONFIG_QUEUE_SIZE 7
#define CONFIG_MODE 0
#define CONFIG_IO_RES 16
#define CONFIG_CLOCK_SPEED_HZ 1000000
#define CONFIG_DMA_CHAN 1
#define CONFIG_COMMAND_BITS 0
#define CONFIG_ADDRESS_BITS 0
/*
 * @brief: The struct of spi.
 * @host: SPI peripheral that controls this bus. Only support HSPI_HOST and VSPI_HOST
 * @bus_config: Pointer to a spi_bus_config_t struct specifying how the host should be initialized
 * @dma_chan: choose dma channel. Either channel 1 or 2, or 0 in the case when no DMA is required.  
 *            Selecting a DMA channel for a SPI bus allows transfers on the bus to have sizes only 
 *            limited by the amount of internal memory. Selecting no DMA channel (by passing the 
 *            value 0) limits the amount of bytes transfered to a maximum of 64. Set to 0 if only 
 *            the SPI flash uses this bus.
 */
struct spi
{
    esp_err_t ret;
    spi_device_handle_t spi_handle; // attach spi and device
    int dma_chan;
    spi_host_device_t host;
    uint8_t *send_buffer;
    uint8_t *recv_buffer;
    uint8_t send_byte;
    uint8_t recv_byte;
};

typedef struct spi spi_t;

/*
 * @brief: Initialize the SPI bus.
 * @note: acquire the host of spi.
 * @param: `spi` the spi structure hold the content of SPI bus.
 * @return: 
 *         - ESP_ERR_INVALID_ARG if configuration is invalid
 *         - ESP_ERR_INVALID_STATE if host already is in use
 *         - ESP_ERR_NO_MEM if out of memory
 *         - ESP_OK on success
 */
esp_err_t spi_bus_init(spi_t *spi);

/*
 * @brief: open a spi port
 * @note: if you want to add more than one device, 'spi_bus_add_device' should detach from device to device.
 * @param: `spi` the handle of SPI
 *         `devcfg` the spi device interface config
 * @return: ESP_OK on success 
 *          ESP_FAIL on fail
 */
esp_err_t openport(spi_t *spi, spi_device_interface_config_t *devcfg);

/**
 * @brief: remove device from spi bus
 * @param: `spi` the handle of SPI 
 * @return: ESP_OK on success
 *          ESP_FAIL on fail
 */
esp_err_t closeport(spi_t *spi);

/**
 * @brief: write content to device attached through spi port.
 * @param: `spi` the handle of SPI
 *         `data` the single byte to be transmitted to the peer device
 *         `usevarabile` User-defined variable. Can be used to store eg transaction ID
 *         `cmd` if set the length of command in spi_device_interface_config_t, this param will be transmitted 
 *         `add` if set the length of address in spi_device_interface_config_t, this param will be transmitted 
 *         `rxdata` the receive data which is one byte from the peer advice 
 * @return: ESP_OK success
 *          ESP_FAIL fail
 */
esp_err_t write_byte(spi_t *spi, uint8_t data, void * uservarabile, uint16_t cmd, uint64_t addr);

/**
 * @brief: write content to device attached through spi port.
 * @param: `spi` the handle of SPI 
 *         `txbuffer` the data buffer waitted to be transmitted.
 *         `len` the length of buffer.
 *         `uservarabile` User-defined variable. Can be used to store eg transaction ID. if in oled, this parameter could be cmd or data.
 *         `cmd` if set the command bits, cmd will be the command data. And this parameter will be the first data to be transmitted
 *         `addr` if set the address bits, addr will be the address data. And this parameter will be the second data to be transmitted
 * @return: ESP_OK success
 *          ESP_FAIL fail
 */
esp_err_t read_write_buff(spi_t *spi, uint8_t *txbuffer, size_t len, void *uservarabile, uint16_t cmd, uint64_t addr, uint8_t *rxbuffer);
#endif // SPI_H_

