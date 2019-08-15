/*
 * @Descripttion: 
 * @version: 
 * @Author: donghang
 * @Date: 2019-08-11 17:43:54
 * @LastEditors: donghang
 * @LastEditTime: 2019-08-15 23:57:53
 */
#ifndef SDIO_TFCARD_H_
#define SDIO_TFCARD_H_
#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"
#include "sdmmc_cmd.h"

/**
 * @brief: 
 * @param {type} 
 * @return: 
 */
esp_err_t sdio_tfcard_init(sdmmc_card_t** card);
#endif