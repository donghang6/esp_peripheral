/*
 * @Descripttion: 
 * @version: 
 * @Author: donghang
 * @Date: 2019-08-04 21:56:33
 * @LastEditors: donghang
 * @LastEditTime: 2019-08-24 06:44:10
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
    spi_t spi;
    ret = spi_bus_init(&spi); // initialize spi bus
    assert(ret == ESP_OK);
    W25QXX_Init_auto(&spi);
    W25QXX_Write_auto(&spi, bf, 0, 20);
    W25QXX_Read_auto(&spi, rc, 0, 20);

    oled_power_up_auto(&spi);
    oled_clear_auto(&spi);
    sdmmc_card_t* card;
    sdio_tfcard_init(&card);
    oled_show_number_auto(&spi, 0, 0, 11.01);
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
//     oled_power_up_auto(&oled_spi);
//     oled_clear_auto(&oled_spi);
//     sdmmc_card_t* card;
//     sdio_tfcard_init(&card);
//     oled_show_number_auto(&oled_spi, 0, 0, 11.01);
//     for(;;)
//     {
//         printf("\n");
//         printf("\nHello World\n");
//         vTaskDelay(1000 / portTICK_RATE_MS );
//     }
// }