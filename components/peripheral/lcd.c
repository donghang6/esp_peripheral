#include "lcd.h"
#include "font.h"

spi_device_interface_config_t devcfg={
	.clock_speed_hz=10*1000*1000,           //Clock out at 10 MHz
	.mode=0,                                //SPI mode 0
	.spics_io_num=PIN_NUM_CS,               //CS pin
	.queue_size=7,                          //We want to be able to queue 7 transactions at a time
	.pre_cb=lcd_spi_pre_transfer_callback,  //Specify pre-transfer callback to handle D/C line
};

DRAM_ATTR static const lcd_init_cmd_t ili_init_cmds[]={
    /* Power contorl B, power control = 0, DC_ENA = 1 */
    {0xCF, {0x00, 0x83, 0X30}, 3},
    /* Power on sequence control,
     * cp1 keeps 1 frame, 1st frame enable
     * vcl = 0, ddvdh=3, vgh=1, vgl=2
     * DDVDH_ENH=1
     */
    {0xED, {0x64, 0x03, 0X12, 0X81}, 4},
    /* Driver timing control A,
     * non-overlap=default +1
     * EQ=default - 1, CR=default
     * pre-charge=default - 1
     */
    {0xE8, {0x85, 0x01, 0x79}, 3},
    /* Power control A, Vcore=1.6V, DDVDH=5.6V */
    {0xCB, {0x39, 0x2C, 0x00, 0x34, 0x02}, 5},
    /* Pump ratio control, DDVDH=2xVCl */
    {0xF7, {0x20}, 1},
    /* Driver timing control, all=0 unit */
    {0xEA, {0x00, 0x00}, 2},
    /* Power control 1, GVDD=4.75V */
    {0xC0, {0x26}, 1},
    /* Power control 2, DDVDH=VCl*2, VGH=VCl*7, VGL=-VCl*3 */
    {0xC1, {0x11}, 1},
    /* VCOM control 1, VCOMH=4.025V, VCOML=-0.950V */
    {0xC5, {0x35, 0x3E}, 2},
    /* VCOM control 2, VCOMH=VMH-2, VCOML=VML-2 */
    {0xC7, {0xBE}, 1},
    /* Memory access contorl, MX=MY=0, MV=1, ML=0, BGR=1, MH=0 */
    {0x36, {0x28}, 1},
    /* Pixel format, 16bits/pixel for RGB/MCU interface */
    {0x3A, {0x55}, 1},
    /* Frame rate control, f=fosc, 70Hz fps */
    {0xB1, {0x00, 0x1B}, 2},
    /* Enable 3G, disabled */
    {0xF2, {0x08}, 1},
    /* Gamma set, curve 1 */
    {0x26, {0x01}, 1},
    /* Positive gamma correction */
    {0xE0, {0x1F, 0x1A, 0x18, 0x0A, 0x0F, 0x06, 0x45, 0X87, 0x32, 0x0A, 0x07, 0x02, 0x07, 0x05, 0x00}, 15},
    /* Negative gamma correction */
    {0XE1, {0x00, 0x25, 0x27, 0x05, 0x10, 0x09, 0x3A, 0x78, 0x4D, 0x05, 0x18, 0x0D, 0x38, 0x3A, 0x1F}, 15},
    /* Column address set, SC=0, EC=0xEF */
    {0x2A, {0x00, 0x00, 0x00, 0xEF}, 4},
    /* Page address set, SP=0, EP=0x013F */
    {0x2B, {0x00, 0x00, 0x01, 0x3f}, 4},
    /* Memory write */
    {0x2C, {0}, 0},
    /* Entry mode set, Low vol detect disabled, normal display */
    {0xB7, {0x07}, 1},
    /* Display function control */
    {0xB6, {0x0A, 0x82, 0x27, 0x00}, 4},
    /* Sleep out */
    {0x11, {0}, 0x80},
    /* Display on */
    {0x29, {0}, 0x80},
    {0, {0}, 0xff},
};

/* Send a command to the LCD. Uses spi_device_polling_transmit, which waits
 * until the transfer is complete.
 *
 * Since command transactions are usually small, they are handled in polling
 * mode for higher speed. The overhead of interrupt transactions is more than
 * just waiting for the transaction to complete.
 */
