#include  "spi.h"

/*
 * funtion: open a spi port
 * @spi: the all of SPI
 */
esp_err_t open(spi_t *spi)
{
    spi->ret = spi_bus_initialize(spi->host, &spi->buscfg, spi->dma_chan);
    printf("spi_bus_initialize\r\n");
    assert(spi->ret == ESP_OK);
    spi->ret = spi_bus_add_device(spi->host, &spi->devcfg, &spi->spi_handle);
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
 * @buffer: the data buffer waitted to be transmitted. This parameter will be used if mode = 1.
 * @len: the length of buffer.
 * @data: the one byte data waitted to be transmitted. This parameter will be used if mode = 0.
 * @mode: choose to write a bytes or bytes via it. 0 is a byte and 1 is bytes.
 * @uservarabile: User-defined variable. Can be used to store eg transaction ID.
 */
esp_err_t write(spi_t *spi, uint8_t *buffer, int len, uint8_t data, int mode, uint8_t uservarabile)
{
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    if (mode == TRANSMIT_BYTES) {
        printf("transmit bytes\n");
        if (len == 0) return ESP_FAIL;
        t.length = len*8;
        t.tx_buffer = buffer;
        t.user = (void*)uservarabile;
        spi->ret = spi_device_polling_transmit(spi->spi_handle, &t); // Transmit!
        assert(spi->ret == ESP_OK);
        return spi->ret;
    } else if (mode == TRANSMIT_A_BYTE) {
        printf("transmit a byte\n");
        printf("start transmit a byte\n");
        t.flags = SPI_TRANS_USE_TXDATA;
        t.length = 8;
        t.user = (void*)uservarabile;
        t.tx_data[0] = data;
        spi->ret = spi_device_polling_transmit(spi->spi_handle, &t); // Transmit!
        assert(spi->ret == ESP_OK);
        return spi->ret;
    } else {
        return ESP_FAIL;
        // later process
    }
}

/*
 * later process
 */
void read(void)
{

}