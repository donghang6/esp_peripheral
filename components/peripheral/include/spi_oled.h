/*
 * @Descripttion: 
 * @version: 
 * @Author: donghang
 * @Date: 2019-08-04 21:56:33
 * @LastEditors: donghang
 * @LastEditTime: 2019-08-15 23:53:27
 */
#ifndef SPI_OLED_H_
#define SPI_OLED_H_
#include "sdkconfig.h"
#include "spi.h"

#define COMMAND (void*)0
#define DATA (void*)1

/**
 * @brief: to power up oled
 * @param {type} 
 * @return: 
 */
esp_err_t oled_power_up(spi_t *spi);

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
esp_err_t oled_entering_sleep_mode(spi_t *spi);

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
esp_err_t oled_exiting_sleep_mode(spi_t *spi);

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
esp_err_t oled_reset(spi_t *spi);

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
esp_err_t oled_clear(spi_t *spi);

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
esp_err_t oled_show_string(spi_t *spi, uint8_t page, uint8_t segment, char* str);

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
esp_err_t oled_show_number(spi_t *spi, uint8_t page, uint8_t segment, float num);

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
void oled_right_horizontal_scroll(spi_t *spi, uint8_t start_page, uint8_t end_page, uint8_t interval);

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
void oled_left_horizontal_scroll(spi_t *spi, uint8_t start_page, uint8_t end_page, uint8_t interval);

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
void oled_exit_scroll(spi_t *spi);
#endif