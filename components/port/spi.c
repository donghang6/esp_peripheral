/*
 * @Descripttion: 
 * @version: 
 * @Author: donghang
 * @Date: 2019-08-04 21:56:33
 * @LastEditors: donghang
 * @LastEditTime: 2019-08-15 23:55:02
 */
#include  "spi.h"

#define ONE_BYTE_LENGTH 8
#define TOTAL_DATA_LENGTH(length) length*ONE_BYTE_LENGTH

// 　could i attach more than one device in SPI Master?
spi_bus_config_t buscfg = {
    .miso_io_num = CONFIG_MISO_IO_NUM,
    .mosi_io_num = CONFIG_MOSI_IO_NUM,
    .sclk_io_num = CONFIG_SCLK_IO_NUM,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1,
    .max_transfer_sz = CONFIG_MAX_TRANSFER_SZ,
};

/*
 * @brief: Initialize the SPI bus.
 * @note: acquire the host of spi.
 * @param: spi the spi structure hold the content of SPI bus.
 * @return: 
 *         - ESP_ERR_INVALID_ARG if configuration is invalid
 *         - ESP_ERR_INVALID_STATE if host already is in use
 *         - ESP_ERR_NO_MEM if out of memory
 *         - ESP_OK on success
 */
esp_err_t spi_bus_init(spi_t *spi)
{
    #if CONFIG_HSPI_HOST
        spi->host = HSPI_HOST;
    #elif CONFIG_VSPI_HOST
        spi->host = VSPI_HOST;
    #endif
    spi->ret = spi_bus_initialize(spi->host, &buscfg, CONFIG_DMA_CHAN);
    assert(spi->ret == ESP_OK);    
    return spi->ret;
}

/*
 * @brief: open a spi port
 * @note: if you want to add more than one device, 'spi_bus_add_device' should detach from device to device.
 * @spi: the all of SPI
 * @return: ESP_OK on success and ESP_FAIL on fail
 */
esp_err_t openport(spi_t *spi, spi_device_interface_config_t *devcfg)
{
    #if CONFIG_HSPI_HOST
        spi->host = HSPI_HOST;
    #elif CONFIG_VSPI_HOST
        spi->host = VSPI_HOST;
    #endif
    spi->ret = spi_bus_add_device(spi->host, devcfg, &spi->spi_handle);
    assert(spi->ret == ESP_OK);
    return spi->ret;
}

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
esp_err_t closeport(spi_t *spi)
{
    spi->ret = spi_bus_remove_device(spi->spi_handle);
    assert(spi->ret == ESP_OK);
    // spi->ret = spi_bus_free(spi->host);
    // assert(spi->ret == ESP_OK);
    return spi->ret;
}

/*
 * @brief: write content to device attached through spi port.
 * @spi: the all of SPI.
 * @data: the one byte data waitted to be transmitted. This parameter will be used if mode = 0.
 * @uservarabile: User-defined variable. Can be used to store eg transaction ID.
 */
/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
esp_err_t write_byte(spi_t *spi, uint8_t data, void * uservarabile, uint16_t cmd, uint64_t addr)
{
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.flags = SPI_TRANS_USE_TXDATA;
    t.cmd = cmd;
    t.addr = addr;
    t.length = TOTAL_DATA_LENGTH(1);  // total data length
    t.user = (void*)uservarabile;
    t.tx_data[0] = data;
    #if CONFIG_POLLING_TRANSMIT
        spi->ret = spi_device_polling_transmit(spi->spi_handle, &t); // Transmit!
    #elif CONFIG_INTERRUPT_TRANSMIT
        spi->ret = spi_device_transmit(spi->spi_handle, &t);  
    #endif  
    assert(spi->ret == ESP_OK);
    return spi->ret;
}

/*
 * @brief: write content to device attached through spi port.
 * @spi: the all of SPI.
 * @buffer: the data buffer waitted to be transmitted. This parameter will be used if mode = 1.
 * @len: the length of buffer.
 * @uservarabile: User-defined variable. Can be used to store eg transaction ID. if in oled, this parameter could be cmd or data.
 * @cmd: if set the command bits, cmd will be the command data. And this parameter will be the first data to be transmitted.
 * @addr: if set the address bits, addr will be the address data. And this parameter will be the second data to be transmitted.
 */
/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
esp_err_t write_buff(spi_t *spi, uint8_t *buffer, size_t len, void *uservarabile, uint16_t cmd, uint64_t addr)
{
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    if (len == 0) return ESP_FAIL;
    t.cmd = cmd;
    t.addr = addr;
    t.length = TOTAL_DATA_LENGTH(len);
    t.tx_buffer = buffer;
    t.user = uservarabile;
    #if CONFIG_POLLING_TRANSMIT
        spi->ret = spi_device_polling_transmit(spi->spi_handle, &t); // Transmit! Transaction are atomic.
    #elif CONFIG_INTERRUPT_TRANSMIT
        spi->ret = spi_device_transmit(spi->spi_handle, &t);
    #endif
    assert(spi->ret == ESP_OK);
    return spi->ret;
}

/*
 * later process
 */
void read_byte(void)
{

}

void read_buff(void)
{

}