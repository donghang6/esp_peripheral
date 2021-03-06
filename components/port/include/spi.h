/*
 * @Descripttion: 
 * @version: 
 * @Author: donghang
 * @Date: 2019-08-04 21:56:33
 * @LastEditors: donghang
 * @LastEditTime: 2019-08-15 23:57:40
 */
#ifndef SPI_H_
#define SPI_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "driver/spi_master.h"
#include "sdkconfig.h"
#include "esp_err.h"

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
#define CONFIG_DMA_CHAN 2
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

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
esp_err_t spi_bus_init(spi_t *spi);

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
esp_err_t openport(spi_t *spi, spi_device_interface_config_t *devcfg);

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
esp_err_t closeport(spi_t *spi);

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
esp_err_t write_byte(spi_t *spi, uint8_t data, void * uservarabile, uint16_t cmd, uint64_t addr);

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
esp_err_t write_buff(spi_t *spi, uint8_t *buffer, size_t len, void *uservarabile, uint16_t cmd, uint64_t addr);

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
void read_byte(void);

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
void read_buff(void);
#endif // SPI_H_

