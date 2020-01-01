# target should be the location of the cross compiler
#TARGET=

# the build tools are assumed to be the GNU gcc toolchain
AS=$(TARGET)-as
CC=$(TARGET)-gcc

C_FLAGS=-c -std=gnu99 -ffreestanding -O2 -Wall -Wextra

LNK_FLAGS=-ffreestanding -O2 -nostdlib

OBJ=boot.o port.o serial.o load_gdt.o load_idt.o gdt.o idt.o isr.o string.o vga.o kernel.o
BIN=myos.bin
ISO=myos.iso

all: build


build:
	$(AS) boot.s -o boot.o
	$(AS) port.s -o port.o
	$(AS) load_gdt.s -o load_gdt.o
	$(AS) load_idt.s -o load_idt.o
	$(CC) $(C_FLAGS) serial.c -o serial.o
	$(CC) $(C_FLAGS) gdt.c -o gdt.o
	$(CC) $(C_FLAGS) idt.c -o idt.o
	$(CC) $(C_FLAGS) isr.c -o isr.o
	$(CC) $(C_FLAGS) string.c -o string.o
	$(CC) $(C_FLAGS) vga.c -o vga.o
	$(CC) $(C_FLAGS) kernel.c -o kernel.o
	
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
