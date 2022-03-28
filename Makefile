# Don't use normal gcc, use the arm cross compiler
CC = arm-none-eabi-gcc
GDB = arm-none-eabi-gdb
OBJCOPY = arm-none-eabi-objcopy
# Set any constants based on the raspberry pi model.  Version 1 has some differences to 2 and 3

CPU = arm1176jzf-s
FPU = vfpv2

CFLAGS= -mcpu=$(CPU) -mfpu=$(FPU) -mfloat-abi=hard -mgeneral-regs-only -fpic -ffreestanding
CSRCFLAGS= -O2 -Wall -Wextra
LFLAGS= -ffreestanding -O2 -nostdlib

# Location of the files
KER_SRC = src/kernel
COM_SRC = src/common
OBJ_DIR = objects
A_HEAD = include
KER_CSRC = $(wildcard $(KER_SRC)/*.c)
COM_CSRC += $(wildcard $(COM_SRC)/*.c)
ASMSOURCES = $(wildcard $(KER_SRC)/*.s)
OBJECTS = $(patsubst $(KER_SRC)/%.s, $(OBJ_DIR)/%.o, $(ASMSOURCES))
OBJECTS += $(patsubst $(KER_SRC)/%.c, $(OBJ_DIR)/%.o, $(KER_CSRC))
OBJECTS += $(patsubst $(COM_SRC)/%.c, $(OBJ_DIR)/%.o, $(COM_CSRC))
HEADERS = $(wildcard $(A_HEAD)/*.h)

IMG_NAME=kernel

build: $(OBJECTS) $(HEADERS)
	echo $(OBJECTS)
	$(CC) -T linker.ld -o $(IMG_NAME).elf $(LFLAGS) $(OBJECTS)
	$(OBJCOPY) $(IMG_NAME).elf -O binary $(IMG_NAME).img

$(OBJ_DIR)/%.o: $(KER_SRC)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(KER_SRC) -I$(A_HEAD) -c $< -o $@ $(CSRCFLAGS)

$(OBJ_DIR)/%.o: $(KER_SRC)/%.s
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(KER_SRC) -c $< -o $@

$(OBJ_DIR)/%.o: $(COM_SRC)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(KER_SRC) -I$(COM_SRC) -I$(A_HEAD) -c $< -o $@ $(CSRCFLAGS)

clean:
	rm -rf $(OBJ_DIR)
	rm $(IMG_NAME).elf
	rm $(IMG_NAME).img

run: build
	qemu-system-arm -m 256 -M versatilepb -cpu arm1176 -serial stdio -kernel $(IMG_NAME).img

dbg:
	$(GDB) $(IMG_NAME).elf
dbgrun: build gdbinit
	qemu-system-arm -m 256 -no-reboot -M versatilepb -cpu arm1176 -serial stdio -kernel $(IMG_NAME).elf -S -s

.PHONY: gdbinit
gdbinit:
	echo "target remote localhost:1234" > .gdbinit
	echo "break kernel_main" >> .gdbinit
