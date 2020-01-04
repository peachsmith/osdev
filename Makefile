# target should be the location of the cross compiler
#TARGET=

# the build tools are assumed to be the GNU gcc toolchain
AS=$(TARGET)-as
CC=$(TARGET)-gcc
SRC=./src
INC=-I./include

C_FLAGS=-c -std=gnu99 -ffreestanding -O2 -Wall -Wextra

LNK_FLAGS=-ffreestanding -O2 -nostdlib

OBJ=boot.o port.o enable_paging.o pit.o serial.o load_gdt.o load_idt.o gdt.o idt.o isr.o paging.o string.o vga.o kernel.o
BIN=myos.bin
ISO=myos.iso

all: build


build:
	$(AS) $(SRC)/boot.s -o boot.o
	$(AS) $(SRC)/port.s -o port.o
	$(AS) $(SRC)/enable_paging.s -o enable_paging.o
	$(AS) $(SRC)/load_gdt.s -o load_gdt.o
	$(AS) $(SRC)/load_idt.s -o load_idt.o
	$(CC) $(INC) $(C_FLAGS) $(SRC)/pit.c -o pit.o
	$(CC) $(INC) $(C_FLAGS) $(SRC)/serial.c -o serial.o
	$(CC) $(INC) $(C_FLAGS) $(SRC)/gdt.c -o gdt.o
	$(CC) $(INC) $(C_FLAGS) $(SRC)/idt.c -o idt.o
	$(CC) $(INC) $(C_FLAGS) $(SRC)/isr.c -o isr.o
	$(CC) $(INC) $(C_FLAGS) $(SRC)/paging.c -o paging.o
	$(CC) $(INC) $(C_FLAGS) $(SRC)/string.c -o string.o
	$(CC) $(INC) $(C_FLAGS) $(SRC)/vga.c -o vga.o
	$(CC) $(INC) $(C_FLAGS) $(SRC)/kernel.c -o kernel.o
	
	$(CC) -T linker.ld -o $(BIN) $(LNK_FLAGS) $(OBJ) -lgcc
	cp $(BIN) isodir/boot/
	grub-mkrescue -o $(ISO) isodir

run:
	qemu-system-i386 -cdrom $(ISO) -serial stdio

clean:
	rm *.o
	rm $(BIN)
	rm $(ISO)
	rm isodir/boot/myos.bin
