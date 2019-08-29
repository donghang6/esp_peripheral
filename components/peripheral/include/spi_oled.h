/*
 * @Descripttion: 
 * @version: 
 * @Author: donghang
 * @Date: 2019-08-04 21:56:33
 * @LastEditors: donghang
 * @LastEditTime: 2019-08-26 21:27:18
 */
#ifndef SPI_OLED_H_
#define SPI_OLED_H_
#include "sdkconfig.h"
#include "spi.h"

#define COMMAND (void*)0
#define DATA (void*)1

/**
 * @brief: config oled and power up it
 * @param: `spi` the handle of SPI
 * @return: ESP_OK on success
 *          ESP_FAIL on fail
 */
esp_err_t oled_power_up(spi_t *spi);

/**
 * @brief: oled enter the sleep mode
 * @param: `spi` the handle of SPI 
 * @return: ESP_OK on success
 *          ESP_FAIL on fail
 */
esp_err_t oled_entering_sleep_mode(spi_t *spi);

/**
 * @brief: oled exit the sleep mode
 * @param: `spi` the handle of SPI 
 * @return: ESP_OK on success
 *          ESP_FAIL on fail
 */
esp_err_t oled_exiting_sleep_mode(spi_t *spi);

/**
 * @brief: reset th oled
 * @param: `spi` the handle of SPI
 * @return: ESP_OK on success
 *          ESP_FAIL on fail
 */
esp_err_t oled_reset(spi_t *spi);

/**
 * @brief: clear the oled
 * @param: `spi` the handle of SPI
 * @return: ESP_OK on success
 *          ESP_FAIL on fail
 */
esp_err_t oled_clear(spi_t *spi);

/**
 * @brief: show string
 * @param: `spi` the handle of SPI
 *         `page` choose the page(0-7)
 *         `segment` choose the segment(0-127)
 *         `str` the string to be showed in oled, could be chinese or ascii 
 * @return: ESP_OK success
 *          ESP_FAIL fail
 */
esp_err_t oled_show_string(spi_t *spi, uint8_t page, uint8_t segment, char* str);

/**
 * @brief: show number
 * @param: `spi` the handle of SPI
 *         `page` choose the page(0-7)
 *         `segment` choose the segment(0-127)
 *         `num` the number to be showed in oled, could be float or int
 * @return: ESP_OK success
 *          ESP_FAIL fail
 */
esp_err_t oled_show_number(spi_t *spi, uint8_t page, uint8_t segment, float num);

/**
 * @brief: horizontal_scroll from left to right
 * @param: `spi` the handle of SPI
 *         `start_page` the start page(0-7)
 *         `end_page` the end page(0-7)
 *         `interval` the frame 
 * @return: none
 */
void oled_right_horizontal_scroll(spi_t *spi, uint8_t start_page, uint8_t end_page, uint8_t interval);

/**
 * @brief: horizontal_scroll from right to left
 * @param: `spi` the handle of SPI
 *         `start_page` the start page(0-7)
 *         `end_page` the end page(0-7)
 *         `interval` the frame 
 * @return: none
 */
void oled_left_horizontal_scroll(spi_t *spi, uint8_t start_page, uint8_t end_page, uint8_t interval);

/**
 * @brief: exit the scroll mode
 * @param: `spi` the handle of SPI
 * @return: none
 */
void oled_exit_scroll(spi_t *spi);

/**
 * @brief: remove oled from spi bus
 * @param: `spi` the handle of SPI
 * @return: ESP_OK success
 *          ESP_FAIL fail
 */
esp_err_t oled_close(spi_t *spi);

/**
 * @brief: add oled from spi bus
 * @param: `spi` the handle of SPI
 *         `devcfg` the pointer of spi_device_interface_config_t
 * @return: ESP_OK success
 *          ESP_FAIL fail
 */
esp_err_t oled_open(spi_t *spi);

/**
 * @brief: config oled and power up it and anto attach with SPI bus
 * @param: `spi` the handle of SPI
 * @return: none
 */
void oled_power_up_auto(spi_t *spi);

/**
 * @brief: clear the oled and auto attach and deattach with SPU bus
 * @param: `spi` the handle of SPI
 * @return: none
 */
void oled_clear_auto(spi_t *spi);

/**
 * @brief: show number
 * @param: `spi` the handle of SPI
 *         `page` choose the page(0-7)
 *         `segment` choose the segment(0-127)
 *         `num` the number to be showed in oled, could be float or int
 * @return: ESP_OK success
 *          ESP_FAIL fail
 */
void oled_show_number_auto(spi_t *spi, uint8_t page, uint8_t segment, float num);

/**
 * @brief: show string
 * @param: `spi` the handle of SPI
 *         `page` choose the page(0-7)
 *         `segment` choose the segment(0-127)
 *         `str` the string to be showed in oled, could be chinese or ascii 
 * @return: ESP_OK success
 *          ESP_FAIL fail
 */
void oled_show_string_auto(spi_t *spi, uint8_t page, uint8_t segment, char* str);
#endif