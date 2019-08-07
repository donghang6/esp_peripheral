deps_config := \
	/home/Hang/esp/esp-idf/components/app_trace/Kconfig \
	/home/Hang/esp/esp-idf/components/aws_iot/Kconfig \
	/home/Hang/esp/esp-idf/components/bt/Kconfig \
	/home/Hang/esp/esp-idf/components/driver/Kconfig \
	/home/Hang/esp/esp-idf/components/esp32/Kconfig \
	/home/Hang/esp/esp-idf/components/esp_adc_cal/Kconfig \
	/home/Hang/esp/esp-idf/components/esp_event/Kconfig \
	/home/Hang/esp/esp-idf/components/esp_http_client/Kconfig \
	/home/Hang/esp/esp-idf/components/esp_http_server/Kconfig \
	/home/Hang/esp/esp-idf/components/ethernet/Kconfig \
	/home/Hang/esp/esp-idf/components/fatfs/Kconfig \
	/home/Hang/esp/esp-idf/components/freemodbus/Kconfig \
	/home/Hang/esp/esp-idf/components/freertos/Kconfig \
	/home/Hang/esp/esp-idf/components/heap/Kconfig \
	/home/Hang/esp/esp-idf/components/libsodium/Kconfig \
	/home/Hang/esp/esp-idf/components/log/Kconfig \
	/home/Hang/esp/esp-idf/components/lwip/Kconfig \
	/home/Hang/esp/esp-idf/components/mbedtls/Kconfig \
	/home/Hang/esp/esp-idf/components/mdns/Kconfig \
	/home/Hang/esp/esp-idf/components/mqtt/Kconfig \
	/home/Hang/esp/esp-idf/components/nvs_flash/Kconfig \
	/home/Hang/esp/esp-idf/components/openssl/Kconfig \
	/home/Hang/app/esp_peripheral/components/peripheral/Kconfig \
	/home/Hang/esp/esp-idf/components/pthread/Kconfig \
	/home/Hang/esp/esp-idf/components/spi_flash/Kconfig \
	/home/Hang/esp/esp-idf/components/spiffs/Kconfig \
	/home/Hang/esp/esp-idf/components/tcpip_adapter/Kconfig \
	/home/Hang/esp/esp-idf/components/vfs/Kconfig \
	/home/Hang/esp/esp-idf/components/wear_levelling/Kconfig \
	/home/Hang/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/Hang/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/Hang/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/Hang/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)

ifneq "$(IDF_CMAKE)" "n"
include/config/auto.conf: FORCE
endif

$(deps_config): ;
