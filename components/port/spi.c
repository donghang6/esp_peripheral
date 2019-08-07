#include  "spi.h"

#define ONE_BYTE_LENGTH 8
#define TOTAL_DATA_LENGTH(length) length*ONE_BYTE_LENGTH

spi_bus_config_t buscfg = {
    .miso_io_num = CONFIG_MISO_IO_NUM,
    .mosi_io_num = CONFIG_MOSI_IO_NUM,
    .sclk_io_num = CONFIG_SCLK_IO_NUM,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1,
    .max_transfer_sz = CONFIG_MAX_TRANSFER_SZ,
};

spi_device_interface_config_t devcfg  = {
    .command_bits = CONFIG_COMMAND_BITS,
    .address_bits = CONFIG_ADDRESS_BITS,
    .clock_speed_hz = CONFIG_CLOCK_SPEED_HZ,
    .mode = CONFIG_MODE,
    .spics_io_num = CONFIG_SPICS_IO_NUM,
    .queue_size = CONFIG_QUEUE_SIZE,
    .pre_cb = NULL,
};

/*
 * funtion: open a spi port
 * @spi: the all of SPI
 */
esp_err_t open(spi_t *spi)
{
    #if CONFIG_HSPI_HOST
        spi->host = HSPI_HOST;
    #elif CONFIG_VSPI_HOST
        spi->host = VSPI_HOST;
    #endif
    spi->ret = spi_bus_initialize(spi->host, &buscfg, CONFIG_DMA_CHAN);
    printf("spi_bus_initialize\r\n");
    assert(spi->ret == ESP_OK);
    spi->ret = spi_bus_add_device(spi->host, &devcfg, &spi->spi_handle);
    printf("spi_bus_add_device\r\n");
    assert(spi->ret == ESP_OK);
    return spi->ret;
}

esp_err_t close(spi_t *spi)
{
    esp_err_t ret = ESP_FAIL;
    spi->ret = spi_bus_remove_device(spi->spi_handle);
    assert(spi->ret == ESP_OK);
    spi->ret = spi_bus_free(spi->host);
    assert(spi->ret == ESP_OK);
    ret = ESP_OK;
    return ret;
}

/*
 * function: write content to device attached through spi port.
 * @spi: the all of SPI.
 * @data: the one byte data waitted to be transmitted. This parameter will be used if mode = 0.
 * @uservarabile: User-defined variable. Can be used to store eg transaction ID.
 */
esp_err_t write_byte(spi_t *spi, uint8_t data, void * uservarabile)
{
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    printf("transmit a byte\n");
    printf("start transmit a byte\n");
    t.flags = SPI_TRANS_USE_TXDATA;
    t.length = TOTAL_DATA_LENGTH(1);  // total data length
    t.user = (void*)uservarabile;
    t.tx_data[0] = data;
    spi->ret = spi_device_polling_transmit(spi->spi_handle, &t); // Transmit!
    assert(spi->ret == ESP_OK);
    return spi->ret;
}

/*
 * function: write content to device attached through spi port.
 * @spi: the all of SPI.
 * @buffer: the data buffer waitted to be transmitted. This parameter will be used if mode = 1.
 * @len: the length of buffer.
 * @uservarabile: User-defined variable. Can be used to store eg transaction ID. if in oled, this parameter could be cmd or data.
 * @cmd: if set the command bits, cmd will be the command data. And this parameter will be the first data to be transmitted.
 * @addr: if set the address bits, addr will be the address data. And this parameter will be the second data to be transmitted.
 */
esp_err_t write_buff(spi_t *spi, uint8_t *buffer, size_t len, void *uservarabile, uint16_t cmd, uint64_t addr)
{
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    printf("transmit bytes\n");
    if (len == 0) return ESP_FAIL;
    t.cmd = cmd;
    t.addr = addr;
    t.length = TOTAL_DATA_LENGTH(len);
    t.tx_buffer = buffer;
    t.user = uservarabile;
    spi->ret = spi_device_polling_transmit(spi->spi_handle, &t); // Transmit!
    assert(spi->ret == ESP_OK);
    return spi->ret;
}

/*
 * later process
 */
void read(void)
{

}
