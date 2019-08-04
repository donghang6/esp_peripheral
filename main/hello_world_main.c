/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "spi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main()
{
    esp_err_t ret = ESP_FAIL;
    spi_t spi;
    spi_bus_config_t buscfg = {
        .miso_io_num = 25,
        .mosi_io_num = 23,
        .sclk_io_num = 19,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 1000,
    };

    spi_device_interface_config_t devcfg  = {
        .clock_speed_hz=10*1000*1000,
        .mode = 0,
        .spics_io_num = 19,
        .queue_size = 7,
        .pre_cb = NULL,
    };
    // //Initialize the SPI bus
    // ret=spi_bus_initialize(HSPI_HOST, &buscfg, 1);
    // ESP_ERROR_CHECK(ret);
    // //Attach the LCD to the SPI bus
    // ret=spi_bus_add_device(HSPI_HOST, &devcfg, &spi.spi_handle);
    // ESP_ERROR_CHECK(ret);
    spi.host = HSPI_HOST;
    spi.dma_chan = 1;
    spi.buscfg = buscfg;
    spi.devcfg = devcfg;
    ret = open(&spi);
    assert(ret == ESP_OK);
    printf("Initial SPI OK!\n");
    uint8_t data = 170;
    for(;;) {
        ret = write(&spi, NULL, 0, data, TRANSMIT_A_BYTE, 0);
        assert(ret == ESP_OK);
        printf("Transmit success!\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
