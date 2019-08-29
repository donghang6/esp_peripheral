/*
 * @Descripttion: 
 * @version: 
 * @Author: donghang
 * @Date: 2019-08-27 22:58:30
 * @LastEditors: donghang
 * @LastEditTime: 2019-08-29 21:34:04
 */
#ifndef WS2812B_H_
#define WS2812B_H
#include "spi.h"
esp_err_t ws2812b_light(spi_t *spi, uint8_t count, uint32_t color);
#endif