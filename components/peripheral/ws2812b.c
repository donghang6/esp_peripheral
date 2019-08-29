/*
 * @Descripttion: 
 * @version: 
 * @Author: donghang
 * @Date: 2019-08-27 22:58:48
 * @LastEditors: donghang
 * @LastEditTime: 2019-08-29 22:21:18
 */
#include "ws2812b.h"
#include "color.h"
#include <string.h>

#define CODE0 0x80
#define CODE1 0xF8

static esp_err_t reset(spi_t *spi);

/**
 * @brief: the ws2812b interface config
 */
spi_device_interface_config_t ws2812bcfg  = {
    .clock_speed_hz = 9000000,
    .mode = CONFIG_MODE,
    .spics_io_num = CONFIG_SPICS_IO_NUM_OLED,
    .queue_size = CONFIG_QUEUE_SIZE,
};

/**
 * @brief: write buffer into ws2812b 
 * @param: `spi` the handle of SPI
 *         `buffer` the buffer of data
 *         `len` the length of buffer
 * @return: ESP_OK on success
 *          ESP_FAIL on fail
 */
static esp_err_t ws2812b_write_buf(spi_t *spi, uint8_t *buffer, size_t len)
{
    return read_write_buff(spi, buffer, len, NULL, 0, 0, NULL);
}


/**
 * @brief: remove ws2812b from spi bus
 * @param: `spi` the handle of SPI
 * @return: ESP_OK success
 *          ESP_FAIL fail
 */
static esp_err_t ws2812b_close(spi_t *spi)
{
    return closeport(spi);
}

/**
 * @brief: add ws2812b from spi bus
 * @param: `spi` the handle of SPI
 *         `devcfg` the pointer of spi_device_interface_config_t
 * @return: ESP_OK success
 *          ESP_FAIL fail
 */
static esp_err_t ws2812b_open(spi_t *spi)
{
    return openport(spi, &ws2812bcfg);
}

/**
 * @brief: convert color(for eg. 0x00FF00) to buffer(for eg. CODE0 CODE1 ……)
 * @param: `converted` the buffer has been converted 
 *         `count` the count of light
 *         `color` the color want to show 
 * @return: none
 */
static void convert_color(uint8_t* converted, uint8_t count, uint32_t color)
{
    memset(converted, CODE0, count*24);
    uint32_t red = color & 0xFF0000;
    uint32_t green = color & 0x00FF00;
    uint32_t blue = color & 0x0000FF;    
    uint32_t temp_color = (green << 8) | (red >> 8) | blue;
    for(int cur_light = 0; cur_light < count; cur_light++) {
        uint32_t shift = 0x800000;
        for(int bit = 0; bit < 24; bit++) {
            if(temp_color & shift) {
                converted[cur_light*24+bit] = CODE1;
            }
            shift >>= 1;
        }
        shift = 0x800000;
    }
}
/**
 * @brief: light the ws2812b of count, the order is GRB
 * @param: `spi` the handle of SPI
 *         `count` the count of ws2812b
 *         `buffer` the color buffer
 * @return: ESP_OK success
 *          ESP_FAIL fail
 */
esp_err_t ws2812b_light(spi_t *spi, uint8_t count, uint32_t color)
{
    uint8_t* buffer = malloc(sizeof(uint8_t)*count*24);
    spi->ret = ws2812b_open(spi);
    assert(spi->ret == ESP_OK);
    convert_color(buffer, count, color);
    spi->ret = ws2812b_write_buf(spi, buffer, count*24);
    assert(spi->ret == ESP_OK);
    spi->ret = reset(spi);
    assert(spi->ret == ESP_OK);
    spi->ret = ws2812b_close(spi);
    assert(spi->ret == ESP_OK);
    free(buffer);
    return spi->ret;
}

/**
 * @brief: reset time between two order
 * @param: `spi` the handle of SPI 
 * @return: ESP_OK success
 *          ESP_FAIL fail
 */
static esp_err_t reset(spi_t *spi)
{
    uint8_t *reset_buffer = malloc(sizeof(uint8_t)*(10+60+10)); // 16 0xFF 488 0 16 0xFF 300um
    memset(reset_buffer, 0xFF, 10);
    memset(reset_buffer+10, 0, 60);
    memset(reset_buffer+10+60, 0xFF, 10);
    spi->ret = ws2812b_write_buf(spi, reset_buffer, 10+60+10);
    assert(spi->ret == ESP_OK);
    return spi->ret;
}