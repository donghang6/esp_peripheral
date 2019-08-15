/*
 * @Descripttion: the driver for 0.96 inch oled
 * @version: v1.0
 * @Author: donghang
 * @Date: 2019-08-04 21:56:33
 * @LastEditors: donghang
 * @LastEditTime: 2019-08-15 23:56:58
 */
#include "spi_oled.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "font.h"

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
void lcd_spi_pre_transfer_callback(spi_transaction_t *t)
{
    int dc=(int)t->user;
    gpio_set_level(CONFIG_IO_DC, dc);
}

/**
 * @brief: 
 */
spi_device_interface_config_t oledcfg  = {
    .command_bits = CONFIG_COMMAND_BITS,
    .address_bits = CONFIG_ADDRESS_BITS,
    .clock_speed_hz = CONFIG_CLOCK_SPEED_HZ,
    .mode = CONFIG_MODE,
    .spics_io_num = CONFIG_SPICS_IO_NUM,
    .queue_size = CONFIG_QUEUE_SIZE,
    .pre_cb = lcd_spi_pre_transfer_callback,
};

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
static esp_err_t oled_gpio_config(gpio_config_t *oled_io_conf)
{
    esp_err_t ret = ESP_FAIL;
    oled_io_conf->pin_bit_mask = (1ULL << CONFIG_IO_RES) | (1ULL << CONFIG_IO_DC);   
    oled_io_conf->pull_down_en = GPIO_PULLDOWN_ENABLE;
    oled_io_conf->mode = GPIO_MODE_OUTPUT;
    ret = gpio_config(oled_io_conf);
    return ret;
}

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
static esp_err_t oled_write_buf(spi_t *spi, uint8_t *buffer, size_t len, void *uservarabile)
{
    return write_buff(spi, buffer, len, uservarabile, 0, 0);
}

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
static esp_err_t oled_write_byte(spi_t *spi, uint8_t data, void * uservarabile)
{
    return write_byte(spi, data, uservarabile, 0, 0);
}

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
esp_err_t oled_power_up(spi_t *spi)
{
    esp_err_t ret = ESP_FAIL;
    uint8_t init_cmd[28] = {};
    gpio_config_t oled_io_conf;

    memset(&oled_io_conf, 0, sizeof(gpio_config_t));
    ret = oled_gpio_config(&oled_io_conf);
    assert(ret == ESP_OK);
    ret = openport(spi, &oledcfg);
    assert(ret ==ESP_OK);

    init_cmd[0]  = 0xAE;  //---turn off oled panel
    init_cmd[1]  = 0x00;  //---set low column address
    init_cmd[2]  = 0x10;  //---set high column address
    init_cmd[3]  = 0x40;  //---set start line address  Set 'Mapping RAM' Display Start Line (0x00~0x3F)
    init_cmd[4]  = 0x81;  //---set contrast control register. reset is 7Fh
    init_cmd[5]  = 0xCF;  // --Set SEG Output Current Brightness
    init_cmd[6]  = 0xA1;  //---Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    init_cmd[7]  = 0xC8;  //---Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    init_cmd[8]  = 0xA6;  //---set normal display
    init_cmd[9]  = 0xA8;  //---set multiplex ratio(1 to 64) use 64 rows of display
    init_cmd[10] = 0x3F;  // --1/64 duty
    init_cmd[11] = 0xD3;  //---set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    init_cmd[12] = 0x00;  // --not offset
    init_cmd[13] = 0xD5;  //---set display clock divide ratio/oscillator frequency
    init_cmd[14] = 0x80;  // --set divide ratio, Set Clock as 100 Frames/Sec
    init_cmd[15] = 0xD9;  //---set pre-charge period
    init_cmd[16] = 0xF1;  // --Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    init_cmd[17] = 0xDA;  //---set com pins hardware configuration
    init_cmd[18] = 0x12;  // --alternative COM pin configuration, disable COM Left/Right remap
    init_cmd[19] = 0xDB;  //---set vcomh
    init_cmd[20] = 0x40;  // --Set VCOM Deselect Level
    init_cmd[21] = 0x20;  //---Set Page Addressing Mode (0x00/0x01/0x02) *
    init_cmd[22] = 0x02;  // --paging addressing mode                    *
    init_cmd[23] = 0x8D;  //---set Charge Pump enable/disable
    init_cmd[24] = 0x14;  // --set(0x10) disable, 0x14 is able. set 0x10 to turn off the oled
    init_cmd[25] = 0xA4;  //---Disable Entire Display On (0xa4/0xa5)
    init_cmd[26] = 0xA6;  // --Disable Inverse Display On (0xa6/a7) 
    // dalay
    init_cmd[27] = 0xAF;  //---turn on oled panel

    gpio_set_level(CONFIG_IO_RES, 1);
    vTaskDelay(1000 / portTICK_RATE_MS );
    gpio_set_level(CONFIG_IO_RES, 0);
    vTaskDelay(1000 / portTICK_RATE_MS );
    gpio_set_level(CONFIG_IO_RES, 1);
    ret = oled_write_buf(spi, init_cmd, 27, COMMAND);
    assert(ret == ESP_OK);
    vTaskDelay(100 / portTICK_RATE_MS );
    ret = oled_write_byte(spi, init_cmd[27], COMMAND);
    assert(ret == ESP_OK);
    return ret;
}

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
static void oled_set_page_segment(uint8_t *cmd, uint8_t page, uint8_t segment)
{
    cmd[0] = 0xB0 + page;
    cmd[2] = 0x10 | ((0xF0 & segment) >> 4);
    cmd[1] = 0x01 | (0x0F & segment);
}

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
esp_err_t oled_clear(spi_t *spi)  
{
    esp_err_t ret = ESP_FAIL;
	uint8_t page = 0; // from 0 to 7
    uint8_t clear_buf[128];
    memset(clear_buf, 0, 128*sizeof(uint8_t));
	for(; page < 8; page++) {  
        uint8_t page_address_cmd[3] = {};
        oled_set_page_segment(page_address_cmd, page, 0);
		ret = oled_write_buf(spi, page_address_cmd, 3, COMMAND);
        assert(ret ==ESP_OK);
        ret = oled_write_buf(spi, clear_buf, 128, DATA);
        assert(ret ==ESP_OK);
	}
    return ret;
}

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
esp_err_t oled_reset(spi_t *spi)
{
    esp_err_t ret = ESP_FAIL;
    return ret;
}

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
esp_err_t oled_entering_sleep_mode(spi_t *spi)
{
    esp_err_t ret = ESP_FAIL;
    ret = oled_write_byte(spi, 0xAE, COMMAND);
    assert(ret == ESP_OK);
    return ret;    
}

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
esp_err_t oled_exiting_sleep_mode(spi_t *spi)
{
    esp_err_t ret = ESP_FAIL;
    ret = oled_write_byte(spi, 0xAF, COMMAND);
    assert(ret == ESP_OK);
    vTaskDelay(100 / portTICK_RATE_MS );    
    return ret;
}

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
static inline void display(spi_t *spi, uint8_t* line, uint8_t page, uint8_t segment, uint8_t height, uint8_t width)
{
    esp_err_t ret = ESP_FAIL;
    uint8_t page_address_cmd[3] = {};
    for(int cur_page = 0; cur_page < height/8; cur_page++) {
        oled_set_page_segment(page_address_cmd, cur_page+page, segment);
        ret = oled_write_buf(spi, page_address_cmd, 3, COMMAND);
        assert(ret ==ESP_OK);
        ret = oled_write_buf(spi, line + width*cur_page, width, DATA);
        assert(ret ==ESP_OK);
    }
}

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
uint8_t byte_change(uint8_t data)
{
	uint8_t i = 0;
	uint8_t temp = 0;
	for (i = 0; i < 8; i ++)
	{
		temp = temp << 1;
		temp |= (data >> i) & 0x01;
	}
	return temp;
}

