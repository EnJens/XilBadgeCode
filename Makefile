
.SUFFIXES:
.PHONY: all

CROSS_COMPILE ?= arm-none-eabi-
CC := $(CROSS_COMPILE)gcc
LD := $(CROSS_COMPILE)ld
OBJCOPY := $(CROSS_COMPILE)objcopy


PROJECTNAME=template
SOURCES := main.c hw_pinout.c

GECKO_SDK_PATH := lib/GeckoSDK
EFM_DEVICE_PATH := $(GECKO_SDK_PATH)/Device/SiliconLabs/EFM32TG
DEVICE_SOURCES := system_efm32tg.c GCC/startup_efm32tg.c
LINKER_SCRIPT := $(EFM_DEVICE_PATH)/Source/GCC/efm32tg.ld
EMLIB_PATH := $(GECKO_SDK_PATH)/emlib
EMLIB_SRC_PATH := $(EMLIB_PATH)/src
CMSIS_PATH := $(GECKO_SDK_PATH)/CMSIS
LIB_SOURCES := em_acmp.c em_adc.c em_aes.c em_assert.c em_burtc.c em_cmu.c \
	em_cryotimer.c em_crypto.c em_dac.c em_dbg.c em_dma.c em_ebi.c em_emu.c \
	em_gpcrc.c em_gpio.c em_i2c.c em_idac.c em_int.c em_lcd.c em_ldma.c \
	em_lesense.c em_letimer.c em_leuart.c em_mpu.c em_msc.c em_opamp.c \
	em_pcnt.c em_prs.c em_rmu.c em_rtc.c em_rtcc.c em_system.c em_timer.c \
	em_usart.c em_vcmp.c em_wdog.c

OBJ_DIR := build
OBJECTS := $(addprefix $(OBJ_DIR)/src/, $(SOURCES:.c=.o)) \
	$(addprefix $(OBJ_DIR)/$(EMLIB_SRC_PATH)/, $(LIB_SOURCES:.c=.o)) \
	$(addprefix $(OBJ_DIR)/$(EFM_DEVICE_PATH)/Source/, $(DEVICE_SOURCES:.c=.o))

OUTPUT_DIR := build
VPATH = .:src:$(EMLIB_SRC_PATH):$(EMLIB_DEVICE_PATH):$(EFM_DEVICE_PATH)/Source

INCLUDE_DIRS := inc $(EFM_DEVICE_PATH)/Include $(EMLIB_PATH)/inc \
	$(CMSIS_PATH)/Include

CFLAGS := -Wall -Wextra -mcpu=cortex-m3 -mthumb \
	-mfix-cortex-m3-ldrd -ffunction-sections \
	-fdata-sections -fomit-frame-pointer -std=c99 \
	-DEFM32TG110F32 \
	$(addprefix -I, $(INCLUDE_DIRS))

LDFLAGS := -Xlinker -Map=$(PROJECTNAME).map -mcpu=cortex-m3 -mthumb \
	-T$(LINKER_SCRIPT)
LIBS := -Wl,--gc-sections \
	-Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group


all: $(PROJECTNAME).bin

$(OUTPUT_DIR):
	mkdir -p $@

$(OBJECTS): $(OUTPUT_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $<

$(PROJECTNAME).elf: $(OBJECTS)
	@echo "Linking target: $@"
	$(CC) -o $@ $(LDFLAGS) $(OBJECTS) $(LIBS)

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

clean:
	@echo "Cleaning output files"
	@rm -rf build *.elf *.bin *.map

debug:
	openocd -f interface/stlink-v2.cfg -f target/efm32.cfg

flash: $(PROJECTNAME).elf
	openocd -f interface/stlink-v2.cfg -f target/efm32.cfg \
	-c 'init; program $(PROJECTNAME).elf verify reset exit ;'
