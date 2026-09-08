PROJECT = main

CC = arm-none-eabi-gcc
AS = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

CPU = -mcpu=cortex-m3 -mthumb

CFLAGS = $(CPU) \
         -DSTM32F103xB \
         -I ../STM32CubeF1/Drivers/CMSIS/Include \
         -I ../STM32CubeF1/Drivers/CMSIS/Device/ST/STM32F1xx/Include \
         -ffunction-sections \
         -fdata-sections \
         -Wall \
         -O0

LDFLAGS = $(CPU) \
          -T linker.ld \
          -Wl,--gc-sections \
          -Wl,-Map=build/$(PROJECT).map \
          --specs=nano.specs \
          --specs=nosys.specs

SRC_C = main.c \
        ../STM32CubeF1/Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates/system_stm32f1xx.c

SRC_S = ../STM32CubeF1/Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates/gcc/startup_stm32f103xb.s

OBJ_C = $(SRC_C:.c=.o)
OBJ_S = $(SRC_S:.s=.o)

all: build/$(PROJECT).elf build/$(PROJECT).bin

build/$(PROJECT).elf: $(OBJ_C) $(OBJ_S)
	@mkdir -p build
	$(CC) $(LDFLAGS) -o $@ $^

build/$(PROJECT).bin: build/$(PROJECT).elf
	$(OBJCOPY) -O binary $< $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_C) $(OBJ_S)
	rm -f build/$(PROJECT).elf build/$(PROJECT).bin build/$(PROJECT).map

flash: build/$(PROJECT).elf
	openocd -f interface/stlink.cfg \
		-c "transport select hla_swd" \
		-c "set CPUTAPID 0x2ba01477" \
		-f target/stm32f1x.cfg \
		-c "program build/$(PROJECT).elf verify reset exit"