/**
 * @brief: show a char in screen in position defined in page(y coordinate) and segment(x coordinate).
 * @param: `page` there are 8(from 0 to 7) pages in oled(rows), choose a page to write a character into the RAM of oled 
 *         according your configuration(for instance, remapping).
 * @param: `segment` there are 128(from 0 to 127) segments in oled(columns), chose a segment to write a character.
 * @param: `character` it will be weitten into RAM of oled.
 * @return: ESP_OK on success, ESP_FAIL on fail.
 */
static esp_err_t oled_show_char(spi_t *spi, uint8_t page, uint8_t segment, char* c)
{
    esp_err_t ret = ESP_FAIL;
    uint8_t ch = *c;
    uint8_t page_address_cmd[3] = {};
    oled_set_page_segment(page_address_cmd, page, segment);
    ret = oled_write_buf(spi, page_address_cmd, 3, COMMAND);
    assert(ret ==ESP_OK);
    uint8_t* raw_data = asc2_1608[ch-0x20];
    uint8_t raw_data2[16];
    for(int i = 0; i < 16; i++) {
        raw_data2[i] = byte_change(raw_data[i]);
    }
    uint8_t processed_data[16] = {};
    for(int i = 0; i < 8; i++) {
        processed_data[i] = raw_data2[2*i];
        processed_data[i+8] = raw_data2[2*i+1];
    }
    display(spi, processed_data, page, segment, 16, 8);
    return ret;
}

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
static uint16_t transpose_one_column(uint16_t *raw_data, uint16_t shift_column, uint8_t choosed_column) {
    uint16_t transposed_column = 0;
    int raws = 15;
    for(int i = 0; i < 16; i++) {
        transposed_column |= (((raw_data[raws--] & shift_column) << choosed_column) >> i);
    }
    return transposed_column;
}

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
static void transposition(uint16_t *raw_data, uint8_t *processed_data)
{
    uint16_t shift_column = 0x8000;
    uint16_t transposed_buffer[16];
    for(int choosed_column = 0; choosed_column < 16; choosed_column++) {
        transposed_buffer[choosed_column] = transpose_one_column(raw_data, shift_column, choosed_column);
        shift_column >>= 1;
    }
    for(int segment = 0; segment < 16; segment++) {
        processed_data[segment+16] = (transposed_buffer[segment] >> 8) & 0xFF;
        processed_data[segment] = transposed_buffer[segment] & 0xFF;
    }
}

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
static esp_err_t* oled_show_chinese(spi_t *spi, uint8_t page, uint8_t segment, char* s)
{
    esp_err_t ret = ESP_FAIL;
    FILE* f = fopen("/sdcard/GBK16.FON", "rb");
    if (f == NULL) {
        return ESP_FAIL;
    }
    uint8_t line[32];
    uint8_t qh = *s;
    uint8_t ql = *(s+1);
    if(ql<0x7f)
        ql-=0x40;
    else 
        ql-=0x41;
    qh-=0x81;
    unsigned long foffset=((unsigned long)190*qh+ql)*32;
    if(fseek(f, foffset, SEEK_SET) != 0) {
        return ESP_FAIL;
    }
    fread(line, 1, 32, f);
    uint16_t tmp[16];
    for(int i = 0; i < 32; i++) {
        tmp[i] = (line[2*i] << 8) | line[2*i+1];
    }
    transposition(tmp, line);
    display(spi, line, page, segment, 16, 16);
    fclose(f);
    return ret;
}

