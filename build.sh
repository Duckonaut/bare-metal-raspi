arm-none-eabi-gcc -mcpu=cortex-a7 -fpic -ffreestanding -c source/boot.s -o boot.o
arm-none-eabi-gcc -mcpu=cortex-a7 -fpic -ffreestanding -std=gnu99 -c source/kernel.c -o kernel.o -O2 -Wall -Wextra
arm-none-eabi-gcc -T kernel.ld -o myos.elf -ffreestanding -O2 -nostdlib boot.o kernel.o
arm-none-eabi-objcopy myos.elf -O binary kernel.img
