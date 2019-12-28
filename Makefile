# target should be the location of the cross compiler
#TARGET=

# the build tools are assumed to be the GNU gcc toolchain
AS=$(TARGET)-as
CC=$(TARGET)-gcc

C_FLAGS=-c -std=gnu99 -ffreestanding -O2 -Wall -Wextra

LNK_FLAGS=-ffreestanding -O2 -nostdlib

OBJ=boot.o kernel.o
BIN=myos.bin
ISO=myos.iso

all: build


build:
	$(AS) boot.s -o boot.o
	$(CC) $(C_FLAGS) kernel.c -o kernel.o
	$(CC) -T linker.ld -o $(BIN) $(LNK_FLAGS) $(OBJ) -lgcc
	cp $(BIN) isodir/boot/
	grub-mkrescue -o $(ISO) isodir

run:
	qemu-system-i386 -cdrom $(ISO)

clean:
	rm *.o
	rm $(BIN)
	rm $(ISO)
	rm isodir/boot/myos.bin



