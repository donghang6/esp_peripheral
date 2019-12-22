#ifndef LCD_H_
#define LCD_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "esp_system.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "spi.h"

#define LCD_HOST    HSPI_HOST
#define DMA_CHAN    2

#define PIN_NUM_MISO 25
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK  19
#define PIN_NUM_CS   22

#define PIN_NUM_DC   21
#define PIN_NUM_RST  18
#define PIN_NUM_BCKL 5

#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE  0xFC00
#define GREEN 0x003F
#define RED	  0x03F0

#define PARALLEL_LINES 16
#define CROSS_LINES 320
/*
 The LCD needs a bunch of command/argument values to be initialized. They are stored in this struct.
*/
typedef struct {
    uint8_t cmd;
    uint8_t data[16];
    uint8_t databytes; //No of data in data; bit 7 = delay after set; 0xFF = end of cmds.
} lcd_init_cmd_t;

void lcd_init(spi_t *spi);
uint32_t lcd_get_id(spi_device_handle_t spi);
void lcd_spi_pre_transfer_callback(spi_transaction_t *t);
void lcd_data(spi_device_handle_t spi, const uint8_t *data, int len);
void lcd_cmd(spi_device_handle_t spi, const uint8_t cmd);
void lcd_show_char(spi_t *spi, int xpos, int ypos, char c, uint16_t color);
void lcd_clean(spi_t *spi, uint16_t color);

#endif // LEC_H_
