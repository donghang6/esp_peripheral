/*
 * @Descripttion: 
 * @version: 
 * @Author: donghang
 * @Date: 2019-08-11 17:44:05
 * @LastEditors: donghang
 * @LastEditTime: 2019-08-15 23:57:55
 */
#include "sdio_tfcard.h"

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
esp_err_t sdio_tfcard_init(sdmmc_card_t** card)
{
    esp_err_t ret;
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    gpio_set_pull_mode(15, GPIO_PULLUP_ONLY);    // CMD, needed in 4- and 1- line modes
    gpio_set_pull_mode(2,  GPIO_PULLUP_ONLY);    // D0, needed in 4- and 1-line modes
    gpio_set_pull_mode(4,  GPIO_PULLUP_ONLY);    // D1, needed in 4-line mode only
    gpio_set_pull_mode(12, GPIO_PULLUP_ONLY);    // D2, needed in 4-line mode only
    gpio_set_pull_mode(13, GPIO_PULLUP_ONLY);    // D3, needed in 4- and 1-line modes
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };
    ret = esp_vfs_fat_sdmmc_mount("/sdcard", &host, &slot_config, &mount_config, card);
    assert(ret == ESP_OK);
    sdmmc_card_print_info(stdout, *card);
    return ret;
}