void lcd_cmd(spi_device_handle_t spi, const uint8_t cmd)
{
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));       //Zero out the transaction
    t.length=8;                     //Command is 8 bits
    t.tx_buffer=&cmd;               //The data is the cmd itself
    t.user=(void*)0;                //D/C needs to be set to 0
    ret=spi_device_polling_transmit(spi, &t);  //Transmit!
    assert(ret==ESP_OK);            //Should have had no issues.
}

/* Send data to the LCD. Uses spi_device_polling_transmit, which waits until the
 * transfer is complete.
 *
 * Since data transactions are usually small, they are handled in polling
 * mode for higher speed. The overhead of interrupt transactions is more than
 * just waiting for the transaction to complete.
 */
void lcd_data(spi_device_handle_t spi, const uint8_t *data, int len)
{
    esp_err_t ret;
    spi_transaction_t t;
    if (len==0) return;             //no need to send anything
    memset(&t, 0, sizeof(t));       //Zero out the transaction
    t.length=len*8;                 //Len is in bytes, transaction length is in bits.
    t.tx_buffer=data;               //Data
    t.user=(void*)1;                //D/C needs to be set to 1
    ret=spi_device_polling_transmit(spi, &t);  //Transmit!
    assert(ret==ESP_OK);            //Should have had no issues.
}

//This function is called (in irq context!) just before a transmission starts. It will
//set the D/C line to the value indicated in the user field.
void lcd_spi_pre_transfer_callback(spi_transaction_t *t)
{
    int dc=(int)t->user;
    gpio_set_level(PIN_NUM_DC, dc);
}

uint32_t lcd_get_id(spi_device_handle_t spi)
{
    //get_id cmd
    lcd_cmd(spi, 0x04);

    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length=8*3;
    t.flags = SPI_TRANS_USE_RXDATA;
    t.user = (void*)1;

    esp_err_t ret = spi_device_polling_transmit(spi, &t);
    ESP_ERROR_CHECK(ret);

    return *(uint32_t*)t.rx_data;
}

void lcd_init(spi_t *spi)
{
    int cmd=0;
    esp_err_t ret = ESP_FAIL;
    const lcd_init_cmd_t* lcd_init_cmds;

    ret=openport(spi, &devcfg);
    ESP_ERROR_CHECK(ret);
    //Initialize non-SPI GPIOs
    gpio_set_direction(PIN_NUM_DC, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_NUM_RST, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_NUM_BCKL, GPIO_MODE_OUTPUT);

    //Reset the display
    gpio_set_level(PIN_NUM_RST, 0);
    vTaskDelay(100 / portTICK_RATE_MS);
    gpio_set_level(PIN_NUM_RST, 1);
    vTaskDelay(100 / portTICK_RATE_MS);

    //detect LCD type
    uint32_t lcd_id = lcd_get_id(spi->spi_handle);

    printf("LCD ID: %08X\n", lcd_id);
    if ( lcd_id == 0 ) {
        //zero, ili
        printf("LCD ILI9341 initialization.\n");
        lcd_init_cmds = ili_init_cmds;
    }

    //Send all the commands
    while (lcd_init_cmds[cmd].databytes!=0xff) {
        lcd_cmd(spi->spi_handle, lcd_init_cmds[cmd].cmd);
        lcd_data(spi->spi_handle, lcd_init_cmds[cmd].data, lcd_init_cmds[cmd].databytes&0x1F);
        if (lcd_init_cmds[cmd].databytes&0x80) {
            vTaskDelay(100 / portTICK_RATE_MS);
        }
        cmd++;
    }

    ///Enable backlight
    gpio_set_level(PIN_NUM_BCKL, 0);
}

static void lcd_lines_finish(spi_t *spi)
{
    spi_transaction_t *rtrans;
    esp_err_t ret;
    //Wait for all 6 transactions to be done and get back the results.
    for (int x=0; x<6; x++) {
        ret=spi_device_get_trans_result(spi->spi_handle, &rtrans, portMAX_DELAY);
        assert(ret==ESP_OK);
        //We could inspect rtrans now if we received any info back. The LCD is treated as write-only, though.
    }
}

