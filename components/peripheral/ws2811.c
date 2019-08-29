/*
 * @Descripttion: 
 * @version: 
 * @Author: donghang
 * @Date: 2019-08-29 08:12:27
 * @LastEditors: donghang
 * @LastEditTime: 2019-08-29 22:10:41
 */
#include "ws2811.h"
#include "color.h"
#include <string.h>

#define CODE0 0xF000
#define CODE1 0xFF00

static esp_err_t reset(spi_t *spi);
/**
 * @brief: the ws2811 interface config
 */
spi_device_interface_config_t ws2811cfg  = {
    .clock_speed_hz = 13000000,
    .mode = CONFIG_MODE,
    .spics_io_num = CONFIG_SPICS_IO_NUM_OLED,
    .queue_size = CONFIG_QUEUE_SIZE,
};

/**
 * @brief: write buffer into ws2811 
 * @param: `spi` the handle of SPI
 *         `buffer` the buffer of data
 *         `len` the length of buffer
 * @return: ESP_OK on success
 *          ESP_FAIL on fail
 */
static esp_err_t ws2811_write_buf(spi_t *spi, uint8_t *buffer, size_t len)
{
    return read_write_buff(spi, buffer, len, NULL, 0, 0, NULL);
}


/**
 * @brief: remove ws2811 from spi bus
 * @param: `spi` the handle of SPI
 * @return: ESP_OK success
 *          ESP_FAIL fail
 */
static esp_err_t ws2811_close(spi_t *spi)
{
    return closeport(spi);
}

/**
 * @brief: add ws2811 from spi bus
 * @param: `spi` the handle of SPI
 *         `devcfg` the pointer of spi_device_interface_config_t
 * @return: ESP_OK success
 *          ESP_FAIL fail
 */
static esp_err_t ws2811_open(spi_t *spi)
{
    return openport(spi, &ws2811cfg);
}

/**
 * @brief: convert color(for eg. 0x00FF00) to buffer(for eg. CODE0 CODE1 ……)
 * @param: `converted` the buffer has been converted 
 *         `count` the count of light
 *         `color` the color want to show 
 * @return: none
 */
static void convert_color(uint16_t* converted, uint8_t count, uint32_t color)
{
    for(int i = 0; i < 24*count; i++) {
        converted[i] = CODE0;
    }
    uint32_t red   = color & 0xFF0000;
    uint32_t green = color & 0x00FF00;
    uint32_t blue  = color & 0x0000FF;    
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
 * @brief: light the ws2811 of count, the order is GRB
 * @param: `spi` the handle of SPI
 *         `count` the count of ws2811
 *         `buffer` the color buffer
 * @return: ESP_OK success
 *          ESP_FAIL fail
 */
esp_err_t ws2811_light(spi_t *spi, uint8_t count, uint32_t color)
{
    uint16_t* buffer = malloc(sizeof(uint16_t)*count*24);
    uint8_t* line = malloc(sizeof(uint16_t)*count*24);
    spi->ret = ws2811_open(spi);
    assert(spi->ret == ESP_OK);
    convert_color(buffer, count, color);
    for(int i = 0; i < count*24; i++) {
        line[2*i] = (buffer[i] >> 8) & 0xFF;
        line[2*i+1] = buffer[i] & 0xFF;
    }
    for(int i = 0; i < 48; i++) {
        printf("%x\n", line[i]);
    }
    spi->ret = ws2811_write_buf(spi, line, count*24*2);
    assert(spi->ret == ESP_OK);
    spi->ret = reset(spi);
    assert(spi->ret == ESP_OK);
    spi->ret = ws2811_close(spi);
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
    uint8_t *reset_buffer = malloc(sizeof(uint8_t)*(16+488+16)); // 16 0xFF 488 0 16 0xFF 300um
    memset(reset_buffer, 0xFF, 16);
    memset(reset_buffer+16, 0, 488);
    memset(reset_buffer+16+488, 0xFF, 16);
    spi->ret = ws2811_write_buf(spi, reset_buffer, 16+488+16);
    assert(spi->ret == ESP_OK);
    return spi->ret;
}