TARGET = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp
TIVA_WARE_PATH = /home/luck/.local/share/tivaware/
THIRD_PARTY_LIBS_INC = -DPART_TM4C123GH6PM -I$(TIVA_WARE_PATH) -DTARGET_IS_TM4C123_RB1
THIRD_PARTY_LIBS_OBJ = $(TIVA_WARE_PATH)/driverlib/gcc/libdriver.a


LDSCRIPT = ldscript.ld
ELF = bin/main.elf
BIN = bin/main.bin
ASM = bin/main.asm

PREFIX=

CC = $(PREFIX)arm-none-eabi-gcc
LD = $(PREFIX)arm-none-eabi-g++
OBJCOPY = $(PREFIX)arm-none-eabi-objcopy
OBJDUMP = $(PREFIX)arm-none-eabi-objdump
SIZE = $(PREFIX)arm-none-eabi-size

CFLAGS = -O2 -Os -s -Wall -Wextra -pedantic -std=c99
CFLAGS+= $(TARGET) -fdata-sections -ffunction-sections
CFLAGS+= $(THIRD_PARTY_LIBS_INC)

LDFLAGS = -T$(LDSCRIPT) $(TARGET) -mthumb -Wl,--gc-sections
LDFLAGS+= $(THIRD_PARTY_LIBS_OBJ) -lc -lm -lgcc

SRCS = $(shell find -name '*.c')

OBJS = $(SRCS:.c=.o)
OBJS+= $(THIRD_PARTY_LIBS_OBJ)

all: $(ELF)
	$(OBJDUMP) --disassemble $< > $(ASM)
	$(OBJCOPY) -O binary $< $(BIN)
	$(SIZE) $(ELF)

$(ELF): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@  $<

%.o: %.s
	$(CC) -c $(CFLAGS) -o $@  $<

CLEAN_FILES = $(shell find -name '*.o')
CLEAN_FILES+= build/main*
clean:
	rm -f $(CLEAN_FILES)

run: all
	sudo lm4flash $(BIN)

