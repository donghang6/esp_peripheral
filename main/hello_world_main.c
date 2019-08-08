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

// 添加中断方法去写 这个要加在 Kconfig 中

void app_main()
{
    esp_err_t ret = ESP_FAIL;
    spi_t spi;
    ret = open(&spi);
    assert(ret == ESP_OK);
    printf("Initial SPI OK!\n");
    uint8_t data = 0;
    uint8_t buffer[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for(;;) {
        ret = write_byte(&spi, data, 0, 0, 0);
        assert(ret == ESP_OK);
        printf("Transmit success!\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        data++;
    }
}
