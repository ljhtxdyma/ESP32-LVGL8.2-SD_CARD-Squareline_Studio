#ifndef MY_SD_FATFS_H
#define MY_SD_FATFS_H
 
#ifdef __cplusplus
extern "C" {
#endif
 
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "driver/sdspi_host.h"
#include "driver/spi_common.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"
 
void SD_Fatfs_Init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif 

#endif 