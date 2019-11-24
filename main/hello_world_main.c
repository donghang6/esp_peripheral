/*
 * @Descripttion: 
 * @version: 
 * @Author: donghang
 * @Date: 2019-08-04 21:56:33
 * @LastEditors: donghang
 * @LastEditTime: 2019-08-29 22:38:00
 */
#include <stdio.h>
#include "spi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "spi_oled.h"
#include "ws28xx.h"
#include "color.h"

/*
 * if this contructure couldn't relate to real device(eg. OLED), 
 * it's have no meaning. Because it's diffierent to each device.
 */
void app_main()
{
    esp_err_t ret = ESP_FAIL;
    spi_t spi;
    ret = spi_bus_init(&spi); // initialize spi bus
    assert(ret == ESP_OK);

    // oled_power_up_auto(&spi);
    // oled_clear_auto(&spi);
    // oled_show_number_auto(&spi, 0, 0, 11.01);
    // oled_show_string_auto(&spi, 4, 0, "abcdefghigklmnopqrstuvwxyz");

    for(;;)
    {
        printf("\n");
        printf("\nHello World\n");
        vTaskDelay(1000 / portTICK_RATE_MS );
        ws28xx_light(&spi, 10, GREEN_APPLE);
        vTaskDelay(1000 / portTICK_RATE_MS );
        ws28xx_light(&spi, 10, GRAY);
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