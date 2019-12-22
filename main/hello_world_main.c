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
#include "lcd.h"
/*
 * if this contructure couldn't relate to real device(eg. OLED), 
 * it's have no meaning. Because it's diffierent to each device.
 */

void app_main()
{
    esp_err_t ret = ESP_FAIL;
    spi_t spi;
    ret = spi_bus_init(&spi);
    ESP_ERROR_CHECK(ret);
    lcd_init(&spi);
    lcd_clean(&spi, WHITE);
    lcd_show_char(&spi, 1, 1, '1', BLACK);
    for(;;)
    {
        printf("\n");
        printf("\nHello World\n");
        vTaskDelay(1000 / portTICK_RATE_MS );
    }
}