void lcd_lines(spi_t *spi, int xstart, int xend, int ystart, int yend, uint16_t *linedata)
{
    esp_err_t ret;
    int x;
    //Transaction descriptors. Declared static so they're not allocated on the stack; we need this memory even when this
    //function is finished because the SPI driver needs access to it even while we're already calculating the next line.
    static spi_transaction_t trans[6];

    //In theory, it's better to initialize trans and data only once and hang on to the initialized
    //variables. We allocate them on the stack, so we need to re-init them each call.
    for (x=0; x<6; x++) {
        memset(&trans[x], 0, sizeof(spi_transaction_t));
        if ((x&1)==0) {
            //Even transfers are commands
            trans[x].length=8;
            trans[x].user=(void*)0;
        } else {
            //Odd transfers are data
            trans[x].length=8*4;
            trans[x].user=(void*)1;
        }
        trans[x].flags=SPI_TRANS_USE_TXDATA;
    }
    trans[0].tx_data[0]=0x2A;           //Column Address Set
    trans[1].tx_data[0]=xstart-1;        //Start Col High 1
    trans[1].tx_data[1]=xstart-1;     //Start Col Low 1
    trans[1].tx_data[2]=(xend-1)>>8;       //End Col High 240
    trans[1].tx_data[3]=(xend-1)&0xff;     //End Col Low 240
    trans[2].tx_data[0]=0x2B;           //Page address set
    trans[3].tx_data[0]=(ystart-1)>>8;        //Start page high 1
    trans[3].tx_data[1]=(ystart-1)&0xff;      //start page low 1
    trans[3].tx_data[2]=(yend-1)>>8;    //end page high 320
    trans[3].tx_data[3]=(yend-1)&0xff;  //end page lows 320
    trans[4].tx_data[0]=0x2C;           //memory write
    trans[5].tx_buffer=linedata;        //finally send the line data
    trans[5].length=(xend-xstart+1)*2*8*(yend-ystart+1);          //Data length, in bits one point have 16 bits
    trans[5].flags=0; //undo SPI_TRANS_USE_TXDATA flag
    //Queue all transactions.
    for (x=0; x<6; x++) {
        ret=spi_device_queue_trans(spi->spi_handle, &trans[x], portMAX_DELAY);
        assert(ret==ESP_OK);
    }
}

void lcd_clean(spi_t *spi, uint16_t color)
{
    uint16_t *lines;
    //Allocate memory for the pixel buffers
    lines = heap_caps_malloc(CROSS_LINES*PARALLEL_LINES*sizeof(uint16_t), MALLOC_CAP_DMA);
    assert(lines != NULL);
    for (int i = 0; i < PARALLEL_LINES; i++) {
    	for (int j = 0; j < CROSS_LINES; j++) {
    		lines[j+i*CROSS_LINES] = color;
    	}
    }
	for (int y=0; y<240; y+=PARALLEL_LINES) {
		lcd_lines(spi, 1, CROSS_LINES, 1+y, PARALLEL_LINES+y, lines);
		lcd_lines_finish(spi);
	}
	heap_caps_free(lines);
}

void lcd_show_char(spi_t *spi, int xpos, int ypos, char c, uint16_t color)
{
    uint16_t *lines;
    uint8_t ch = c;
    const unsigned char* raw_data = asc2_1608[ch-0x20];
    //Allocate memory for the pixel buffers
    lines = heap_caps_malloc(8*16*sizeof(uint16_t), MALLOC_CAP_DMA);
    assert(lines != NULL);
    for (int i = 0; i < 16; i++) {
    	for (int j = 0; j < 8; j++) {
    		if (raw_data[i]&(0x80>>j)){
    			lines[j+i*8] = color;
    		} else {
    			lines[j+i*8] = WHITE;
    		}
    	}
    }
    lcd_lines(spi, xpos, xpos+7, ypos, ypos+15, lines);
	lcd_lines_finish(spi);
	heap_caps_free(lines);
}



//esp_err_t lcd_show_number(spi_t *spi, uint8_t page, uint8_t segment, float num)
//{
//
//}
//
//esp_err_t lcd_show_string(spi_t *spi, uint8_t page, uint8_t segment, char* str)
//{
//
//}
