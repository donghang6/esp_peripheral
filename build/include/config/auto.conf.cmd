deps_config := \
	/home/dongh/esp/esp-idf/components/app_trace/Kconfig \
	/home/dongh/esp/esp-idf/components/aws_iot/Kconfig \
	/home/dongh/esp/esp-idf/components/bt/Kconfig \
	/home/dongh/esp/esp-idf/components/driver/Kconfig \
	/home/dongh/esp/esp-idf/components/esp32/Kconfig \
	/home/dongh/esp/esp-idf/components/esp_adc_cal/Kconfig \
	/home/dongh/esp/esp-idf/components/esp_event/Kconfig \
	/home/dongh/esp/esp-idf/components/esp_http_client/Kconfig \
	/home/dongh/esp/esp-idf/components/esp_http_server/Kconfig \
	/home/dongh/esp/esp-idf/components/ethernet/Kconfig \
	/home/dongh/esp/esp-idf/components/fatfs/Kconfig \
	/home/dongh/esp/esp-idf/components/freemodbus/Kconfig \
	/home/dongh/esp/esp-idf/components/freertos/Kconfig \
	/home/dongh/esp/esp-idf/components/heap/Kconfig \
	/home/dongh/esp/esp-idf/components/libsodium/Kconfig \
	/home/dongh/esp/esp-idf/components/log/Kconfig \
	/home/dongh/esp/esp-idf/components/lwip/Kconfig \
	/home/dongh/esp/esp-idf/components/mbedtls/Kconfig \
	/home/dongh/esp/esp-idf/components/mdns/Kconfig \
	/home/dongh/esp/esp-idf/components/mqtt/Kconfig \
	/home/dongh/esp/esp-idf/components/nvs_flash/Kconfig \
	/home/dongh/esp/esp-idf/components/openssl/Kconfig \
	/c/Users/dongh/Documents/GitHub/esp_peripheral/components/peripheral/Kconfig \
	/home/dongh/esp/esp-idf/components/pthread/Kconfig \
	/home/dongh/esp/esp-idf/components/spi_flash/Kconfig \
	/home/dongh/esp/esp-idf/components/spiffs/Kconfig \
	/home/dongh/esp/esp-idf/components/tcpip_adapter/Kconfig \
	/home/dongh/esp/esp-idf/components/vfs/Kconfig \
	/home/dongh/esp/esp-idf/components/wear_levelling/Kconfig \
	/home/dongh/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/dongh/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/dongh/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/dongh/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)

ifneq "$(IDF_CMAKE)" "n"
include/config/auto.conf: FORCE
endif

$(deps_config): ;
