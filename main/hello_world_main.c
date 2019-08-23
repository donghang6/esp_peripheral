/*
 * @Descripttion: 
 * @version: 
 * @Author: donghang
 * @Date: 2019-08-04 21:56:33
 * @LastEditors: donghang
 * @LastEditTime: 2019-08-23 08:24:14
 */
#include <stdio.h>
#include "spi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "spi_oled.h"
#include "sdio_tfcard.h"
#include "w25qxx.h"

/*
 * if this contructure couldn't relate to real device(eg. OLED), 
 * it's have no meaning. Because it's diffierent to each device.
 */
void app_main()
{
    uint8_t bf[20] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    uint8_t rc[20] = {};
    esp_err_t ret = ESP_FAIL;
    spi_t w25qxx_spi;
    ret = spi_bus_init(&w25qxx_spi); // initialize spi bus
    assert(ret == ESP_OK);
    ret = W25QXX_Init(&w25qxx_spi);
    assert(ret == ESP_OK);
    ret = W25QXX_Write(&w25qxx_spi, bf, 0, 20);
    assert(ret == ESP_OK);
    ret = W25QXX_Read(&w25qxx_spi, rc, 0, 20);
    assert(ret == ESP_OK);
    for(;;)
    {
        printf("\n");
        printf("\nHello World\n");
        vTaskDelay(1000 / portTICK_RATE_MS );
    }
}

// void app_main()
// {
//     esp_err_t ret = ESP_FAIL;
//     spi_t oled_spi;
//     ret = spi_bus_init(&oled_spi); // initialize spi bus
//     assert(ret == ESP_OK);
//     ret = oled_power_up(&oled_spi);
//     assert(ret == ESP_OK);
//     ret = oled_clear(&oled_spi);
//     assert(ret == ESP_OK);
//     sdmmc_card_t* card;
//     sdio_tfcard_init(&card);
//     oled_show_number(&oled_spi, 0, 0, 11.01);
//     for(;;)
//     {
//         printf("\n");
//         printf("\nHello World\n");
//         vTaskDelay(1000 / portTICK_RATE_MS );
//     }
// }