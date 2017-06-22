TARGTE := getInfo

CROSS := mipsel-linux-
CC := $(CROSS)gcc

all: client_info.o mtk_operate.o  utils.o crc32.o flash_api.o nvram_env.o  adm_info.o
	$(CC) $^ -o $(TARGTE)

clean: 
	rm client_info.o adm_info.o mtk_operate.o  utils.o $(TARGTE)
