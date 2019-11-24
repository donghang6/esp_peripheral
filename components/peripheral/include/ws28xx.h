/*
 * @Descripttion: 
 * @version: 
 * @Author: donghang
 * @Date: 2019-08-29 08:12:21
 * @LastEditors: donghang
 * @LastEditTime: 2019-08-29 08:31:03
 */
#ifndef WS2811_H_
#define WS2811_H
#include "spi.h"
esp_err_t ws28xx_light(spi_t *spi, uint8_t count, uint32_t color);;
#endif