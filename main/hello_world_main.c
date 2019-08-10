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
#include "spi_oled.h"
/*
 * if this contructure couldn't relate to real device(eg. OLED), 
 * it's have no meaning. Because it's diffierent to each device.
 */
// slove the problem of CS and DMA 是否可以采用注册的方式接收不同 SPI 从设备的数据
// 先采用两个设备试一试
// 将总线与具体的设备分开，总线保持。每个设备注册自己的 spi_device_interface_config_t

void app_main()
{
    esp_err_t ret = ESP_FAIL;
    spi_t spi;
    ret = spi_bus_init(&spi); // initialize spi bus
    assert(ret == ESP_OK);
    ret = oled_init(&spi);
    assert(ret == ESP_OK);
    ret = oled_clear(&spi);
    assert(ret == ESP_OK);
    for(;;) {

    }
}
