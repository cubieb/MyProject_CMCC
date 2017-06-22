TARGTE :=wifi_setting.cgi

CROSS := mipsel-linux-
CC := $(CROSS)gcc

all: wifi_setting.o mtk_operate.o  utils.o crc32.o flash_api.o nvram_env.o 
	$(CC) $^ -o $(TARGTE)

clean: 
	rm wifi_setting.o mtk_operate.o  utils.o $(TARGTE)
