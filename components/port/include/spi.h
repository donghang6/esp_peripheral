#ifndef SPI_H_
#define SPI_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "driver/spi_master.h"
#include "driver/spi_slave.h"

/*
 * funtion: The struct of spi.
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
    spi_bus_config_t buscfg; // spi bus config
    spi_device_interface_config_t devcfg; // spi device config
    uint8_t *send_buffer;
    uint8_t *recv_buffer;
    uint8_t send_byte;
    uint8_t recv_byte;
};

typedef struct spi spi_t;
/*
 * write one byte or bytes
 */
enum DATA
{
    TRANSMIT_A_BYTE = 0,
    TRANSMIT_BYTES = 1,
};

esp_err_t open(spi_t *spi);
esp_err_t close(spi_t *spi);
esp_err_t write(spi_t *spi, uint8_t *buffer, int len, uint8_t data, int mode, uint8_t uservarabile);
void read(void);
#endif // SPI_H_