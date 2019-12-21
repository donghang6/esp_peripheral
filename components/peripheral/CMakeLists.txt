idf_component_register(SRCS "spi_oled.c" "lcd.c"
                       INCLUDE_DIRS "include"
                       				"../port/include"
                       REQUIRES freertos)