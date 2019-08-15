/*
 * @Descripttion: 
 * @version: 
 * @Author: donghang
 * @Date: 2019-08-04 21:56:33
 * @LastEditors: donghang
 * @LastEditTime: 2019-08-15 23:51:47
 */
#include <stdio.h>
#include "spi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "spi_oled.h"
#include "sdio_tfcard.h"
/*
 * if this contructure couldn't relate to real device(eg. OLED), 
 * it's have no meaning. Because it's diffierent to each device.
 */

void app_main()
{
    esp_err_t ret = ESP_FAIL;
    spi_t oled_spi;
    ret = spi_bus_init(&oled_spi); // initialize spi bus
    assert(ret == ESP_OK);
    ret = oled_power_up(&oled_spi);
    assert(ret == ESP_OK);
    ret = oled_clear(&oled_spi);
    assert(ret == ESP_OK);
    sdmmc_card_t* card;
    sdio_tfcard_init(&card);
    // oled_show_string(&oled_spi, 0, 0, "董航乐鑫合肥安徽安庆1234567890更明确的说是指申请内存空间时还不知道用户是用这段空间来存储什么类型的数据");
    // oled_left_horizontal_scroll(&oled_spi, 5, 7, 0);
    oled_show_number(&oled_spi, 0, 0, 11.01);
    for(;;)
    {
        printf("\n");
        printf("\nHello World\n");
        vTaskDelay(1000 / portTICK_RATE_MS );
    }
}