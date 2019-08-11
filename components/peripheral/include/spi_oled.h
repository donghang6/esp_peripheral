#include "sdkconfig.h"
#include "spi.h"

#ifndef SPI_OLED_H_
#define SPI_OLED_H_

#define COMMAND 0
#define DATA 1

esp_err_t oled_init(spi_t *spi);
esp_err_t oled_write_buf(spi_t *spi, uint8_t *buffer, size_t len, void *uservarabile);
esp_err_t oled_write_byte(spi_t *spi, uint8_t data, void * uservarabile);
esp_err_t oled_clear(spi_t *spi);
void oled_reset();

#endif