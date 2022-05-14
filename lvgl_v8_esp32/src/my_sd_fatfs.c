#include "my_sd_fatfs.h"
 
static const char *TAG = "TFCardFat";
 
#define MOUNT_POINT		"/sdcard"		// 第一个字节必须是‘/’
#define SPI_DMA_CHAN    2
 
//在测试SD和SPI模式时，请记住，一旦在SPI模式下初始化了卡，在不接通卡电源的情况下就无法在SD模式下将其重新初始化。
#define PIN_NUM_MISO 19
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK  18
#define PIN_NUM_CS   5
 
// static void get_fatfs_usage(size_t* out_total_bytes, size_t* out_free_bytes);
 
 
void SD_Fatfs_Init(void)
{
	//gpio_set_level(14,1);
	
	esp_err_t ret;								// ESP错误定义
	sdmmc_card_t* card;							// SD / MMC卡信息结构
	const char mount_point[] = MOUNT_POINT;		// 根目录
	char ReadFileBuff[64];
 
	esp_vfs_fat_sdmmc_mount_config_t mount_config = {	// 文件系统挂载配置
		.format_if_mount_failed = false,				// 如果挂载失败：true会重新分区和格式化/false不会重新分区和格式化
		.max_files = 5,									// 打开文件最大数量
		.allocation_unit_size = 32 * 1024
	};

	ESP_LOGI(TAG,"->Initializing SD card");
	ESP_LOGI(TAG,"->Using SPI peripheralr");

	sdmmc_host_t host = SDSPI_HOST_DEFAULT();
	host.slot = HSPI_HOST;
	host.max_freq_khz = SDMMC_FREQ_PROBING;

	spi_bus_config_t bus_cfg = {
		.mosi_io_num = PIN_NUM_MOSI,
		.miso_io_num = PIN_NUM_MISO,
		.sclk_io_num = PIN_NUM_CLK,
		.quadwp_io_num = -1,
		.quadhd_io_num = -1,
		.max_transfer_sz = 4 * 1024 * sizeof(uint8_t),
	};
	
	// SPI总线初始化

	ret = spi_bus_initialize(host.slot, &bus_cfg, SPI_DMA_CH_AUTO);
	if (ret != ESP_OK) {
		printf("%s->Failed to initialize bus.\r\n",TAG);
		return;
	}
	
	// 这将初始化没有卡检测（CD）和写保护（WP）信号的插槽。
	// 如果您的主板有这些信号，请修改slot_config.gpio_cd和slot_config.gpio_wp。
	sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();// SPI3_HOST  1
	slot_config.gpio_cs = PIN_NUM_CS;
	slot_config.host_id = host.slot;
	// 挂载文件系统
	ret = esp_vfs_fat_sdspi_mount(mount_point, &host, &slot_config, &mount_config, &card);
	if (ret != ESP_OK) {
		if (ret == ESP_FAIL) {
			printf("%s->Failed to mount filesystem.%s\r\n",TAG,
				"If you want the card to be formatted, set the EXAMPLE_FORMAT_IF_MOUNT_FAILED menuconfig option.");
		} else {
			printf("%s->Failed to initialize the card %s  (%s). ",TAG,
				"Make sure SD card lines have pull-up resistors in place.", esp_err_to_name(ret));
		}
		return;
	}

	// TF卡已经初始化，打印TF卡属性
	sdmmc_card_print_info(stdout, card);
 
 
	// Print FAT FS size information
	// size_t bytes_total, bytes_free;
	// get_fatfs_usage(&bytes_total, &bytes_free);
	// printf("%s->FAT FS Total: %d MB, Free: %d MB \r\n",TAG, bytes_total / 1024, bytes_free / 1024);
}
