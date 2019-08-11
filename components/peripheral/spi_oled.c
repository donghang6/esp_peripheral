#include "spi_oled.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void lcd_spi_pre_transfer_callback(spi_transaction_t *t)
{
    int dc=(int)t->user;

    gpio_set_level(CONFIG_IO_DC, dc);
}

spi_device_interface_config_t oledcfg  = {
    .command_bits = CONFIG_COMMAND_BITS,
    .address_bits = CONFIG_ADDRESS_BITS,
    .clock_speed_hz = CONFIG_CLOCK_SPEED_HZ,
    .mode = CONFIG_MODE,
    .spics_io_num = CONFIG_SPICS_IO_NUM,
    .queue_size = CONFIG_QUEUE_SIZE,
    .pre_cb = lcd_spi_pre_transfer_callback,
};

esp_err_t oled_gpio_config(gpio_config_t *oled_io_conf)
{
    esp_err_t ret = ESP_FAIL;
    oled_io_conf->pin_bit_mask = (1ULL << CONFIG_IO_RES) | (1ULL << CONFIG_IO_DC);   
    oled_io_conf->pull_down_en = GPIO_PULLDOWN_ENABLE;
    oled_io_conf->mode = GPIO_MODE_OUTPUT;
    ret = gpio_config(oled_io_conf);

    return ret;
}

esp_err_t oled_write_buf(spi_t *spi, uint8_t *buffer, size_t len, void *uservarabile)
{
    return write_buff(spi, buffer, len, uservarabile, 0, 0);
}

esp_err_t oled_write_byte(spi_t *spi, uint8_t data, void * uservarabile)
{
    return write_byte(spi, data, uservarabile, 0, 0);
}

esp_err_t oled_init(spi_t *spi)
{
    esp_err_t ret = ESP_FAIL;
    uint8_t init_cmd[29] = {};
    gpio_config_t oled_io_conf;

    memset(&oled_io_conf, 0, sizeof(gpio_config_t));
    ret = oled_gpio_config(&oled_io_conf);
    assert(ret == ESP_OK);
    ret = open(spi, &oledcfg);
    assert(ret ==ESP_OK);
    init_cmd[0] = 0xAE;//--turn off oled panel
    init_cmd[1] = 0x00;//---set low column address
    init_cmd[2] = 0x10;//---set high column address
    init_cmd[3] = 0x40;//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    init_cmd[4] = 0x81;//--set contrast control register. reset is 7Fh
    init_cmd[5] = 0xCF;// Set SEG Output Current Brightness
    init_cmd[6] = 0xA1;//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    init_cmd[7] = 0xC8;//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    init_cmd[8] = 0xA6;//--set normal display
    init_cmd[9] = 0xA8;//--set multiplex ratio(1 to 64)
    init_cmd[10] = 0x3F;//--1/64 duty
    init_cmd[11] = 0xD3;//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    init_cmd[12] = 0x00;//not offset
    init_cmd[13] = 0xD5;//--set display clock divide ratio/oscillator frequency
    init_cmd[14] = 0x80;//--set divide ratio, Set Clock as 100 Frames/Sec
    init_cmd[15] = 0xD9;//--set pre-charge period
    init_cmd[16] = 0xF1;//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    init_cmd[17] = 0xDA;//--set com pins hardware configuration
    init_cmd[18] = 0x12;
    init_cmd[19] = 0xDB;//--set vcomh
    init_cmd[20] = 0x40;//Set VCOM Deselect Level
    init_cmd[21] = 0x20;//-Set Page Addressing Mode (0x00/0x01/0x02)
    init_cmd[22] = 0x02;// paging addressing mode
    init_cmd[23] = 0x8D;//--set Charge Pump enable/disable
    init_cmd[24] = 0x14;//--set(0x10) disable
    init_cmd[25] = 0xA4;// Disable Entire Display On (0xa4/0xa5)
    init_cmd[26] = 0xA6;// Disable Inverse Display On (0xa6/a7) 
    init_cmd[27] = 0xAF;//--turn on oled panel

    init_cmd[28] = 0xAF;//--turn on oled panel

    vTaskDelay(100 / portTICK_RATE_MS );
    gpio_set_level(CONFIG_IO_RES, 0);
    vTaskDelay(200 / portTICK_RATE_MS );
    gpio_set_level(CONFIG_IO_RES, 1);

    return oled_write_buf(spi, init_cmd, 29, COMMAND);
}

esp_err_t oled_clear(spi_t *spi)  
{
    esp_err_t ret = ESP_FAIL;
	uint8_t i;    
    uint8_t clear_buf[128];

    memset(clear_buf, 0xFF, 128*sizeof(uint8_t));

	for(i=0; i<8; i++)
	{  
		ret = oled_write_byte(spi, 0xB0+i, COMMAND);    //设置页地址（0~7）
        assert(ret ==ESP_OK);
		ret = oled_write_byte(spi, 0x00, COMMAND);      //设置显示位置—列低地址
        assert(ret ==ESP_OK);
		ret = oled_write_byte(spi, 0x10, COMMAND);      //设置显示位置—列高地址 
        assert(ret ==ESP_OK);
        ret = oled_write_buf(spi, clear_buf, 128, DATA);
        assert(ret ==ESP_OK);
	}   //更新显示

    return ret;
}