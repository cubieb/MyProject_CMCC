TARGTE := getinfo

CROSS := mipsel-linux-
CC := $(CROSS)gcc

#all: wifi_setting.o mtk_operate.o  utils.o crc32.o flash_api.o nvram_env.o 
#	$(CC) $^ -o $(TARGTE)

#all: stacountinfo.o mtk_operate.o  utils.o crc32.o flash_api.o nvram_env.o 
#	$(CC) $^ -o $(TARGTE)

#all: mytest.o mtk_operate.o  utils.o crc32.o flash_api.o nvram_env.o 
#   $(CC) $^ -o $(TARGTE)

#all: client_info.o  utils.o 
#	$(CC) $^ -o $(TARGTE)

all: mytest.o 
	$(CC) $^ -o $(TARGTE)
clean: 
	rm client_list.o test.o wifi_setting.o mtk_operate.o  utils.o $(TARGTE)