// Ŀǰֻ֧��16*16���ֿ�
/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
esp_err_t oled_show_string(spi_t *spi, uint8_t page, uint8_t segment, char* str)
{
    esp_err_t ret = ESP_FAIL;
    uint8_t current_segment = page;
    uint8_t current_page = segment;
    while(*str != '\0') {
        if(*str > 0x80) {
            // chinese
            if(current_segment >= 127) {
                current_segment = 0;
                current_page += 2;
                if(current_page >= 7) {
                    current_page = 0;
                }
            }
            char s[3];
            s[0] = *str;
            s[1] = *(str+1);
            s[2] = '\0';
            ret = oled_show_chinese(spi, current_page, current_segment, s);
            assert(ret ==ESP_OK);
            current_segment += 16;
            str += 2;
        } else { 
            // ASCII char
            if(current_segment >= 127) {
                current_segment = 0;
                current_page += 2;
                if(current_page >= 7) {
                    current_page = 0;
                }
            }
            char s[2];
            s[0] = *str;
            s[1] = '\0';
            ret = oled_show_char(spi, current_page, current_segment, s);
            assert(ret ==ESP_OK);
            current_segment += 8;
            str++;
        }
    }  
    return ret;
}

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
esp_err_t oled_show_number(spi_t *spi, uint8_t page, uint8_t segment, float num)
{
    esp_err_t ret = ESP_FAIL;
    char buf[128] = {};
    if(num - (int)num > 0.00000001) {
        sprintf(buf, "%.2f", num);
    } else {
        sprintf(buf, "%d", (int)num);
    }
    ret = oled_show_string(spi, page, segment, buf);
    return ret;
}

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
void oled_right_horizontal_scroll(spi_t *spi, uint8_t start_page, uint8_t end_page, uint8_t interval)
{
    esp_err_t ret = ESP_FAIL;
    uint8_t cmd_set[7] = {};
    cmd_set[0] = 0x29;
    cmd_set[1] = 0x00;
    cmd_set[2] = start_page;
    cmd_set[3] = interval;
    cmd_set[4] = end_page;
    cmd_set[5] = 0x00;
    cmd_set[6] = 0x2F;
    ret = oled_write_buf(spi, cmd_set, 7, COMMAND);
    assert(ret ==ESP_OK);
}

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
void oled_left_horizontal_scroll(spi_t *spi, uint8_t start_page, uint8_t end_page, uint8_t interval)
{
    esp_err_t ret = ESP_FAIL;
    uint8_t cmd_set[7] = {};
    cmd_set[0] = 0x2A;
    cmd_set[1] = 0x00;
    cmd_set[2] = start_page;
    cmd_set[3] = interval;
    cmd_set[4] = end_page;
    cmd_set[5] = 0x00;
    cmd_set[6] = 0x2F;
    ret = oled_write_buf(spi, cmd_set, 7, COMMAND);
    assert(ret ==ESP_OK);
}

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
void oled_exit_scroll(spi_t *spi)
{
    esp_err_t ret = ESP_FAIL;
    uint8_t cmd_set = 0x2E;
    ret = oled_write_buf(spi, &cmd_set, 1, COMMAND);
    assert(ret ==ESP_OK);
}